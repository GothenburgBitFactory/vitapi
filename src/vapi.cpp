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

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <vitapi.h>

static std::stringstream output; // Output buffer
static int screenWidth  = 0;     // Terminal width
static int screenHeight = 0;     // Terminal height (may include status line)
static bool full_screen = false; // Should deinitialize restore?
static bool has_status  = false; // Terminal has status area

static void getTerminalSize (int&, int&);
static void handler (int);

////////////////////////////////////////////////////////////////////////////////
// Initialize visual processing
extern "C" int vapi_initialize ()
{
  output.str ("");

  getTerminalSize (screenWidth, screenHeight);

  char* term = getenv ("TERM");
  if (term)
  {
    tapi_initialize (term);

    char hs[64];
    tapi_get ("hs", hs);
    has_status = hs != "" ? true : false;

    // Handle assorted signals.
    signal (SIGWINCH, handler);
    signal (SIGINT,   handler);
    signal (SIGQUIT,  handler);
    signal (SIGKILL,  handler);

    return 0;
  }

  return -1;
}

////////////////////////////////////////////////////////////////////////////////
// End of visual processing
extern "C" void vapi_deinitialize ()
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
// Update the display
extern "C" int vapi_refresh ()
{
  if (output.str ().size ())
  {
    std::cout << output.str () << std::flush;
    output.str ("");
    return 0;
  }

  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// Use the full screen
extern "C" void vapi_full_screen ()
{
  char ti[64];
  char alt[64];
  tapi_get ("ti", ti);
  tapi_get ("Alt", alt);
  output << ti << alt;

  full_screen = true;
}

////////////////////////////////////////////////////////////////////////////////
// End use of full screen
extern "C" void vapi_end_full_screen ()
{
  char te[64];
  tapi_get ("te", te);
  output << te;

  full_screen = false;
}

////////////////////////////////////////////////////////////////////////////////
// Clear the screen
extern "C" void vapi_clear ()
{
  char cl[64];
  tapi_get ("cl", cl);
  output << cl;
}

////////////////////////////////////////////////////////////////////////////////
// Move cursor
extern "C" void vapi_moveto (int x, int y)
{
  char mv[64];
  tapi_get_xy ("Mv", mv, x, y);
  output << mv;
}

////////////////////////////////////////////////////////////////////////////////
// Draw text at cursor
extern "C" void vapi_text (const char* text)
{
  output << text;
}

////////////////////////////////////////////////////////////////////////////////
// Draw colored text at cursor
extern "C" void vapi_color_text (color c, const char* text)
{
  int safe_size = strlen (text) + 24;
  char* buf = new char [safe_size];
  if (buf)
  {
    strncpy (buf, text, safe_size);
    color_colorize (buf, safe_size, c);
    output << buf;
    delete [] buf;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Draw text at position
// TODO Text placement should be cropped.
extern "C" void vapi_pos_text (int x, int y, const char* text)
{
  vapi_moveto (x, y);
  vapi_text (text);
}

////////////////////////////////////////////////////////////////////////////////
// Draw colored text at position
// TODO Text placement should be cropped.
extern "C" void vapi_pos_color_text (int x, int y, color c, const char* text)
{
  vapi_moveto (x, y);
  vapi_color_text (c, text);
}

////////////////////////////////////////////////////////////////////////////////
// Draw a colored rectangle
// TODO Rectangle should be cropped.
extern "C" void vapi_rectangle (int x, int y, int w, int h, color c)
{
  std::string line (w, ' ');

  for (int i = 0; i < h; ++i)
    vapi_pos_color_text (x, y + i, c, line.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Get the terminal width
extern "C" int vapi_width ()
{
  return screenWidth;
}

////////////////////////////////////////////////////////////////////////////////
// Get the terminal height
extern "C" int vapi_height ()
{
#ifdef CYGWIN
  return screenHeight;
#else
  return screenHeight + (has_status ? 1 : 0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Set the terminal title
extern "C" void vapi_title (const char* title)
{
  char ttl[64];
  tapi_get_str ("Ttl", ttl, title);
  output << ttl << std::flush;
}

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

