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
#include <vitapi.h>
#include <test.h>

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (7);

  t.is (tapi_initialize ("xterm-256color"),  0, "tapi_initialize xterm-256color good");
  t.is (tapi_initialize ("foo"),            -1, "tapi_initialize foo bad");

  tapi_add ("foo", "a:_E_ b:_E__s__B_ c:_x_,_y_ d:bunny e:_E__E__E__E__E__E__E__E_");

  char value[64];
  t.is (tapi_get ("a", value, 64),
        "\033",
        "_E_ -> \\033");

  t.is (tapi_get_str ("b", value, 64, "dog"),
        "\033dog\007",
        "_E__s__B_ -> \\033dog\\007");

  t.is (tapi_get_xy ("c", value, 64, 1, 2),
        "1,2",
        "_x_,_y_ -> 1,2");

  t.is (tapi_get ("d", value, 64),
        "bunny",
        "bunny -> bunny");

  t.is (tapi_get ("e", value, 64),
        "\033\033\033\033\033\033\033\033",
        "_E__E__E__E__E__E__E__E_ -> \\033\\033\\033\\033\\033\\033\\033\\033");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
