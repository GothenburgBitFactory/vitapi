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

#include <map>
#include <string>
#include <sstream>
#include <vitapi.h>

static std::string current_term;
static std::map <std::string, std::string> data;

static std::string lookup (const std::string&);
static std::string decode (const std::string&);
static std::string encode (const std::string&, int, int);
static std::string encode (const std::string&, const std::string&);
static std::string substitute (const std::string&, const std::string&, const std::string&);
static std::string substitute (const std::string&, const std::string&, int);

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
//   ti:               full screen on
//   te:               full screen off
//   hs:               has status line
//   cl:               clear screen
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
extern "C" int tapi_initialize (const char* term)
{
  current_term = strcmp (term, "") ? term : "xterm-256color";

  //data["vt100"] = "";       // TODO Missing def
  //data["vt220"] = "";       // TODO Missing def
  //data["xterm"] = "";       // TODO Missing def
  //data["xterm-color"] = ""; // TODO Missing def

  data["xterm-256color"] =
    "ku:_E_OA "             // ku
    "kd:_E_OB "             // kd
    "kr:_E_OC "             // kr
    "kl:_E_OD "             // kl
    "k1:_E_OP "             // k1
    "k2:_E_OQ "             // k2
    "k3:_E_OR "             // k3
    "k4:_E_OS "             // k4
    "k5:_E_[15~ "           // k5
    "k6:_E_[17~ "           // k6
    "k7:_E_[18~ "           // k7
    "k8:_E_[19~ "           // k8
    "k9:_E_[20~ "           // k9
    "k0: "                  // k0
    "kH: "                  // kH
    "kb:\010 "              // kb
    "kD:_E_[3~ "            // kD
    "kP:_E_[5~ "            // kP
    "kN:_E_[6~ "            // kN
    "AM:_E_[?1h "
    "NM:_E_[?1l "
    "Ms1:_E_[?1000h "
    "Ms0:_E_[?1000l "
    "Mt1:_E_[?1002h "
    "Mt0:_E_[?1002l "
    "ti:_E_[?1049h "        // ti
    "te:_E_[?1049l "        // te
    "hs:1 "                 // hs
    "cl:_E_[_E_[2J "        // cl
    "Mv:_E_[_y_;_x_H "
    "Alt:_E_[1049h "
    "Ttl:_E_]2;_s__B_";

  // Error if term is not supported.
  if (data.find (term) == data.end ())
    return -1;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// This is currently the only way to provide additional terminal types.
extern "C" void tapi_add (const char* term, const char* def)
{
  data[term] = def;
}

////////////////////////////////////////////////////////////////////////////////
// Simply returns the control string - may or may not be ready to use.
extern "C" void tapi_get (const char* key, char* value)
{
  std::string s = decode (lookup (key));
  strcpy (value, s.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with [x,y] coordinate substitution, ready to use.
extern "C" void tapi_get_xy (const char* key, char* value, int x, int y)
{
  std::string s = encode (decode (lookup (key)), x, y);
  strcpy (value, s.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with string substitution, ready to use.
extern "C" void tapi_get_str (const char* key, char* value, const char* str)
{
  std::string s = encode (decode (lookup (key)), str);
  strcpy (value, s.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Finds current_term in data, then locates the key within the definition.
static std::string lookup (const std::string& key)
{
  std::string output;
  std::map <std::string, std::string>::iterator t = data.find (current_term);
  if (t != data.end ())
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
// Converts "..._E_..." -> "...\033...".
// TODO Support more than 2 _E_ substitutions.
static std::string decode (const std::string& input)
{
  return substitute (
           substitute (
             substitute (input, "_E_", "\033"),
             "_E_",
             "\033"),
           "_B_",
           "\007");
}

////////////////////////////////////////////////////////////////////////////////
// Converts "..._x_..._y_..." -> "...x...y...".
static std::string encode (const std::string& input, int x, int y)
{
  return substitute (
           substitute (input, "_x_", x),
           "_y_",
           y);
}

////////////////////////////////////////////////////////////////////////////////
// Converts "..._s_..." -> "...value...".
static std::string encode (const std::string& input, const std::string& value)
{
  return substitute (input, "_s_", value);
}

////////////////////////////////////////////////////////////////////////////////
// Converts "...FROM..." -> "...TO...".
static std::string substitute (
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
// Converts "...FROM..." -> "...TO...".
static std::string substitute (
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
