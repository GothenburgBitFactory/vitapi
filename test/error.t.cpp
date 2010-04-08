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
  UnitTest t (6);

  char error [256];

  t.is (color_def ("donkey on red"),   -1, "donkey on red   -> -1");
  color_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "error           -> The color 'donkey' is not recognized.");

  t.is (color_def ("red on toast"),    -1, "red on toast    -> -1");
  color_error (error, 256);
  t.is (error, "The color 'toast' is not recognized.",
               "error           -> The color 'toast' is not recognized.");

  t.is (color_def ("donkey on toast"), -1, "donkey on toast -> -1");
  color_error (error, 256);
  t.is (error, "The color 'donkey' is not recognized.",
               "error           -> The color 'donkey' is not recognized.");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
