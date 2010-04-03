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
#ifndef INCLUDED_CAPS
#define INCLUDED_CAPS

#include <string>
#include <map>

class Caps
{
public:
  Caps ();
  ~Caps ();
  Caps (const Caps&);            // Unimplemented.
  Caps& operator= (const Caps&); // Unimplemented.

  bool initialize (const std::string& term = "");

  void add (const std::string&, const std::string&);

  std::string get (const std::string&);
  std::string get (const std::string&, int, int);
  std::string get (const std::string&, const std::string&);

private:
  std::map <std::string, std::string> _data;
  std::string _term;
};

#endif
////////////////////////////////////////////////////////////////////////////////
