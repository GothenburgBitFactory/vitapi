////////////////////////////////////////////////////////////////////////////////
// task - a command line task list manager.
//
// Copyright 2006 - 2010, Paul Beckingham.
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
void Caps::add (const std::string& term, const std::string& def)
{
  _data[term] = def;
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::get (const std::string& key)
{
  // TODO Lookup term
  // TODO Default is xterm
  // TODO Decode
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::get (const std::string& key, int x, int y)
{
  // TODO Lookup term
  // TODO Default is xterm
  // TODO Decode _E_, _B_
  // TODO Encode _x_, _y_
}

////////////////////////////////////////////////////////////////////////////////
std::string Caps::get (const std::string& key, const std::string& value)
{
  // TODO Lookup term
  // TODO Default is xterm
  // TODO Decode _E_, _B_
  // TODO Encode _s_
}

////////////////////////////////////////////////////////////////////////////////
