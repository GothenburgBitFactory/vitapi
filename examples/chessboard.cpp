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

#include "vapi.h"

int main (int argc, char** argv)
{
  vapi_initialize ();
  vapi_full_screen ();
  vapi_clear ();

  Color black ("on bright black");
  Color white ("on bright white");

  int origin_x = (vapi_width ()  - 8 * 3 * 2) / 2;
  int origin_y = (vapi_height () - 8 * 3)     / 2;

  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      vapi_rectangle (origin_x + (col * 3 * 2),
                      origin_y + (row * 3),
                      3 * 2,
                      3,
                      ((row + col) % 2 == 0 ? black : white));

  vapi_refresh ();
  sleep (4);
  vapi_deinitialize ();
  return 0;
}

