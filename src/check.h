////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2009-2010, Göteborg Bit Factory.
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

#ifndef INCLUDED_CHECK
#define INCLUDED_CHECK

void vitapi_set_error (const std::string&);

#define CHECK0(arg,msg)  if (!(arg))                    {vitapi_set_error (msg); return;}
#define CHECK1(arg,msg)  if (!(arg))                    {vitapi_set_error (msg); return -1;}
#define CHECKC0(arg,msg) if ((arg) == -1)               {vitapi_set_error (msg); return;}
#define CHECKC1(arg,msg) if ((arg) == -1)               {vitapi_set_error (msg); return -1;}
#define CHECKX0(x,msg)   if ((x)<1 || (x)>screenWidth)  {vitapi_set_error (msg); return;}
#define CHECKY0(y,msg)   if ((y)<1 || (y)>screenHeight) {vitapi_set_error (msg); return;}
#define CHECKW0(w,msg)   if ((w)<1)                     {vitapi_set_error (msg); return;}

#endif
////////////////////////////////////////////////////////////////////////////////

