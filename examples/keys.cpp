////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2016, Göteborg Bit Factory.
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
#include <ctype.h>

#ifdef SOLARIS
#define CBREAK O_CBREAK
#include <sys/ttold.h>
#endif

#include <vitapi.h>

int main (int argc, char** argv)
{
  vapi_initialize (); // For IAPI_RESIZE only.
  iapi_initialize ();
  iapi_raw ();
  iapi_noecho ();
  iapi_mouse_tracking ();

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

    case IAPI_RESIZE:        std::cout << "<Resize>"    << std::endl; break;

    case IAPI_MOUSE_1_CLICK: std::cout << "<Button1> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_2_CLICK: std::cout << "<Button2> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_3_CLICK: std::cout << "<Button3> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_4_CLICK: std::cout << "<Button4> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_5_CLICK: std::cout << "<Button5> [" << x << "," << y << "]" << std::endl; break;

    case IAPI_MOUSE_1_MOVE:  std::cout << "<Move1> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_2_MOVE:  std::cout << "<Move2> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_3_MOVE:  std::cout << "<Move3> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_4_MOVE:  std::cout << "<Move4> [" << x << "," << y << "]" << std::endl; break;
    case IAPI_MOUSE_5_MOVE:  std::cout << "<Move5> [" << x << "," << y << "]" << std::endl; break;

    case IAPI_MOUSE_RELEASE: std::cout << "<Release> [" << x << "," << y << "]" << std::endl; break;

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

  iapi_nomouse_tracking ();
  iapi_echo ();
  iapi_noraw ();
  iapi_deinitialize ();
  vapi_deinitialize ();

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
