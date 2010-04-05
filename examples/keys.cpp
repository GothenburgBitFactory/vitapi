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
#include <ctype.h>

#ifdef SOLARIS
#define CBREAK O_CBREAK
#include <sys/ttold.h>
#endif

#include <vitapi.h>

int main (int argc, char** argv)
{
  iapi_initialize ();
  iapi_raw ();
  iapi_noecho ();
  iapi_mouse ();

  std::cout << "Press keys to see how IAPI reads and maps them."
            << std::endl
            << "Press 'q' to quit."
            << std::endl;

  int key;
  int x, y;  // mouse
  while ((key = iapi_getch ()) != EOF)
  {
    if (key == 'q')
      break;

    iapi_mouse_pos (&x, &y); // Get mouse position, just in case.

    switch (key)
    {
    case   9:                std::cout << "<Tab>"       << std::endl; break;
    case  10:                std::cout << "<Newline>"   << std::endl; break;
    case  27:                std::cout << "<Esc>"       << std::endl; break;
    case  32:                std::cout << "<Space>"     << std::endl; break;
    case IAPI_KEY_BACKSPACE: std::cout << "<Backspace>" << std::endl; break;
    case IAPI_KEY_LEFT:      std::cout << "<Left>"      << std::endl; break;
    case IAPI_KEY_RIGHT:     std::cout << "<Right>"     << std::endl; break;
    case IAPI_KEY_UP:        std::cout << "<Up>"        << std::endl; break;
    case IAPI_KEY_DOWN:      std::cout << "<Down>"      << std::endl; break;
    case IAPI_KEY_F1:        std::cout << "<F1>"        << std::endl; break;
    case IAPI_KEY_F2:        std::cout << "<F2>"        << std::endl; break;
    case IAPI_KEY_F3:        std::cout << "<F3>"        << std::endl; break;
    case IAPI_KEY_F4:        std::cout << "<F4>"        << std::endl; break;
    case IAPI_KEY_F5:        std::cout << "<F5>"        << std::endl; break;
    case IAPI_KEY_F6:        std::cout << "<F6>"        << std::endl; break;
    case IAPI_KEY_F7:        std::cout << "<F7>"        << std::endl; break;
    case IAPI_KEY_F8:        std::cout << "<F8>"        << std::endl; break;
    case IAPI_KEY_F9:        std::cout << "<F9>"        << std::endl; break;
    case IAPI_KEY_F10:       std::cout << "<F10>"       << std::endl; break;
    case IAPI_KEY_HOME:      std::cout << "<Home>"      << std::endl; break;
    case IAPI_KEY_DEL:       std::cout << "<Del>"       << std::endl; break;
    case IAPI_KEY_PGUP:      std::cout << "<PgUp>"      << std::endl; break;
    case IAPI_KEY_PGDN:      std::cout << "<PgDn>"      << std::endl; break;

    case IAPI_MOUSE_1_CLICK: std::cout << "<Mouse1> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_2_CLICK: std::cout << "<Mouse2> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_3_CLICK: std::cout << "<Mouse3> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_RELEASE: std::cout << "<MouseRelease> [" << x << "," << y << "]" << std::endl; break;

    default:
      if (isprint (key))
        std::cout << (char) key << " (" << key << ")" << std::endl;
      else
        std::cout << "0d" << key << std::endl;
      break;
    }
  }

  if (key == 'q') std::cout << "# quit" << std::endl;
  if (key != 'q') std::cout << "# EOF"  << std::endl;

  iapi_nomouse ();
  iapi_echo ();
  iapi_noraw ();
  iapi_deinitialize ();

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
