////////////////////////////////////////////////////////////////////////////////
// Tegelsten - building blocks for UI
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
#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

#include <string>
#include <vector>
#include <map>
#include <sys/types.h>

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#define foreach(i, c)                                              \
for (typeof (c) *foreach_p = & (c);                                \
     foreach_p;                                                    \
     foreach_p = 0)                                                \
  for (typeof (foreach_p->begin()) i = foreach_p->begin();         \
       i != foreach_p->end();                                      \
       ++i)

// util.cpp
int autoComplete (const std::string&, const std::vector<std::string>&, std::vector<std::string>&);

#endif
////////////////////////////////////////////////////////////////////////////////
