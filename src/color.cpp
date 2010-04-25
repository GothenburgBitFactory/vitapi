////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2009 - 2010, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
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
  // deprecated.
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
    else if (word == "on")        bg = true;

    // X where X is one of black, red, blue ...
    else if ((index = color_index (word)) != -1)
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
extern "C" void color_name (char* buf, size_t size, color c)
{
  CHECK0 (buf, "Null buffer pointer passed to color_name.");
  CHECKC0 (c,  "Invalid color passed to color_name.");

  std::string description;
  if (c & _COLOR_BOLD) description += "bold";

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
    return;
  }

  strncpy (buf, description.c_str (), size);
}

////////////////////////////////////////////////////////////////////////////////
// Convert a color -> bit description
extern "C" void color_decode (char* buf, size_t size, color c)
{
  CHECK0 (buf, "Null buffer pointer passed to color_decode.");
  CHECKC0 (c,  "Invalid color passed to color_decode.");

  std::stringstream description;

  description << ((c & _COLOR_256)       ? "256-" : "-");
  description << ((c & _COLOR_HASBG)     ? "BG-"  : "-");
  description << ((c & _COLOR_HASFG)     ? "FG-"  : "-");
  description << ((c & _COLOR_UNDERLINE) ? "U-"   : "-");
  description << ((c & _COLOR_BOLD)      ? "BO-"  : "-");
  description << ((c & _COLOR_BRIGHT)    ? "BR-"  : "-");

  int bg = (c & _COLOR_BG) >> 8;
  if (c & _COLOR_HASBG)
    description << bg;
  else
    description << "-";

  int fg = c & _COLOR_FG;
  if (c & _COLOR_HASFG)
    description << fg;
  else
    description << "-";

  if (description.str ().length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to color_decode.");
    return;
  }

  strncpy (buf, description.str ().c_str (), size);
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
extern "C" color color_downgrade (color)
{
  vitapi_set_error ("color_downgrade is not implemented.");
  return -1;
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

  one |= (two & _COLOR_UNDERLINE);    // Always inherit underline.

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
extern "C" void color_colorize (char* buf, size_t size, color c)
{
  CHECK0 (buf, "Null buffer pointer passed to color_colorize.");
  CHECKC0 (c, "Invalid color passed to color_colorize.");

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
    return;
  }

  strncpy (buf, result.str ().c_str (), size);
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
