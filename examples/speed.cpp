////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2014, Göteborg Bit Factory.
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

