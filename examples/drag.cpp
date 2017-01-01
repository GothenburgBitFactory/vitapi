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
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vitapi.h>

#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)

int main (int argc, char** argv)
{
  // VAPI up.
  if (! vapi_initialize ())
  {
    vapi_full_screen ();
    vapi_clear ();

    // IAPI up.
    iapi_initialize ();
    iapi_raw ();
    iapi_noecho ();
    iapi_mouse_tracking ();

    // Draw a rectangle.
    color c = color_def ("black on cyan");
    int r_x = 1;
    int r_y = 1;
    int r_width = 10;
    int r_height = 3;
    vapi_rectangle (r_x, r_y, r_width, r_height, c);
    vapi_pos_color_text (r_x + 1, r_y + 1, c, "Drag me.");

    int width  = vapi_width ();
    int height = vapi_height ();

    vapi_pos_text ((width - 17) / 2, height / 2, "Press 'q' to quit");
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
        iapi_mouse_pos (&x, &y);

        if (dragging)
        {
          r_x = MAX (1, MIN (x, width  - r_width + 1));
          r_y = MAX (1, MIN (y, height - r_height));

          vapi_clear ();
          vapi_pos_text ((width - 17) / 2, height / 2, "Press 'q' to quit");
          vapi_rectangle (r_x, r_y, r_width, r_height, c);
          vapi_pos_color_text (r_x + 1, r_y + 1, c, "Drag me.");
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
  }
  else
    std::cout << "Your $TERM ("
              << getenv ("TERM")
              << ") is not supported."
              << std::endl;

  return 0;
}

