////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2009 - 2010, Paul Beckingham, Federico Hernandez.
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

