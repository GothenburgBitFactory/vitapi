////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2006 - 2010, Paul Beckingham, Federico Hernandez.
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
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <sys/time.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <string.h>
//#include <pwd.h>
#include "util.h"

////////////////////////////////////////////////////////////////////////////////
int autoComplete (
  const std::string& partial,
  const std::vector<std::string>& list,
  std::vector<std::string>& matches)
{
  matches.clear ();

  // Handle trivial case. 
  unsigned int length = partial.length ();
  if (length)
  {
    foreach (item, list)
    {
      // An exact match is a special case.  Assume there is only one exact match
      // and return immediately.
      if (partial == *item)
      {
        matches.clear ();
        matches.push_back (*item);
        return 1;
      }

      // Maintain a list of partial matches.
      if (length <= item->length () &&
          partial == item->substr (0, length))
        matches.push_back (*item);
    }
  }

  return matches.size ();
}

////////////////////////////////////////////////////////////////////////////////
