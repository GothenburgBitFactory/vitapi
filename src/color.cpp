////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2009 - 2017, Göteborg Bit Factory.
// All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <util.h>
#include <vitapi.h>
#include <check.h>

static std::string error;
static char color_names[][16] =
{
  //   0        1      2        3         4       5          6       7        8
  "none", "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white", ""
};

#define NUM_COLORS (sizeof (color_names) / sizeof (color_names[0]))

static int color_index (const std::string&);
static std::string color_fg (color);
static std::string color_bg (color);
static void rgb (int, int&, int&, int&);
static int euclidean_distance (int, int, int, int, int, int);

////////////////////////////////////////////////////////////////////////////////
// Supports the following constructs:
//   [bright] [color] [on color] [bright] [underline]
//
// Where [color] is one of:
//   black
//   red
//   ...
//   grayN  0 <= N <= 23       fg 38;5;232 + N              bg 48;5;232 + N
//   greyN  0 <= N <= 23       fg 38;5;232 + N              bg 48;5;232 + N
//   colorN 0 <= N <= 255      fg 38;5;N                    bg 48;5;N
//   rgbRGB 0 <= R,G,B <= 5    fg 38;5;16 + R*36 + G*6 + B  bg 48;5;16 + R*36 + G*6 + B
extern "C" color color_def (const char* def)
{
  CHECK1 (def, "Null pointer to a color definition passed to color_def.");

  // The color that is being constructed.
  color c = 0;

  // Special case - if def contains only digits, then consider it a color code
  // that is in string form, and simply convert it using atoi.
  if (digitsOnly (def))
    return atoi (def);

  // By converting underscores to spaces, we inherently support the old "on_red"
  // style of specifying background colors.  We consider underscores to be
  // deprecated, but convenient.
  std::string modifiable_spec = def;
  std::replace (modifiable_spec.begin (), modifiable_spec.end (), '_', ' ');

  // Split spec into words.
  std::vector <std::string> words;
  split (words, modifiable_spec, ' ');

  // Construct the color as two separate colors, then blend them later.  This
  // make it possible to declare a color such as "color1 on black", and have
  // the upgrade work properly.
  color fg_value = 0;
  color bg_value = 0;

  bool bg = false;
  int index;
  std::string word;
  std::vector <std::string>::iterator it;
  for (it = words.begin (); it != words.end (); ++it)
  {
    word = lowerCase (trim (*it));

         if (word == "bold")      fg_value |= _COLOR_BOLD;
    else if (word == "bright")    bg_value |= _COLOR_BRIGHT;
    else if (word == "underline") fg_value |= _COLOR_UNDERLINE;
    else if (word == "inverse")   fg_value |= _COLOR_INVERSE;
    else if (word == "on")        bg = true;

    // X where X is one of black, red, blue ...
    else if ((index = color_index (word)) != -1)
    {
      if (index)
      {
        if (bg)
        {
          bg_value |= _COLOR_HASBG;
          bg_value |= index << 8;
        }
        else
        {
          fg_value |= _COLOR_HASFG;
          fg_value |= index;
        }
      }
    }

    // greyN/grayN, where 0 <= N <= 23.
    else if (word.substr (0, 4) == "grey" ||
             word.substr (0, 4) == "gray")
    {
      index = atoi (word.substr (4).c_str ());
      if (index < 0 || index > 23)
      {
        vitapi_set_error ("The color '" + *it + "' is not recognized.");
        return -1;
      }

      if (bg)
      {
        bg_value |= _COLOR_HASBG;
        bg_value |= (index + 232) << 8;
        bg_value |= _COLOR_256;
      }
      else
      {
        fg_value |= _COLOR_HASFG;
        fg_value |= index + 232;
        fg_value |= _COLOR_256;
      }
    }

    // rgbRGB, where 0 <= R,G,B <= 5.
    else if (word.substr (0, 3) == "rgb")
    {
      index = atoi (word.substr (3).c_str ());
      if (word.length () != 6 ||
          index < 0 || index > 555)
      {
        vitapi_set_error ("The color '" + *it + "' is not recognized.");
        return -1;
      }

      int r = atoi (word.substr (3, 1).c_str ());
      int g = atoi (word.substr (4, 1).c_str ());
      int b = atoi (word.substr (5, 1).c_str ());
      if (r < 0 || r > 5 ||
          g < 0 || g > 5 ||
          b < 0 || b > 5)
      {
        vitapi_set_error ("The color '" + *it + "' is not recognized.");
        return -1;
      }

      index = 16 + r*36 + g*6 + b;

      if (bg)
      {
        bg_value |= _COLOR_HASBG;
        bg_value |= index << 8;
        bg_value |= _COLOR_256;
      }
      else
      {
        fg_value |= _COLOR_HASFG;
        fg_value |= index;
        fg_value |= _COLOR_256;
      }
    }

    // colorN, where 0 <= N <= 255.
    else if (word.substr (0, 5) == "color")
    {
      index = atoi (word.substr (5).c_str ());
      if (index < 0 || index > 255)
      {
        vitapi_set_error ("The color '" + *it + "' is not recognized.");
        return -1;
      }

      // TODO Determine why this was thought necessary.
      // c = upgrade (c);

      if (bg)
      {
        bg_value |= _COLOR_HASBG;
        bg_value |= index << 8;
        bg_value |= _COLOR_256;
      }
      else
      {
        fg_value |= _COLOR_HASFG;
        fg_value |= index;
        fg_value |= _COLOR_256;
      }
    }
    else if (word != "")
    {
      vitapi_set_error ("The color '" + *it + "' is not recognized.");
      return -1;
    }
  }

  // Now combine the fg and bg into a single color.
  return c = color_blend (fg_value, bg_value);
}

////////////////////////////////////////////////////////////////////////////////
// Convert a color -> description
extern "C" const char* color_name (char* buf, size_t size, color c)
{
  if (!buf)
  {
    vitapi_set_error ("Null buffer pointer passed to color_name.");
    return NULL;
  }

  if (c == -1)
  {
    vitapi_set_error ("Invalid color passed to color_name.");
    return NULL;
  }

  std::string description;
  if (c & _COLOR_BOLD) description += "bold";

  if (c & _COLOR_INVERSE)
    description += std::string (description.length () ? " " : "") + "inverse";

  if (c & _COLOR_UNDERLINE)
    description += std::string (description.length () ? " " : "") + "underline";

  if (c & _COLOR_HASFG)
    description += std::string (description.length () ? " " : "") + color_fg (c);

  if (c & _COLOR_HASBG)
  {
    description += std::string (description.length () ? " " : "") + "on";

    if (c & _COLOR_BRIGHT)
      description += std::string (description.length () ? " " : "") + "bright";

    description += " " + color_bg (c);
  }

  if (description.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to color_name.");
    return buf;
  }

  strncpy (buf, description.c_str (), size);
  return buf;
}

////////////////////////////////////////////////////////////////////////////////
// Convert a color -> bit description
extern "C" const char* color_decode (char* buf, size_t size, color c)
{
  if (!buf)
  {
    vitapi_set_error ("Null buffer pointer passed to color_decode.");
    return NULL;
  }

  if (c == -1)
  {
    vitapi_set_error ("Invalid color passed to color_decode.");
    return NULL;
  }

  std::stringstream description;

  description << ((c & _COLOR_256)       ? "256-" : "-");
  description << ((c & _COLOR_HASBG)     ? "BG-"  : "-");
  description << ((c & _COLOR_HASFG)     ? "FG-"  : "-");
  description << ((c & _COLOR_INVERSE)   ? "I-"   : "-");
  description << ((c & _COLOR_UNDERLINE) ? "U-"   : "-");
  description << ((c & _COLOR_BOLD)      ? "BO-"  : "-");
  description << ((c & _COLOR_BRIGHT)    ? "BR-"  : "-");

  int bg = (c & _COLOR_BG) >> 8;
  if (c & _COLOR_HASBG) description << bg;
  description << "-";

  int fg = c & _COLOR_FG;
  if (c & _COLOR_HASFG) description << fg;
  description << "-";

  if (description.str ().length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to color_decode.");
    return buf;
  }

  strncpy (buf, description.str ().c_str (), size);
  return buf;
}

////////////////////////////////////////////////////////////////////////////////
// Convert 16- to 256-color
extern "C" color color_upgrade (color c)
{
  CHECKC1 (c, "Invalid color passed to color_upgrade.");

  if (!(c & _COLOR_256))
  {
    if (c & _COLOR_HASFG)
    {
      bool bold = c & _COLOR_BOLD;
      unsigned int fg = c & _COLOR_FG;
      c &= ~_COLOR_FG;
      c &= ~_COLOR_BOLD;
      c |= (bold ? fg + 7 : fg - 1);
    }

    if (c & _COLOR_HASBG)
    {
      bool bright = c & _COLOR_BRIGHT;
      unsigned int bg = (c & _COLOR_BG) >> 8;
      c &= ~_COLOR_BG;
      c &= ~_COLOR_BRIGHT;
      c |= (bright ? bg + 7 : bg - 1) << 8;
    }

    c |= _COLOR_256;
  }

  return c;
}

////////////////////////////////////////////////////////////////////////////////
// Convert 256- to 16-color, with loss
extern "C" color color_downgrade (color c, int quantity)
{
  if (quantity != _COLOR_QUANTIZE_8 &&
      quantity != _COLOR_QUANTIZE_16)
  {
    vitapi_set_error ("Color downgrade only supports 8 or 16 colors.");
    return -1;
  }

  // Convert to 256 colors, to remove an entire dimension from the matrix.
  if (!(c & _COLOR_256))
    c = color_upgrade (c);

  color new_color = 0;

  if (quantity == _COLOR_QUANTIZE_16)
  {
    static struct
    {
      int i;
      int r;
      int g;
      int b;
    } all[] =
    {
      {1,  0, 0, 0},  // black
      {2,  3, 0, 0},  // red
      {3,  0, 3, 0},  // green
      {4,  3, 3, 0},  // yellow
      {5,  0, 0, 3},  // blue
      {6,  3, 0, 3},  // magenta
      {7,  0, 3, 3},  // cyan
      {8,  3, 3, 3},  // white

      {9,  1, 1, 1},  // light black
      {10, 5, 1, 1},  // light red
      {11, 0, 5, 0},  // light green
      {12, 5, 5, 0},  // light yellow
      {13, 0, 0, 5},  // light blue
      {14, 5, 0, 5},  // light magenta
      {15, 0, 5, 5},  // light cyan
      {16, 5, 5, 5},  // light white
    };

    if (c & _COLOR_HASFG)
    {
      int r, g, b;
      rgb (c & _COLOR_FG, r, g, b);

      int distance = 376; // Max value + 1;
      int choice = 0;
      for (int i = 0; i < 16; ++i)
      {
        int value = euclidean_distance (r, g, b, all[i].r, all[i].g, all[i].b);
        if (value < distance)
        {
          distance = value;
          choice = all[i].i;
        }
      }

      new_color |= _COLOR_HASFG;
      if (choice > 8)
      {
        new_color |= _COLOR_BOLD;
        new_color |= choice - 8;
      }
      else
      {
        new_color |= choice;
      }
    }

    if (c & _COLOR_HASBG)
    {
      int r, g, b;
      rgb ((c & _COLOR_BG) >> 8, r, g, b);

      int distance = 376; // Max value + 1;
      int choice = 0;
      for (int i = 0; i < 16; ++i)
      {
        int value = euclidean_distance (r, g, b, all[i].r, all[i].g, all[i].b);
        if (value < distance)
        {
          distance = value;
          choice = all[i].i;
        }
      }

      new_color |= _COLOR_HASBG;
      if (choice > 8)
      {
        new_color |= _COLOR_BRIGHT;
        new_color |= ((choice - 8) << 8);
      }
      else
      {
        new_color |= (choice << 8);
      }
    }
  }

  // Determine the rgb value for the color.
  else if (quantity == _COLOR_QUANTIZE_8)
  {
    static struct
    {
      int i;
      int r;
      int g;
      int b;
    } all[] =
    {
      {1, 0, 0, 0},  // black
      {2, 3, 0, 0},  // red
      {3, 0, 3, 0},  // green
      {4, 3, 3, 0},  // yellow
      {5, 0, 0, 3},  // blue
      {6, 3, 0, 3},  // magenta
      {7, 0, 3, 3},  // cyan
      {8, 3, 3, 3},  // white
    };

    if (c & _COLOR_HASFG)
    {
      int r, g, b;
      rgb (c & _COLOR_FG, r, g, b);

      int distance = 376; // Max value + 1;
      int choice = 0;
      for (int i = 0; i < 8; ++i)
      {
        int value = euclidean_distance (r, g, b, all[i].r, all[i].g, all[i].b);
        if (value < distance)
        {
          distance = value;
          choice = all[i].i;
        }
      }

      new_color |= _COLOR_HASFG;
      new_color |= choice;
    }

    if (c & _COLOR_HASBG)
    {
      int r, g, b;
      rgb ((c & _COLOR_BG) >> 8, r, g, b);

      int distance = 376; // Max value + 1;
      int choice = 0;
      for (int i = 0; i < 8; ++i)
      {
        int value = euclidean_distance (r, g, b, all[i].r, all[i].g, all[i].b);
        if (value < distance)
        {
          distance = value;
          choice = all[i].i;
        }
      }

      new_color |= _COLOR_HASBG;
      new_color |= (choice << 8);
    }
  }

  return new_color;
}

////////////////////////////////////////////////////////////////////////////////
// Blend two colors, possible upgrade.  If 'two' has styles that are
// compatible, merge them into 'one'.  Colors in 'two' take precedence.
extern "C" color color_blend (color one, color two)
{
  // Cannot blend bad colors.
  if (one == -1 && two == -1)
  {
    vitapi_set_error ("Two invalid colors passed to color_blend.");
    return -1;
  }

  if (one == -1) return two;          // if one is bad, result -> two
  if (two == -1) return one;          // if two is bad, result -> one

  // Blending two non-colors is a NOP.
  if (one == 0 && two == 0)
    return one;

  one |= (two & _COLOR_UNDERLINE);    // Always inherit underline.
  one |= (two & _COLOR_INVERSE);      // Always inherit inverse.

  // 16 <-- 16.
  if (!(one & _COLOR_256) &&
      !(two & _COLOR_256))
  {
    one |= (two & _COLOR_BOLD);       // Inherit bold.
    one |= (two & _COLOR_BRIGHT);     // Inherit bright.

    if (two & _COLOR_HASFG)
    {
      one |= _COLOR_HASFG;            // There is now a color.
      one &= ~_COLOR_FG;              // Remove previous color.
      one |= (two & _COLOR_FG);       // Apply other color.
    }

    if (two & _COLOR_HASBG)
    {
      one |= _COLOR_HASBG;            // There is now a color.
      one &= ~_COLOR_BG;              // Remove previous color.
      one |= (two & _COLOR_BG);       // Apply other color.
    }
  }
  else
  {
    // Upgrade either color, if necessary.
    if (!(one & _COLOR_256)) one = color_upgrade (one);
    if (!(two & _COLOR_256)) two = color_upgrade (two);

    // 256 <-- 256.
    if (two & _COLOR_HASFG)
    {
      one |= _COLOR_HASFG;            // There is now a color.
      one &= ~_COLOR_FG;              // Remove previous color.
      one |= (two & _COLOR_FG);       // Apply other color.
    }

    if (two & _COLOR_HASBG)
    {
      one |= _COLOR_HASBG;            // There is now a color.
      one &= ~_COLOR_BG;              // Remove previous color.
      one |= (two & _COLOR_BG);       // Apply other color.
    }
  }

  return one;
}

/////////////////////////////////////////////////////////////////////////////////
// Sample color codes:
//   red                  \033[31m
//   bold red             \033[91m
//   underline red        \033[4;31m
//   bold underline red   \033[1;4;31m
//
//   on red               \033[41m
//   on bright red        \033[101m
//
//   256 fg               \033[38;5;Nm
//   256 bg               \033[48;5;Nm
extern "C" const char* color_colorize (char* buf, size_t size, color c)
{
  if (!buf)
  {
    vitapi_set_error ("Null buffer pointer passed to color_colorize.");
    return NULL;
  }

  if (c == -1)
  {
    vitapi_set_error ("Invalid color passed to color_colorize.");
    return NULL;
  }

  int count = 0;
  std::stringstream result;

  // 256 color
  if (c & _COLOR_256)
  {
    bool needTerminator = false;

    if (c & _COLOR_UNDERLINE)
    {
      result << "\033[4m";
      needTerminator = true;
    }

    if (c & _COLOR_INVERSE)
    {
      result << "\033[7m";
      needTerminator = true;
    }

    if (c & _COLOR_HASFG)
    {
      result << "\033[38;5;" << (c & _COLOR_FG) << "m";
      needTerminator = true;
    }

    if (c & _COLOR_HASBG)
    {
      result << "\033[48;5;" << ((c & _COLOR_BG) >> 8) << "m";
      needTerminator = true;
    }

    result << buf;
    if (needTerminator)
      result << "\033[0m";
  }

  // 16 color, nontrivial
  else if (c != 0)
  {
    result << "\033[";

    if (c & _COLOR_BOLD)
    {
      if (count++) result << ";";
      result << "1";
    }

    if (c & _COLOR_UNDERLINE)
    {
      if (count++) result << ";";
      result << "4";
    }

    if (c & _COLOR_INVERSE)
    {
      if (count++) result << ";";
      result << "7";
    }

    if (c & _COLOR_HASFG)
    {
      if (count++) result << ";";
      result << (29 + (c & _COLOR_FG));
    }

    if (c & _COLOR_HASBG)
    {
      if (count++) result << ";";
      result << ((c & _COLOR_BRIGHT ? 99 : 39) + ((c & _COLOR_BG) >> 8));
    }

    result << "m" << buf << "\033[0m";
  }

  // Trivial
  else
    result << buf;

  if (result.str ().size () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to color_colorize.");
    return buf;
  }

  strncpy (buf, result.str ().c_str (), size);
  return buf;
}

////////////////////////////////////////////////////////////////////////////////
static int color_index (const std::string& input)
{
  for (int i = 0; color_names[i][0]; ++i)
    if (input == color_names[i])
      return (int) i;

  return -1;
}

////////////////////////////////////////////////////////////////////////////////
static std::string color_fg (color c)
{
  int index = c & _COLOR_FG;

  if (c & _COLOR_256)
  {
    if (c & _COLOR_HASFG)
    {
      std::stringstream s;
      s << "color" << (c & _COLOR_FG);
      return s.str ();
    }
  }
  else if (index >= 0 && index < NUM_COLORS)
    return color_names[index];

  return "";
}

////////////////////////////////////////////////////////////////////////////////
static std::string color_bg (color c)
{
  int index = (c & _COLOR_BG) >> 8;

  if (c & _COLOR_256)
  {
    if (c & _COLOR_HASBG)
    {
      std::stringstream s;
      s << "color" << ((c & _COLOR_BG) >> 8);
      return s.str ();
    }
  }
  else if (index >= 0 && index < NUM_COLORS)
    return color_names[index];

  return "";
}

////////////////////////////////////////////////////////////////////////////////
static void rgb (int i, int& r, int& g, int& b)
{
  // Basic 0 - 15
  if (i < 16)
  {
    switch (i)
    {
      case 0:  r = 0; g = 0; b = 0; break;
      case 1:  r = 3; g = 0; b = 0; break;
      case 2:  r = 0; g = 3; b = 0; break;
      case 3:  r = 3; g = 3; b = 0; break;
      case 4:  r = 0; g = 0; b = 3; break;
      case 5:  r = 3; g = 0; b = 3; break;
      case 6:  r = 0; g = 3; b = 3; break;
      case 7:  r = 3; g = 3; b = 3; break;
      case 8:  r = 1; g = 1; b = 1; break;
      case 9:  r = 5; g = 1; b = 1; break;
      case 10: r = 0; g = 5; b = 0; break;
      case 11: r = 5; g = 5; b = 0; break;
      case 12: r = 0; g = 0; b = 5; break;
      case 13: r = 5; g = 0; b = 5; break;
      case 14: r = 0; g = 5; b = 5; break;
      case 15: r = 5; g = 5; b = 5; break;
    }
  }

  // Color cube 16 - 231
  else if (i < 232)
  {
    r =  (i - 16) / 36;
    g = ((i - 16) % 36) / 6;
    b =  (i - 16) % 6;
  }

  // Gray ramp 232 - 255
  else
  {
    r = g = b = ((i - 232) / 4);
  }
}

////////////////////////////////////////////////////////////////////////////////
// No point performing the square root.
static int euclidean_distance (int r1, int g1, int b1, int r2, int g2, int b2)
{
  return ((r1 - r2) * (r1 - r2)) +
         ((g1 - g2) * (g1 - g2)) +
         ((b1 - b2) * (b1 - b2));
}

////////////////////////////////////////////////////////////////////////////////
