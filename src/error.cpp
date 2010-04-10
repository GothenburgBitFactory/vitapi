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
