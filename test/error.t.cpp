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
#include <test.h>
#include <vitapi.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (18);

  char error [256];

  // vitapi_error
  vitapi_error (NULL, 0);    // Cause an error
  vitapi_error (error, 256); // Get the message
  t.is (error, "Null buffer pointer passed to vitapi_error.",      "vitapi_error NULL -> Null buffer pointer passed to vitapi_error.");

  vitapi_error (NULL, 0);    // Cause an error
  vitapi_error (error, 1);   // Cause another
  vitapi_error (error, 256); // Get the message
  t.is (error, "Insufficient buffer size passed to vitapi_error.", "vitapi_error 0    -> Insufficient buffer size passed to vitapi_error.");

  // color_def
  t.is (color_def ("donkey on red"),   -1, "donkey on red     -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "error             -> The color 'donkey' is not recognized.");

  t.is (color_def ("red on toast"),    -1, "red on toast      -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'toast' is not recognized.",
               "error             -> The color 'toast' is not recognized.");

  t.is (color_def ("donkey on toast"), -1, "donkey on toast   -> -1");
  vitapi_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "error             -> The color 'donkey' is not recognized.");

  t.is (color_def (NULL),              -1, "NULL              -> -1");
  vitapi_error (error, 256);
  t.is (error, "Null pointer to a color definition passed to color_def.",
               "error            -> Null pointer to a color definition passed to color_def.");

  // color_name
  color_name (NULL, 1, 0);
  vitapi_error (error, 256);
  t.is (error, "Null buffer pointer passed to color_name.",
               "error            -> Null buffer pointer passed to color_name.");

  color_name (error, 1, -1);
  vitapi_error (error, 256);
  t.is (error, "Invalid color passed to color_name.",
               "error            -> Invalid color passed to color_name.");

  // color_downgrade
  t.is (color_downgrade (0),           -1, "color_downgrade  -> -1");
  vitapi_error (error, 256);
  t.is (error, "color_downgrade is not implemented.",
               "error            -> color_downgrade is not implemented.");

  // color_blend
  t.is (color_blend (-1, -1),          -1, "blend -1, -1     -> error");
  vitapi_error (error, 256);
  t.is (error, "Two invalid colors passed to color_blend.",
               "error            -> Two invalid colors passed to color_blend.");

  // color_colorize
  color_colorize (NULL, 1, 0);
  vitapi_error (error, 256);
  t.is (error, "Null buffer pointer passed to color_colorize.",
               "error            -> Null buffer pointer passed to color_colorize.");

  color_colorize (error, 1, color_def ("red"));
  vitapi_error (error, 256);
  t.is (error, "Insufficient buffer size passed to color_colorize.",
               "error            -> Null buffer pointer passed to color_colorize.");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
