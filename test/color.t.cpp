////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2017, Göteborg Bit Factory.
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
#include <iostream>
#include <stdio.h>
#include <test.h>
#include <string.h>
#include <vitapi.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (1048);

  // Non-color.
  t.is (color_def ("none"),         0, "none -> 0");
  t.is (color_def ("on none"),      0, "on none -> 0");
  t.is (color_def ("none on none"), 0, "none on none -> 0");

  // Auto upgrades.
  char value [256];
  color c = color_def ("red on color0");
  t.is (color_name (value, 256, c), "color1 on color0", "upgrade red on color0 -> color1 on color0");

  c = color_def ("color1 on black");
  t.is (color_name (value, 256, c), "color1 on color0", "upgrade color1 on black -> color1 on color0");

  c = color_def ("bold red on color0");
  t.is (color_name (value, 256, c), "color9 on color0", "upgrade bold red on color0 -> color9 on color0");

  c = color_def ("color1 on bright black");
  t.is (color_name (value, 256, c), "color1 on color8", "upgrade color1 on bright black -> color1 on color8");

  // Simple blending.
  c = color_blend (color_def ("red"), color_def ("on white"));
  t.is (color_name (value, 256, c), "red on white", "red + on white -> red on white");

  c = color_blend (color_def ("bold underline red"), color_def ("on bright white"));
  t.is (color_name (value, 256, c), "bold underline red on bright white", "bold underline red + on bright white -> bold underline red on bright white");

  t.is (color_blend (color_def ("red"), -1), color_def ("red"), "blend red, -1 -> red");
  t.is (color_blend (-1, color_def ("red")), color_def ("red"), "blend -1, red, -> red");

  // Blending with conflicts.
  c = color_blend (color_def ("red on white"), color_def ("on blue"));
  t.is (color_name (value, 256, c), "red on blue", "red on white + on blue -> red on blue");

  c = color_blend (color_def ("red on white"), color_def ("blue on magenta"));
  t.is (color_name (value, 256, c), "blue on magenta", "red on white + blue on magenta -> blue on magenta");

  // Blending with upgrades.
  c = color_blend (color_def ("color1 on color0"), color_def ("blue"));
  t.is (color_name (value, 256, c), "color4 on color0", "color1 on color0 + blue -> color4 on color0");

  // Now the dumb show of every color and its code.
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("red"));
  t.is (value, "\033[31mfoo\033[0m",     "red                  -> ^[[31m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("bold red"));
  t.is (value, "\033[1;31mfoo\033[0m",   "bold red             -> ^[[1;31m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("underline red"));
  t.is (value, "\033[4;31mfoo\033[0m",   "underline red        -> ^[[4;31m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("underline bold red"));
  t.is (value, "\033[1;4;31mfoo\033[0m", "underline bold red   -> ^[[1;4;31m");

  // 16-color foregrounds.
  strcpy (value, "foo"); color_colorize (value, 256, color_def (""));
  t.is (value, "foo",                    "''                   -> ^[[31m");

  strcpy (value, "foo"); color_colorize (value, 256, color_def ("black"));
  t.is (value, "\033[30mfoo\033[0m",     "black                -> ^[[30m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("red"));
  t.is (value, "\033[31mfoo\033[0m",     "red                  -> ^[[31m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("green"));
  t.is (value, "\033[32mfoo\033[0m",     "green                -> ^[[32m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("yellow"));
  t.is (value, "\033[33mfoo\033[0m",     "yellow               -> ^[[33m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("blue"));
  t.is (value, "\033[34mfoo\033[0m",     "blue                 -> ^[[34m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("magenta"));
  t.is (value, "\033[35mfoo\033[0m",     "magenta              -> ^[[35m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("cyan"));
  t.is (value, "\033[36mfoo\033[0m",     "cyan                 -> ^[[36m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("white"));
  t.is (value, "\033[37mfoo\033[0m",     "white                -> ^[[37m");

  // 16-color backgrounds.
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on bright black"));
  t.is (value, "\033[100mfoo\033[0m",    "on bright black      -> ^[[100m");

  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on black"));
  t.is (value, "\033[40mfoo\033[0m",     "on black             -> ^[[40m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on red"));
  t.is (value, "\033[41mfoo\033[0m",     "on red               -> ^[[41m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on green"));
  t.is (value, "\033[42mfoo\033[0m",     "on green             -> ^[[42m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on yellow"));
  t.is (value, "\033[43mfoo\033[0m",     "on yellow            -> ^[[43m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on blue"));
  t.is (value, "\033[44mfoo\033[0m",     "on blue              -> ^[[44m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on magenta"));
  t.is (value, "\033[45mfoo\033[0m",     "on magenta           -> ^[[45m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on cyan"));
  t.is (value, "\033[46mfoo\033[0m",     "on cyan              -> ^[[46m");
  strcpy (value, "foo"); color_colorize (value, 256, color_def ("on white"));
  t.is (value, "\033[47mfoo\033[0m",     "on white             -> ^[[47m");

  // 256-color, basic colors.
  char name [24];
  char codes [64];
  char description [64];
  for (int i = 0; i < 256; ++i)
  {
    sprintf (name,        "color%d", i);
    sprintf (codes,       "\033[38;5;%dmfoo\033[0m", i);
    sprintf (description, "color%d -> ^[[38;5;%dm", i, i);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (name));
    t.is (value, codes, description);
  }

  for (int i = 0; i < 256; ++i)
  {
    sprintf (name,        "on color%d", i);
    sprintf (codes,       "\033[48;5;%dmfoo\033[0m", i);
    sprintf (description, "on color%d -> ^[[48;5;%dm", i, i);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (name));
    t.is (value, codes, description);
  }

  // RGB Color Cube.
  for (int r = 0; r < 6; ++r)
    for (int g = 0; g < 6; ++g)
      for (int b = 0; b < 6; ++b)
      {
        int code = 16 + (r*36 + g*6 + b);
        sprintf (name,        "rgb%d%d%d", r, g, b);
        sprintf (codes,       "\033[38;5;%dmfoo\033[0m", code);
        sprintf (description, "rgb%d%d%d -> ^[[38;5;%dm", r, g, b, code);

        strcpy (value, "foo");
        color_colorize (value, 256, color_def (name));
        t.is (value, codes, description);
      }

  for (int r = 0; r < 6; ++r)
    for (int g = 0; g < 6; ++g)
      for (int b = 0; b < 6; ++b)
      {
        int code = 16 + (r*36 + g*6 + b);
        sprintf (name,        "on rgb%d%d%d", r, g, b);
        sprintf (codes,       "\033[48;5;%dmfoo\033[0m", code);
        sprintf (description, "on rgb%d%d%d -> ^[[48;5;%dm", r, g, b, code);

        strcpy (value, "foo");
        color_colorize (value, 256, color_def (name));
        t.is (value, codes, description);
      }

  // 256-color, grays.
  // grey == gray.
  strcpy (value, "foo");
  color_colorize (value, 256, color_def ("grey0"));
  t.is (value, "\033[38;5;232mfoo\033[0m", description);

  for (int i = 0; i < 24; ++i)
  {
    sprintf (name,        "gray%d", i);
    sprintf (codes,       "\033[38;5;%dmfoo\033[0m", i + 232);
    sprintf (description, "gray%d -> ^[[38;5;%dm", i + 232, i + 232);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (name));
    t.is (value, codes, description);
  }

  for (int i = 0; i < 24; ++i)
  {
    sprintf (name,        "on gray%d", i);
    sprintf (codes,       "\033[48;5;%dmfoo\033[0m", i + 232);
    sprintf (description, "on gray%d -> ^[[48;5;%dm", i + 232, i + 232);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (name));
    t.is (value, codes, description);
  }

  // Test color_def ("123456789") which, in combination with color_name can
  // be tested via round-trip conversion.
  strcpy (description, "bold underline red on bright yellow");
  c = color_def (description);
  sprintf (value, "%d", c);
  c = color_def (value);
  t.is (color_name (value, 256, c), description, "round-trip");

  // Downgrading is mapping from 256 -> 16 or 256 -> 8 colors.  Foreground first.
  color white = color_def ("rgb555");
  t.is (color_downgrade (white, 1), -1, "Cannot downgrade fg to 1 color");
  t.is (color_downgrade (white, 2), -1, "Cannot downgrade fg to 2 color");
  t.is (color_downgrade (white, 4), -1, "Cannot downgrade fg to 4 color");

  t.is (color_downgrade (white, 16), color_def ("bold white"), "rgb555 -> @16 bold white");
  t.is (color_downgrade (white, 8),  color_def ("white"),      "rgb555 -> @8 white");

  color grey = color_def ("rgb011");
  t.is (color_downgrade (grey, 16),  color_def ("bold black"), "rgb011 -> @16 bold black");
  t.is (color_downgrade (grey, 8),   color_def ("black"),      "rgb011 -> @8 black");

  color black = color_def ("rgb000");
  t.is (color_downgrade (black, 16), color_def ("black"),      "rgb011 -> @16 black");
  t.is (color_downgrade (black, 8),  color_def ("black"),      "rgb011 -> @8 black");

  // Now background.
  white = color_def ("on rgb555");
  t.is (color_downgrade (white, 1), -1, "Cannot downgrade bg to 1 color");
  t.is (color_downgrade (white, 2), -1, "Cannot downgrade bg to 2 color");
  t.is (color_downgrade (white, 4), -1, "Cannot downgrade bg to 4 color");

  t.is (color_downgrade (white, 16), color_def ("on bright white"), "on rgb555 -> @16 on bright white");
  t.is (color_downgrade (white, 8),  color_def ("on white"),        "on rgb555 -> @8 on white");

  grey = color_def ("on rgb011");
  t.is (color_downgrade (grey, 16),  color_def ("on bright black"), "on rgb011 -> @16 on bright black");
  t.is (color_downgrade (grey, 8),   color_def ("on black"),        "on rgb011 -> @8 on black");

  black = color_def ("on rgb000");
  t.is (color_downgrade (black, 16), color_def ("on black"),        "on rgb011 -> @16 on black");
  t.is (color_downgrade (black, 8),  color_def ("on black"),        "on rgb011 -> @8 on black");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
