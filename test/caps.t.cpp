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
#include <Caps.h>
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (4);

  Caps c;
  c.initialize ("foo");
  c.add ("foo", "a:_E_ b:_E__s__B_ c:_x_,_y_ d:bunny");

  t.is (c.get ("a"),        "\033",        "_E_       -> \\033");
  t.is (c.get ("b", "dog"), "\033dog\007", "_E__s__B_ -> \\033dog\\007");
  t.is (c.get ("c", 1, 2),  "1,2",         "_x_,_y_   -> 1,2");
  t.is (c.get ("d"),        "bunny",       "bunny     -> bunny");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
