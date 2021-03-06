////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2017, Göteborg Bit Factory.
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

  data["vt100"] = data["vt102"] =
    "ku:_E_OA "
    "kd:_E_OB "
    "kr:_E_OC "
    "kl:_E_OD "
    "k1:_E_OP "
    "k2:_E_OQ "
    "k3:_E_OR "
    "k4:_E_OS "
    "k5:_E_Ot "
    "k6:_E_Ou "
    "k7:_E_Ov "
    "k8:_E_Ol "
    "k9:_E_Ow "
    "k0:_E_Oy "
    "kb:8 "
    "cl:_E_[H_E_[J$<50> "
    + common;

  data["vt220"] =
    "ku:_E_[A "
    "kd:_E_[B "
    "kr:_E_[C "
    "kl:_E_[D "
    "k1:_E_OP "
    "k2:_E_OQ "
    "k3:_E_OR "
    "k4:_E_OS "
    "k6:_E_[17~ "
    "k7:_E_[18~ "
    "k8:_E_[19~ "
    "k9:_E_[20~ "
    "kb:8 "
    "kP:_E_[5~ "
    "kN:_E_[6~ "
    "cl:_E_[H_E_[J "
    + common;

  data["xterm-color"] =
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
    "kb:8 "
    "kD:_E_[3~ "
    "kP:_E_[5~ "
    "kN:_E_[6~ "
    "ti:_E_7_E_[?47h "
    "te:_E_[2J_E_[?47l_E_8 "
    "cl:_E_[H_E_[2J "
    + common;

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
extern "C" const char* tapi_get (const char* key, char* value, size_t size)
{
  if (!key)
  {
    vitapi_set_error ("Null pointer to a terminal key passed to tapi_get.");
    return NULL;
  }

  if (!value)
  {
    vitapi_set_error ("Null pointer for a key value passed to tapi_get.");
    return NULL;
  }

  std::string s = decode (lookup (key));

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get.");
    return value;
  }

  strncpy (value, s.c_str (), size);
  return value;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with [x,y] coordinate substitution, ready to use.
extern "C" const char* tapi_get_xy (
  const char* key,
  char* value,
  size_t size,
  int x,
  int y)
{
  if (!key)
  {
    vitapi_set_error ("Null pointer to a terminal key passed to tapi_get_xy.");
    return NULL;
  }

  if (!value)
  {
    vitapi_set_error ("Null pointer for a key value passed to tapi_get_xy.");
    return NULL;
  }

  std::string s = encode (decode (lookup (key)), x, y);

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get_xy.");
    return value;
  }

  strncpy (value, s.c_str (), size);
  return value;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the control string with string substitution, ready to use.
extern "C" const char* tapi_get_str (
  const char* key,
  char* value,
  size_t size,
  const char* str)
{
  if (!key)
  {
    vitapi_set_error ("Null pointer to a terminal key passed to tapi_get_str.");
    return NULL;
  }

  if (!value)
  {
    vitapi_set_error ("Null pointer for a key value passed to tapi_get_str.");
    return NULL;
  }

  if (!str)
  {
    vitapi_set_error ("Null pointer for a substitution passed to tapi_get_str.");
    return value;
  }

  std::string s = encode (decode (lookup (key)), str);

  if (s.length () + 1 >= size)
  {
    vitapi_set_error ("Insufficient buffer size passed to tapi_get_str.");
    return value;
  }

  strncpy (value, s.c_str (), size);
  return value;
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
