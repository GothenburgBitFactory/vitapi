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
