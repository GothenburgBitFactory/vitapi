////////////////////////////////////////////////////////////////////////////////
// Tegelsten - building blocks for UI
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
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "vapi.h"
#include "iapi.h"

#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)

int main (int argc, char** argv)
{
  // VAPI up.
  vapi_initialize ();
  vapi_full_screen ();
  vapi_clear ();

  // IAPI up.
  iapi_initialize ();
  iapi_raw ();
  iapi_noecho ();
  iapi_mouse_tracking ();

  // Draw a rectangle.
  Color c ("black on bright yellow");
  int r_x = 1;
  int r_y = 1;
  int r_width = 10;
  int r_height = 3;
  vapi_rectangle (r_x, r_y, r_width, r_height, c);
  vapi_text (r_x + 1, r_y + 1, c, "Drag me.");

  int width  = vapi_width ();
  int height = vapi_height ();

  vapi_text ((width - 17) / 2, height / 2, "Press 'q' to quit");
  vapi_moveto (1, 1);
  vapi_refresh ();

  // Event loop.
  bool dragging = false;
  int key;
  while ((key = iapi_getch ()) != EOF)
  {
    if (key == 'q')
      break;

    else if (key == IAPI_MOUSE_1_CLICK)
    {
      int x, y;
      iapi_mouse_pos (x, y);

      if (dragging)
      {
        r_x = MAX (1, MIN (x, width  - r_width + 1));
        r_y = MAX (1, MIN (y, height - r_height));

        vapi_clear ();
        vapi_text ((width - 17) / 2, height / 2, "Press 'q' to quit");
        vapi_rectangle (r_x, r_y, r_width, r_height, c);
        vapi_text (r_x + 1, r_y + 1, c, "Drag me.");
        vapi_moveto (1, 1);
        vapi_refresh ();
      }
      else
      {
        if (x > r_x && x < r_x + width && y > r_y && y < r_y + height)
          dragging = true;
      }
    }

    else if (key == IAPI_MOUSE_RELEASE)
      dragging = false;
  }

  // IAPI down.
  iapi_nomouse_tracking ();
  iapi_noraw ();
  iapi_echo ();
  iapi_deinitialize ();

  // VAPI down.
  vapi_end_full_screen ();
  vapi_deinitialize ();

  return 0;
}

