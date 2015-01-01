////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2015, Göteborg Bit Factory.
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

