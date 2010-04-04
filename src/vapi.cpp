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

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <term.h>
#include "vapi.h"
#include "Caps.h"

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) ((a)<(b)?(a):(b))

#ifdef MAX
#undef MAX
#endif
#define MAX(a,b) ((a)>(b)?(a):(b))

////////////////////////////////////////////////////////////////////////////////
static std::stringstream output;
static char buffer [2048];

static int screenWidth  = 0;     // Terminal width
static int screenHeight = 0;     // Terminal height (may include status line)
static bool hs          = true;  // Has status line
static bool full_screen = false; // Should deinitialize restore?
static Caps caps;                // Terminal capabilities.

////////////////////////////////////////////////////////////////////////////////
static void getTerminalSize (int& w, int& h)
{
  unsigned short buff[4];
  if (ioctl (0, TIOCGWINSZ, &buff) != -1)
  {
    screenHeight = buff[0];
    screenWidth  = buff[1];
  }
}

////////////////////////////////////////////////////////////////////////////////
static void handler (int sig)
{
  if (sig == SIGWINCH)
    getTerminalSize (screenWidth, screenHeight);

  // NOP for all other (trapped) signals.
}

////////////////////////////////////////////////////////////////////////////////
bool vapi_initialize ()
{
  output.str ("");

  getTerminalSize (screenWidth, screenHeight);

  char* term = getenv ("TERM");
  if (term)
  {
    caps.initialize (term);

    hs = caps.get ("hs") != "" ? true : false;

    // Handle assorted signals.
    signal (SIGWINCH, handler);
    signal (SIGINT,   handler);
    signal (SIGQUIT,  handler);
    signal (SIGKILL,  handler);

    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_deinitialize ()
{
  if (full_screen)
    vapi_end_full_screen ();

  vapi_refresh ();
  signal (SIGWINCH, SIG_DFL);
  signal (SIGINT,   SIG_DFL);
  signal (SIGQUIT,  SIG_DFL);
  signal (SIGKILL,  SIG_DFL);
}

////////////////////////////////////////////////////////////////////////////////
bool vapi_refresh ()
{
  if (output.str ().size ())
  {
    std::cout << output.str () << std::flush;
    output.str ("");
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_full_screen ()
{
  output << caps.get ("ti") << caps.get ("Alt");  // Alt screen
  full_screen = true;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_end_full_screen ()
{
  output << caps.get ("te");
  full_screen = false;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_clear ()
{
  output << caps.get ("cl");
}

////////////////////////////////////////////////////////////////////////////////
void vapi_moveto (int x, int y)
{
  output << caps.get ("Mv", x, y);
}

////////////////////////////////////////////////////////////////////////////////
void vapi_text (const std::string& text)
{
  output << text.c_str ();
}

////////////////////////////////////////////////////////////////////////////////
void vapi_text (const Color& color, const std::string& text)
{
  output << color.colorize (text.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// TODO Text placement should be cropped.
void vapi_text (int x, int y, const std::string& text)
{
  vapi_moveto (x, y);
  output << text.c_str ();
}

////////////////////////////////////////////////////////////////////////////////
// TODO Text placement should be cropped.
void vapi_text (int x, int y, const Color& color, const std::string& text)
{
  vapi_moveto (x, y);
  output << color.colorize (text.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// TODO Rectangles should be cropped.
void vapi_rectangle (int x, int y, int w, int h, const Color& color)
{
  std::string line;
  for (int i = 0; i < w; ++i)
    line += " ";

  for (int i = 0; i < h; ++i)
    vapi_text (x, y + i, color, line);
}

////////////////////////////////////////////////////////////////////////////////
int vapi_width ()
{
  return screenWidth;
}

////////////////////////////////////////////////////////////////////////////////
int vapi_height ()
{
#ifdef CYGWIN
  return screenHeight;
#else
  return screenHeight + (hs ? 1 : 0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void vapi_title (const std::string& title)
{
  output << caps.get ("Ttl", title) << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
