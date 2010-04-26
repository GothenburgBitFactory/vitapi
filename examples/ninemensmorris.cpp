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
#include <unistd.h>
#include <vitapi.h>

int main (int argc, char** argv)
{
  if (! vapi_initialize ())
  {
    iapi_initialize ();
    iapi_raw ();
    iapi_noecho ();
    vapi_full_screen ();
    vapi_clear ();

    color line    = color_def ("on bright gray16");
    color point   = color_def ("on bright gray8");
    color player1 = color_def ("on yellow");
    color player2 = color_def ("on blue");

    int origin_x = (vapi_width ()  - 56) / 2;
    int origin_y = (vapi_height () - 28) / 2;

    int j = 0;
    int i = 0;

    vapi_rectangle (origin_x +  0, origin_y +  0, 56, 1, line);
    vapi_rectangle (origin_x +  9, origin_y +  4, 38, 1, line);
    vapi_rectangle (origin_x + 18, origin_y +  8, 20, 1, line);
    vapi_rectangle (origin_x + 18, origin_y + 16, 20, 1, line);
    vapi_rectangle (origin_x +  9, origin_y + 20, 38, 1, line);
    vapi_rectangle (origin_x +  0, origin_y + 24, 56, 1, line);

    vapi_rectangle (origin_x +  0, origin_y + 12, 20, 1, line);
    vapi_rectangle (origin_x + 36, origin_y + 12, 20, 1, line);

    vapi_rectangle (origin_x +  0, origin_y +  0, 2, 25, line);
    vapi_rectangle (origin_x +  9, origin_y +  4, 2, 17, line);
    vapi_rectangle (origin_x + 18, origin_y +  8, 2,  9, line);
    vapi_rectangle (origin_x + 36, origin_y +  8, 2,  9, line);
    vapi_rectangle (origin_x + 45, origin_y +  4, 2, 17, line);
    vapi_rectangle (origin_x + 54, origin_y +  0, 2, 25, line);

    vapi_rectangle (origin_x + 27, origin_y +  0, 2,  9, line);
    vapi_rectangle (origin_x + 27, origin_y + 16, 2,  9, line);

    for (i =  0; i <= 54; i+=27)
      vapi_rectangle (origin_x + i, origin_y +  0, 2,  1, point);
    for (i =  9; i <= 45; i+=18)
      vapi_rectangle (origin_x + i, origin_y +  4, 2,  1, point);
    for (i = 18; i <= 36; i+=9)
      vapi_rectangle (origin_x + i, origin_y +  8, 2,  1, point);
    for (i =  0; i <= 18; i+=9)
    {
      vapi_rectangle (origin_x + i     , origin_y + 12, 2,  1, point);
      vapi_rectangle (origin_x + i + 36, origin_y + 12, 2,  1, point);
    }
    for (i = 18; i <= 36; i+=9)
      vapi_rectangle (origin_x + i, origin_y + 16, 2,  1, point);
    for (i =  9; i <= 45; i+=18)
      vapi_rectangle (origin_x + i, origin_y + 20, 2,  1, point);
    for (i =  0; i <= 54; i+=27)
      vapi_rectangle (origin_x + i, origin_y + 24, 2,  1, point);

    vapi_rectangle (origin_x + 43, origin_y +  3, 6,  3, player2);
    vapi_rectangle (origin_x + 43, origin_y + 11, 6,  3, player2);
    vapi_rectangle (origin_x + 43, origin_y + 19, 6,  3, player2);
    vapi_rectangle (origin_x +  7, origin_y + 11, 6,  3, player2);
    vapi_rectangle (origin_x + 16, origin_y + 11, 6,  3, player2);
    vapi_rectangle (origin_x +  7, origin_y + 19, 6,  3, player2);

    vapi_rectangle (origin_x + 34, origin_y +  7, 6,  3, player1);
    vapi_rectangle (origin_x + 34, origin_y + 11, 6,  3, player1);
    vapi_rectangle (origin_x + 34, origin_y + 15, 6,  3, player1);
    vapi_rectangle (origin_x + 25, origin_y + 19, 6,  3, player1);
    vapi_rectangle (origin_x + 25, origin_y + 23, 6,  3, player1);

    vapi_moveto (1, 1);
    vapi_refresh ();
    int key;
    while ((key = iapi_getch ()) != EOF)
    {
      if (key == 'q')
        break;
    }
    iapi_echo ();
    iapi_noraw ();
    iapi_deinitialize ();
    vapi_deinitialize ();
  }
  else
    std::cout << "Your $TERM ("
              << getenv ("TERM")
              << ") is not supported."
              << std::endl;

  return 0;
}

