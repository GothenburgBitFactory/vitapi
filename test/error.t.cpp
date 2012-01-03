////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010-2012, Göteborg Bit Factory.
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
#include <test.h>
#include <vitapi.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (46);

  char error [256];

  // vitapi_error
  vitapi_error (NULL, 0);    // Cause an error
  vitapi_error (error, 256); // Get the message
  t.is (error, "Null buffer pointer passed to vitapi_error.",
               "vitapi_error: NULL pointer");

  vitapi_error (NULL, 0);    // Cause an error
  vitapi_error (error, 1);   // Cause another
  vitapi_error (error, 256); // Get the new message
  t.is (error, "Insufficient buffer size passed to vitapi_error.",
               "vitapi_error: Insufficient buffer");

  // color_def
  t.is (color_def ("donkey on red"), -1, "donkey on red -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "color_def: 'donkey' invalid");

  t.is (color_def ("red on toast"), -1, "red on toast -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'toast' is not recognized.",
               "color_def: 'toast' invalid");

  t.is (color_def ("donkey on toast"), -1, "donkey on toast -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "color_def: 'donkey' invalid");

  t.is (color_def (NULL), -1, "NULL -> -1");
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a color definition passed to color_def.",
               "color_def: NULL pointer");

  // color_name
  color_name (NULL, 1, 0);
  vitapi_error (error, 256);
  t.is (error, "Null buffer pointer passed to color_name.",
               "color_name: NULL pointer");

  color_name (error, 1, -1);
  vitapi_error (error, 256);
  t.is (error, "Invalid color passed to color_name.",
               "color_name: invalid color");

  // color_blend
  t.is (color_blend (-1, -1), -1, "blend -1, -1 -> error");
  vitapi_error (error, 256);
  t.is (error, "Two invalid colors passed to color_blend.",
               "color_blend: two invalid colors");

  t.is (color_blend (color_def ("red"), -1), color_def ("red"), "blend red, -1 -> red");
  t.is (color_blend (-1, color_def ("red")), color_def ("red"), "blend -1, red, -> red");

  // color_colorize
  color_colorize (NULL, 1, 0);
  vitapi_error (error, 256);
  t.is (error, "Null buffer pointer passed to color_colorize.",
               "color_colorize: NULL pointer");

  color_colorize (error, 1, color_def ("red"));
  vitapi_error (error, 256);
  t.is (error, "Insufficient buffer size passed to color_colorize.",
               "color_colorize: insufficient buffer");

  // iapi_mouse_pos
  int x, y;
  iapi_mouse_pos (&x, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to y coordinate passed to iapi_mouse_pos",
               "iapi_mouse_pos: NULL pointer");

  iapi_mouse_pos (NULL, &y);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to x coordinate passed to iapi_mouse_pos",
               "iapi_mouse_pos: NULL pointer");

  iapi_mouse_pos (NULL, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to x coordinate passed to iapi_mouse_pos",
               "iapi_mouse_pos: NULL pointer");

  // vapi_moveto
  vapi_moveto (0, 1);
  vitapi_error (error, 256);
  t.is (error, "Invalid x coordinate passed to vapi_moveto.",
               "vapi_moveto: x coordinate range");

  vapi_moveto (1, 0);
  vitapi_error (error, 256);
  t.is (error, "Invalid y coordinate passed to vapi_moveto.",
               "vapi_moveto: y coordinate range");

  vapi_moveto (10000, 1);
  vitapi_error (error, 256);
  t.is (error, "Invalid x coordinate passed to vapi_moveto.",
               "vapi_moveto: x coordinate range");

  vapi_moveto (1, 10000);
  vitapi_error (error, 256);
  t.is (error, "Invalid y coordinate passed to vapi_moveto.",
               "vapi_moveto: y coordinate range");

  // vapi_text
  vapi_text (NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer passed to vapi_text.",
               "vapi_text: NULL pointer");

  // vapi_color_text
  vapi_color_text (0, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer passed to vapi_color_text.",
               "vapi_color_text: NULL pointer");

  vapi_color_text (-1, "foo");
  vitapi_error (error, 256);
  t.is (error, "Invalid color passed to vapi_color_text.",
               "vapi_color_text: Invalid color");

  // vapi_pos_text
  vapi_pos_text (1, 1, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer passed to vapi_pos_text.",
               "vapi_pos_text: NULL pointer");

  // vapi_pos_color_text
  vapi_pos_color_text (1, 1, 0, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer passed to vapi_pos_color_text.",
               "vapi_pos_color_text: NULL pointer");

  vapi_pos_color_text (1, 1, -1, "foo");
  vitapi_error (error, 256);
  t.is (error, "Invalid color passed to vapi_pos_color_text.",
               "vapi_pos_color_text: Invalid color");

  // vapi_rectangle
  vapi_rectangle (1, 1, 10, 10, -1);
  vitapi_error (error, 256);
  t.is (error, "Invalid color passed to vapi_rectangle.",
               "vapi_rectangle: Invalid color");

  // vapi_title
  vapi_title (NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer passed to vapi_title.",
               "vapi_title: NULL pointer");

  // tapi_initialize
  tapi_initialize (NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal type passed to tapi_initialize.",
               "tapi_initialize: NULL pointer");

  // tapi_add
  tapi_add ("vt100", NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal definition passed to tapi_add.",
               "tapi_add: NULL pointer");

  tapi_add (NULL, "foo:bar");
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal type passed to tapi_add.",
               "tapi_add: NULL pointer");

  // tapi_get
  tapi_initialize ("");
  char buf[64];
  tapi_get (NULL, buf, 64);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal key passed to tapi_get.",
               "tapi_get: NULL pointer");

  tapi_get ("ku", NULL, 64);
  vitapi_error (error, 256);
  t.is (error, "Null pointer for a key value passed to tapi_get.",
               "tapi_get: NULL pointer");

  tapi_get ("ku", buf, 1);
  vitapi_error (error, 256);
  t.is (error, "Insufficient buffer size passed to tapi_get.",
               "tapi_get: Insufficient buffer");

  // tapi_get_xy
  tapi_get_xy (NULL, buf, 64, 1, 1);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal key passed to tapi_get_xy.",
               "tapi_get_xy: NULL pointer");

  tapi_get_xy ("ku", NULL, 64, 1, 1);
  vitapi_error (error, 256);
  t.is (error, "Null pointer for a key value passed to tapi_get_xy.",
               "tapi_get_xy: NULL pointer");

  tapi_get_xy ("ku", buf, 1, 1, 1);
  vitapi_error (error, 256);
  t.is (error, "Insufficient buffer size passed to tapi_get_xy.",
               "tapi_get_xy: Insufficient buffer");

  // tapi_get_str
  tapi_get_str (NULL, buf, 64, buf);
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a terminal key passed to tapi_get_str.",
               "tapi_get_str: NULL pointer");

  tapi_get_str ("Ttl", NULL, 64, buf);
  vitapi_error (error, 256);
  t.is (error, "Null pointer for a key value passed to tapi_get_str.",
               "tapi_get_str: NULL pointer");

  tapi_get_str ("Ttl", buf, 1, buf);
  vitapi_error (error, 256);
  t.is (error, "Insufficient buffer size passed to tapi_get_str.",
               "tapi_get_str: Insufficient buffer");

  tapi_get_str ("Ttl", buf, 64, NULL);
  vitapi_error (error, 256);
  t.is (error, "Null pointer for a substitution passed to tapi_get_str.",
               "tapi_get_str: NULL pointer");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
