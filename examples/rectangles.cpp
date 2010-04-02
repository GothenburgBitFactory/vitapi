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

int main (int argc, char** argv)
{
  vapi_initialize ();
  vapi_full_screen ();
  vapi_clear ();
  vapi_rectangle ( 8,  4, 24, 12, Color ("on rgb001"));
  vapi_rectangle (12,  6, 24, 12, Color ("on rgb002"));
  vapi_rectangle (16,  8, 24, 12, Color ("on rgb003"));
  vapi_rectangle (20, 10, 24, 12, Color ("on rgb004"));
  vapi_rectangle (24, 12, 24, 12, Color ("on rgb005"));

  int width  = vapi_width ();
  int height = vapi_height ();

  char c[24];
  for (int i = 1; i < (height < 24 ? height : 24); ++i)
  {
    sprintf (c, "on gray%d", i);
    vapi_text (width - 12, i + 1, Color (c), "        ");
  }

  Color corner ("bold red");
  vapi_text (        1,          1, corner, "TL---->");
  vapi_text (        1,          2, corner, "|"      );
  vapi_text (        1,          3, corner, "|"      );
  vapi_text (        1,          4, corner, "v"      );

  vapi_text (width - 6,          1, corner, "<----TR");
  vapi_text (    width,          2, corner,       "|");
  vapi_text (    width,          3, corner,       "|");
  vapi_text (    width,          4, corner,       "v");

  vapi_text (        1, height - 4, corner, "^"      );
  vapi_text (        1, height - 3, corner, "|"      );
  vapi_text (        1, height - 2, corner, "|"      );
  vapi_text (        1, height - 1, corner, "BL---->");

  vapi_text (    width, height - 4, corner,       "^");
  vapi_text (    width, height - 3, corner,       "|");
  vapi_text (    width, height - 2, corner,       "|");
  vapi_text (width - 6, height - 1, corner, "<----BR");

  vapi_refresh ();
  sleep (4);

  for (int i = 0; i < 500; ++i)
  {
    sprintf (c, "on color%d", rand () % 256);
    vapi_rectangle ((rand () % (width - 8)) + 1, (rand () % (height - 4)) + 1, 8, 4, Color (c));
    vapi_refresh ();
//    usleep (5000);
  }

  sleep (2);
  vapi_deinitialize ();
  return 0;
}

