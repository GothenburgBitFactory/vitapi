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
#include <string.h>
#include <vitapi.h>
#include <check.h>

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
  CHECK1 (term, "Null pointer to a terminal type passed to tapi_initialize.");

  // Default value is 'xterm-256color'.
  current_term = strcmp (term, "") ? term : "xterm-256color";

  // Settings that are common to all terminals.
  std::string app_mode    = "AM:_E_[?1h ";
  std::string normal_mode = "NM:_E_[?1l ";
  std::string mouse       = "Ms1:_E_[?1000h Ms0:_E_[?1000l Mt1:_E_[?1002h Mt0:_E_[?1002l ";
  std::string move        = "Mv:_E_[_y_;_x_H ";
  std::string alternate   = "Alt:_E_[1049h ";
  std::string title       = "Ttl:_E_]2;_s__B_";   // No trailing space, so last.

  std::string common = app_mode + normal_mode + mouse + move + alternate + title;

  // Now the terminal-specific settings.  The following is a list of
  // Lucid-supported terminal programs, which is a good starting point for the
  // terminals that we might want to support:
  //
  // TODO aterm          - Afterstep XVT - a VT102 emulator for the X
  // TODO aterm-ml       - Afterstep XVT - a VT102 emulator for the X
  // TODO eterm          - Enlightened Terminal Emulator
  // TODO fbiterm        - framebuffer internationalized terminal emu
  // TODO fbterm         - A fast framebuffer based terminal emulator
  // TODO gnome-terminal - The GNOME terminal emulator application
  // TODO gtkterm        - A simple GTK+ serial port terminal
  // TODO jfbterm        - multilingual terminal on Linux framebuffer
  // TODO kterm          - Multi-lingual terminal emulator for X
  // TODO lxterminal     - desktop independent vte-based terminal emu
  // TODO mlterm         - MultiLingual TERMinal
  // TODO mrxvt          - lightweight multi-tabbed X terminal emulator
  // TODO multi-aterm    - tabbed terminal emulator with efficent pse
  // TODO pterm          - PuTTY terminal emulator
  // TODO roxterm        - Multi-tabbed GTK/VTE terminal emulator
  // TODO rxvt           - VT102 terminal emulator for the X Window System
  // TODO rxvt-unicode   - RXVT-like terminal emulator with Unicode support
  // TODO terminal.app   - Terminal Emulator for GNUstep
  // TODO terminator     - multiple GNOME terminals in one window
  // TODO wterm          - lightweight terminal emulator for X
  // TODO wterm-ml       - lightweight multilingual terminal emulator
  // TODO xfce4-terminal - Xfce terminal emulator
  // TODO xiterm         - internationalized terminal emulator for X
  //
  // Others:
  //
  // TODO vt100
  // TODO vt102
  // TODO vt220
  // TODO xterm-color

  data["xterm"] = data["xterm-256color"] =
    "ku:_E_OA "
    "kd:_E_OB "
    "kr:_E_OC "
    "kl:_E_OD "
    "k1:_E_OP "
    "k2:_E_OQ "
    "k3:_E_OR "
    "k4:_E_OS "
    "k5:_E_[15~ "
    "k6:_E_[17~ "
    "k7:_E_[18~ "
    "k8:_E_[19~ "
    "k9:_E_[20~ "
    "k0: "
    "kH: "
    "kb:\010 "
    "kD:_E_[3~ "
    "kP:_E_[5~ "
    "kN:_E_[6~ "
    "ti:_E_[?1049h "
    "te:_E_[?1049l "
    "hs:1 "
    "cl:_E_[_E_[2J "
    + common;

  data["rxvt"] = data["rxvt-unicode"] =
    "ku:_E_OA "
    "kd:_E_OB "
    "kr:_E_OC "
    "kl:_E_OD "
    "k1:_E_[11~ "
    "k2:_E_[12~ "
    "k3:_E_[13~ "
    "k4:_E_[14~ "
    "k5:_E_[15~ "
    "k6:_E_[17~ "
    "k7:_E_[18~ "
    "k8:_E_[19~ "
    "k9:_E_[20~ "
    "kb:127 "
    "kD:_E_[3~ "
    "kP:_E_[5~ "
    "kN:_E_[6~ "
    "ti:_E_[?1049h "
    "te:_E_[r_E_[?1049l "
    "cl:_E_[H_E_[2J "
    + common;

  data["cygwin"] =
    "ku:_E_[A "
    "kd:_E_[B "
    "kr:_E_[C "
    "kl:_E_[D "
    "k1:_E_[[A "
    "k2:_E_[[B "
    "k3:_E_[[C "
    "k4:_E_[[D "
    "k5:_E_[[E "
    "k6:_E_[17~ "
    "k7:_E_[18~ "
    "k8:_E_[19~ "
    "k9:_E_[20~ "
    "kb:8 "
    "kD:_E_[3~ "
    "kP:_E_[5~ "
    "kN:_E_[6~ "
    "ti:_E_7_E_[?47h "
    "te:_E_[2J_E_[?47l_E_8 "
    "cl:_E_[H_E_[J "
    + common;

  // Error if term is not supported.
  if (data.find (term) == data.end ())
  {
    vitapi_set_error (std::string ("Terminal type '") + term + "' is not supported.");
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// This is currently the only way to provide additional terminal types.
extern "C" void tapi_add (const char* term, const char* def)
{
  CHECK0 (term, "Null pointer to a terminal type passed to tapi_add.");
  CHECK0 (def,  "Null pointer to a terminal definition passed to tapi_add.");

  data[term] = def;
}

////////////////////////////////////////////////////////////////////////////////
// Simply returns the control string - may or may not be ready to use.
extern "C" void tapi_get (const char* key, char* value, size_t size)
{
  CHECK0 (key,   "Null pointer to a terminal key passed to tapi_get.");
  CHECK0 (value, "Null pointer for a key value passed to tapi_get.");

  std::string s = decode (lookup (key));

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get.");
    return;
  }

  strncpy (value, s.c_str (), size);
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with [x,y] coordinate substitution, ready to use.
extern "C" void tapi_get_xy (
  const char* key,
  char* value,
  size_t size,
  int x,
  int y)
{
  CHECK0 (key,   "Null pointer to a terminal key passed to tapi_get_xy.");
  CHECK0 (value, "Null pointer for a key value passed to tapi_get_xy.");

  std::string s = encode (decode (lookup (key)), x, y);

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get_xy.");
    return;
  }

  strncpy (value, s.c_str (), size);
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with string substitution, ready to use.
extern "C" void tapi_get_str (
  const char* key,
  char* value,
  size_t size,
  const char* str)
{
  CHECK0 (key,   "Null pointer to a terminal key passed to tapi_get_str.");
  CHECK0 (value, "Null pointer for a key value passed to tapi_get_str.");
  CHECK0 (str,   "Null pointer for a substitution passed to tapi_get_str.");

  std::string s = encode (decode (lookup (key)), str);

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get_str.");
    return;
  }

  strncpy (value, s.c_str (), size);
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
static std::string decode (const std::string& input)
{
  std::string output = input;

  while (output.find ("_E_") != std::string::npos)
    output = substitute (output, "_E_", "\033");

  return substitute (output, "_B_", "\007");
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
