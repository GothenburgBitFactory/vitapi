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
#include <sstream>
#include "Caps.h"

////////////////////////////////////////////////////////////////////////////////
Caps::Caps ()
{
}

////////////////////////////////////////////////////////////////////////////////
Caps::~Caps ()
{
}

////////////////////////////////////////////////////////////////////////////////
// Input
//   ku, kd, kr, kl:   up, down, right, left
//   k1 .. k9, k0:     F1 .. F9, F10
//   kH:               home
//   kb:               backspace
//   kD:               delete
//   kP:               page up
//   kN:               page down
//
// Output
//   AM:               application mode
//   NM:               normal mode
//   Ms1:              mouse on
//   Ms0:              mouse off
//   Mt1:              mouse tracking on
//   Mt0:              mouse tracking off
//   FS1:              full screen on
//   FS0:              full screen off
//   St:               has status line
//   Mv:               move to
//   Alt:              alternate screen buffer
//   Ttl:              window title
//
// Encoding
//   _E_               <Escape>
//   _x_               column
//   _y_               row
//   _s_               string
//   _B_               <Bell>
bool Caps::initialize (const std::string& term /* = "" */)
{
  _term = term;

  _data["xterm"]          = "";
  _data["xterm-color"]    = "";
  _data["xterm-256color"] = "ku:_E_[OA kd:_E_[OB kr:_E_[OC kl:_E_[OD "
                            "k1: k2: k3: k4: k5: k6: k7: k8: k9: k0: "
                            "kH: kb: kD: kP: kN: AM:_E_[?1h NM:_E_[1l "
                            "Ms1: Ms0: Mt1: Mt0: "
                            "FS1: FS0: St: Mv: Alt: Ttl:";

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// This is currently the only way to provide additional terminal types.
void Caps::add (const std::string& term, const std::string& def)
{
  _data[term] = def;
}

////////////////////////////////////////////////////////////////////////////////
// Simply returns the cap string - may or may not be ready to use.
std::string Caps::get (const std::string& key)
{
  return decode (lookup (key));
}

////////////////////////////////////////////////////////////////////////////////
// Returns the cap string with [x,y] coordinate substitution, ready to use.
std::string Caps::get (const std::string& key, int x, int y)
{
  return encode (decode (lookup (key)), x, y);
}

////////////////////////////////////////////////////////////////////////////////
// Returns the cap string with string substitution, ready to use.
std::string Caps::get (const std::string& key, const std::string& value)
{
  return encode (decode (lookup (key)), value);
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::lookup (const std::string& key)
{
  std::string output;
  std::map <std::string, std::string>::iterator t = _data.find (_term);
  if (t != _data.end ())
  {
    std::string::size_type k = t->second.find (key);
    if (k != std::string::npos)
    {
      std::string::size_type space = t->second.find (' ', k);
      if (space != std::string::npos)
        output = t->second.substr (k + key.length () + 1, space - k - key.length () - 1);
      else
        output = t->second.substr (k + key.length () + 1);
    }
  }

  return output;
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::decode (const std::string& input)
{
  return substitute (
           substitute (input, "_E_", "\033"),
           "_B_",
           "\007");
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::encode (const std::string& input, int x, int y)
{
  return substitute (
           substitute (input, "_x_", x),
           "_y_",
           y);
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::encode (const std::string& input, const std::string& value)
{
  return substitute (input, "_s_", value);
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::substitute (
  const std::string& input,
  const std::string& from,
  const std::string& to)
{
  std::string::size_type it;
  if ((it = input.find (from)) != std::string::npos)
    return input.substr (0, it) + to + input.substr (it + from.length ());

  return input;
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::substitute (
  const std::string& input,
  const std::string& from,
  int to)
{
  std::string::size_type it;
  if ((it = input.find (from)) != std::string::npos)
  {
    std::stringstream temp;
    temp << input.substr (0, it) << to << input.substr (it + from.length ());
    return temp.str ();
  }

  return input;
}

////////////////////////////////////////////////////////////////////////////////
