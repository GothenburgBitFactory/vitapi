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
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "vitapi.h"

void redraw (int, int, int, int);
void rulers (int, int);
void instructions (int, int);
void mouse_pos (int, int, int, int);
void term_size (int, int);

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  if (! vapi_initialize ())
  {
    vapi_full_screen ();

    iapi_initialize ();
    iapi_raw ();
    iapi_noecho ();
    iapi_mouse_tracking ();

    int width = vapi_width ();
    int height = vapi_height ();
    int mx = 0;
    int my = 0;

    redraw (width, height, mx, my);

    int key;
    while ((key = iapi_getch ()) != EOF)
    {
      if (key == 'q')
        break;

      else if (key == 12)
      {
        width = vapi_width ();
        height = vapi_height ();
        redraw (width, height, mx, my);
      }

      else if (key == IAPI_MOUSE_1_CLICK ||
               key == IAPI_MOUSE_2_CLICK ||
               key == IAPI_MOUSE_3_CLICK ||
               key == IAPI_MOUSE_4_CLICK ||
               key == IAPI_MOUSE_5_CLICK ||
               key == IAPI_MOUSE_1_MOVE  ||
               key == IAPI_MOUSE_2_MOVE  ||
               key == IAPI_MOUSE_3_MOVE  ||
               key == IAPI_MOUSE_4_MOVE  ||
               key == IAPI_MOUSE_5_MOVE  ||
               key == IAPI_MOUSE_RELEASE)
      {
        iapi_mouse_pos (&mx, &my);
        mouse_pos (width, height, mx, my);
        vapi_moveto (1, 1);
        vapi_refresh ();
      }
    }

    iapi_nomouse_tracking ();
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

////////////////////////////////////////////////////////////////////////////////
void redraw (int w, int h, int mx, int my)
{
  vapi_clear ();

  rulers (w, h);
  instructions (w, h);
  mouse_pos (w, h, mx, my);
  term_size (w, h);

  vapi_moveto (1, 1);
  vapi_refresh ();
}

////////////////////////////////////////////////////////////////////////////////
void rulers (int w, int h)
{
  color red = color_def ("red");

  std::stringstream line1;
  std::stringstream line2;
  std::stringstream line3;
  std::stringstream line4;

  for (int i = 1; i <= w; i++)
  {
    if ((i % 10 == 0) && i > 90)
    {
      line1 << (i / 100);
      line2 << (i % 100) / 10;
      line4 << '|';
    }
    else if (i % 10 == 0)
    {
      line1 << ' ';
      line2 << (i / 10);
      line4 << '|';
    }
    else if (i % 5 == 0)
    {
      line1 << ' ';
      line2 << ' ';
      line4 << ':';
    }
    else
    {
      line1 << ' ';
      line2 << ' ';
      line4 << '.';
    }

    line3 << (i % 10);
  }

  vapi_pos_color_text (1, 1, red, line1.str ().c_str ());
  vapi_pos_color_text (1, 2, red, line2.str ().c_str ());
  vapi_pos_color_text (1, 3, red, line3.str ().c_str ());
  vapi_pos_color_text (1, 4, red, line4.str ().c_str ());
}

////////////////////////////////////////////////////////////////////////////////
void instructions (int w, int h)
{
  vapi_rectangle (1, h - 1, w, 2, color_def ("on red"));
  vapi_pos_color_text ((w - 36) / 2,
                       h - 1,
                       color_def ("bold white on red"),
                       "Press 'q' to quit, Ctrl-L to redraw.");
}

////////////////////////////////////////////////////////////////////////////////
void term_size (int w, int h)
{
  char msg[48];
  sprintf (msg, "Term %dx%d", w, h);

  color c = color_def ("black on green");
  vapi_rectangle (w - 16, 6, 17, 3, c);
  vapi_pos_color_text (w - 15, 7, c, msg);
}

////////////////////////////////////////////////////////////////////////////////
void mouse_pos (int w, int h, int mx, int my)
{
  char msg[48];
  sprintf (msg, "Mouse (%d,%d)", mx, my);

  color c = color_def ("white on blue");
  vapi_rectangle (w - 16, 10, 17, 3, c);
  vapi_pos_color_text (w - 15, 11, c, msg);
}

////////////////////////////////////////////////////////////////////////////////
