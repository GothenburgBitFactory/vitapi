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
#include <stdlib.h>
#include <vitapi.h>

int main (int argc, char** argv)
{
  if (! vapi_initialize ())
  {
    vapi_full_screen ();

    int width  = vapi_width ();
    int height = vapi_height ();

    color palette[] =
    {
      color_def ("white on black"),
      color_def ("white on red"),
      color_def ("white on blue"),
      color_def ("white on green"),
      color_def ("black on magenta"),
      color_def ("black on cyan"),
      color_def ("black on yellow"),
      color_def ("black on white")
    };

    for (int i = 0; i < 1000; ++i)
    {
      vapi_pos_color_text ((rand () % (width - 8)) + 1, (rand () % (height - 1)) + 1, palette [rand () % 8], "VAPIVAPI");
      vapi_refresh ();
    }

    for (int i = 0; i < 1000; ++i)
    {
      vapi_rectangle ((rand () % (width - 8)) + 1, (rand () % (height - 4)) + 1, 8, 4, palette [rand () % 8]);
      vapi_refresh ();
    }

    vapi_deinitialize ();
  }
  else
    std::cout << "Your $TERM ("
              << getenv ("TERM")
              << ") is not supported."
              << std::endl;

  return 0;
}

