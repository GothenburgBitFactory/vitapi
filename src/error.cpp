////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2009-2011, Göteborg Bit Factory.
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

#include <string>
#include <string.h>
#include <vitapi.h>

static std::string error;

////////////////////////////////////////////////////////////////////////////////
void vitapi_set_error (const std::string& e)
{
  error = e;
}

////////////////////////////////////////////////////////////////////////////////
// Return most recent error.
extern "C" int vitapi_error (char* buf, size_t size)
{
  if (!buf)
  {
    vitapi_set_error ("Null buffer pointer passed to vitapi_error.");
    return -1;
  }

  if (error.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to vitapi_error.");
    return -1;
  }

  strncpy (buf, error.c_str (), size);

  // The act of reading the error resets it.
  error = "";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
