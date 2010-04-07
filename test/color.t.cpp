////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010, Paul Beckingham, Federico Hernandez.
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
#include <iostream>
#include <stdio.h>
#include <test.h>
#include <string.h>
#include <vitapi.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (1024);

  // Auto upgrades.
  char value [256];
  color c = color_def ("red on color0");
  color_name (value, 256, c);
  t.is (value, "color1 on color0", "upgrade red on color0 -> color1 on color0");

  c = color_def ("color1 on black");
  color_name (value, 256, c);
  t.is (value, "color1 on color0", "upgrade color1 on black -> color1 on color0");

  c = color_def ("bold red on color0");
  color_name (value, 256, c);
  t.is (value, "color9 on color0", "upgrade bold red on color0 -> color9 on color0");

  c = color_def ("color1 on bright black");
  color_name (value, 256, c);
  t.is (value, "color1 on color8", "upgrade color1 on bright black -> color1 on color8");

  // Simple blending.
  c = color_blend (color_def ("red"), color_def ("on white"));
  color_name (value, 256, c);
  t.is (value, "red on white", "red + on white -> red on white");

  c = color_blend (color_def ("bold underline red"), color_def ("on bright white"));
  color_name (value, 256, c);
  t.is (value, "bold underline red on bright white", "bold underline red + on bright white -> bold underline red on bright white");

  // Blending with conflicts.
  c = color_blend (color_def ("red on white"), color_def ("on blue"));
  color_name (value, 256, c);
  t.is (value, "red on blue", "red on white + on blue -> red on blue");

  c = color_blend (color_def ("red on white"), color_def ("blue on magenta"));
  color_name (value, 256, c);
  t.is (value, "blue on magenta", "red on white + blue on magenta -> blue on magenta");

  // Blending with upgrades.
  c = color_blend (color_def ("color1 on color0"), color_def ("blue"));
  color_name (value, 256, c);
  t.is (value, "color4 on color0", "color1 on color0 + blue -> color4 on color0");

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
  char color [24];
  char codes [64];
  char description [64];
  for (int i = 0; i < 256; ++i)
  {
    sprintf (color,       "color%d", i);
    sprintf (codes,       "\033[38;5;%dmfoo\033[0m", i);
    sprintf (description, "color%d -> ^[[38;5;%dm", i, i);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (color));
    t.is (value, codes, description);
  }

  for (int i = 0; i < 256; ++i)
  {
    sprintf (color,       "on color%d", i);
    sprintf (codes,       "\033[48;5;%dmfoo\033[0m", i);
    sprintf (description, "on color%d -> ^[[48;5;%dm", i, i);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (color));
    t.is (value, codes, description);
  }

  // RGB Color Cube.
  for (int r = 0; r < 6; ++r)
    for (int g = 0; g < 6; ++g)
      for (int b = 0; b < 6; ++b)
      {
        int code = 16 + (r*36 + g*6 + b);
        sprintf (color,       "rgb%d%d%d", r, g, b);
        sprintf (codes,       "\033[38;5;%dmfoo\033[0m", code);
        sprintf (description, "rgb%d%d%d -> ^[[38;5;%dm", r, g, b, code);

        strcpy (value, "foo");
        color_colorize (value, 256, color_def (color));
        t.is (value, codes, description);
      }

  for (int r = 0; r < 6; ++r)
    for (int g = 0; g < 6; ++g)
      for (int b = 0; b < 6; ++b)
      {
        int code = 16 + (r*36 + g*6 + b);
        sprintf (color,       "on rgb%d%d%d", r, g, b);
        sprintf (codes,       "\033[48;5;%dmfoo\033[0m", code);
        sprintf (description, "on rgb%d%d%d -> ^[[48;5;%dm", r, g, b, code);

        strcpy (value, "foo");
        color_colorize (value, 256, color_def (color));
        t.is (value, codes, description);
      }

  // 256-color, grays.
  // grey == gray.
  strcpy (value, "foo");
  color_colorize (value, 256, color_def ("grey0"));
  t.is (value, "\033[38;5;232mfoo\033[0m", description);

  for (int i = 0; i < 24; ++i)
  {
    sprintf (color,       "gray%d", i);
    sprintf (codes,       "\033[38;5;%dmfoo\033[0m", i + 232);
    sprintf (description, "gray%d -> ^[[38;5;%dm", i + 232, i + 232);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (color));
    t.is (value, codes, description);
  }

  for (int i = 0; i < 24; ++i)
  {
    sprintf (color,       "on gray%d", i);
    sprintf (codes,       "\033[48;5;%dmfoo\033[0m", i + 232);
    sprintf (description, "on gray%d -> ^[[48;5;%dm", i + 232, i + 232);

    strcpy (value, "foo");
    color_colorize (value, 256, color_def (color));
    t.is (value, codes, description);
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
