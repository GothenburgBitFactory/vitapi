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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <vitapi.h>
#include <check.h>
#include <util.h>

static std::stringstream output; // Output buffer
static bool full_screen = false; // Should deinitialize restore?
static bool has_status  = false; // Terminal has status area

static bool handled = false;     // Latch
static int screenWidth  = 80;    // Terminal width
static int screenHeight = 24;    // Terminal height (may include status line)

#define MAX_TAPI_SIZE 64         // Max expected key size.

static void setupSignalHandler ();
static void restoreSignalHandler ();
static void getTerminalSize (int&, int&);
static void handler (int);

////////////////////////////////////////////////////////////////////////////////
// Initialize visual processing.
extern "C" int vapi_initialize ()
{
  output.str ("");

  char* term = getenv ("TERM");
  if (term)
  {
    tapi_initialize (term);

    char hs[MAX_TAPI_SIZE];
    tapi_get ("hs", hs, MAX_TAPI_SIZE);
    has_status = hs != "" ? true : false;

    getTerminalSize (screenWidth, screenHeight);
    setupSignalHandler ();
    return 0;
  }

  return -1;
}

////////////////////////////////////////////////////////////////////////////////
// End of visual processing.
extern "C" void vapi_deinitialize ()
{
  if (full_screen)
    vapi_end_full_screen ();

  vapi_refresh ();
  restoreSignalHandler ();
}

////////////////////////////////////////////////////////////////////////////////
// Update the display.
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
// Discard accumulated but unrefreshed output.
extern "C" int vapi_discard ()
{
  int bytes = output.str ().size ();
  output.str ("");

  return bytes;
}

////////////////////////////////////////////////////////////////////////////////
// Use the full screen.
extern "C" void vapi_full_screen ()
{
  char ti[MAX_TAPI_SIZE];
  char alt[MAX_TAPI_SIZE];
  tapi_get ("ti", ti, MAX_TAPI_SIZE);
  tapi_get ("Alt", alt, MAX_TAPI_SIZE);
  output << ti << alt;

  full_screen = true;
}

////////////////////////////////////////////////////////////////////////////////
// End use of full screen.
extern "C" void vapi_end_full_screen ()
{
  char te[MAX_TAPI_SIZE];
  tapi_get ("te", te, MAX_TAPI_SIZE);
  output << te;

  full_screen = false;
}

////////////////////////////////////////////////////////////////////////////////
// Clear the screen.
extern "C" void vapi_clear ()
{
  char cl[MAX_TAPI_SIZE];
  tapi_get ("cl", cl, MAX_TAPI_SIZE);
  output << cl;
}

////////////////////////////////////////////////////////////////////////////////
// Move cursor.
extern "C" void vapi_moveto (int x, int y)
{
  CHECKX0 (x, "Invalid x coordinate passed to vapi_moveto.");
  CHECKY0 (y, "Invalid y coordinate passed to vapi_moveto.");

  char mv[MAX_TAPI_SIZE];
  tapi_get_xy ("Mv", mv, MAX_TAPI_SIZE, x, y);
  output << mv;
}

////////////////////////////////////////////////////////////////////////////////
// Draw text at cursor.
// Note: there is no cropping of text based on location.
extern "C" void vapi_text (const char* text)
{
  CHECK0 (text, "Null pointer passed to vapi_text.");

  output << text;
}

////////////////////////////////////////////////////////////////////////////////
// Draw colored text at cursor.
extern "C" void vapi_color_text (color c, const char* text)
{
  CHECKC0 (c,    "Invalid color passed to vapi_color_text.");
  CHECK0  (text, "Null pointer passed to vapi_color_text.");

  // The longest color sequence is:   ^[ [ 4 ; 5 ; 38 ; 100 m <string> ^[ [ 0 m
  // That adds up to 13 (prologue) + 4 (epilogue) + 1 (null character) = 18.
  int safe_size = strlen (text) + 18;
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
// Draw text at position.
// If only part of the string is visible, truncate it.
extern "C" void vapi_pos_text (int x, int y, const char* text)
{
  CHECK0  (text, "Null pointer passed to vapi_pos_text.");

  // Don't bother displaying off-screen text.
  int len = strlen (text);
  if (y < 1            ||
      y > screenHeight ||
      x > screenWidth  ||
      x + len - 1 < 1)
    return;

  // Amount of text that should be truncated off the left or right.
  int ltrunc  = 0;
  int rtrunc = 0;

  if (x < 1)
  {
    ltrunc = 1 - x;
    x = 1;
  }

  if (x + len - 1 > screenWidth)
    rtrunc = x + len - 1 - screenWidth;

  vapi_moveto (x, y);
  vapi_text (std::string (text).substr (ltrunc, len - ltrunc - rtrunc).c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Draw colored text at position
// If only part of the string is visible, truncate it.
extern "C" void vapi_pos_color_text (int x, int y, color c, const char* text)
{
  CHECKC0 (c,    "Invalid color passed to vapi_pos_color_text.");
  CHECK0  (text, "Null pointer passed to vapi_pos_color_text.");

  // Don't bother displaying off-screen text.
  int len = strlen (text);
  if (y < 1            ||
      y > screenHeight ||
      x > screenWidth  ||
      x + len - 1 < 1)
    return;

  // Amount of text that should be truncated off the left or right.
  int ltrunc  = 0;
  int rtrunc = 0;

  if (x < 1)
  {
    ltrunc = 1 - x;
    x = 1;
  }

  if (x + len - 1 > screenWidth)
    rtrunc = x + len - 1 - screenWidth;

  vapi_moveto (x, y);
  vapi_color_text (
    c, std::string (text).substr (ltrunc, len - ltrunc - rtrunc).c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Draw a colored rectangle, cropping if necessary.
extern "C" void vapi_rectangle (int x, int y, int w, int h, color c)
{
  CHECKC0 (c,     "Invalid color passed to vapi_rectangle.");
  CHECKW0 (w, "Invalid width.");
  CHECKW0 (h, "Invalid height.");

  // Don't bother displaying a completely off-screen rectangle.
  if (x + w < 1 || x > screenWidth || y + h < 1 || y > screenHeight)
    return;

  // The rectangle is at least partially visible, so crop if necessary.
  x = min (max (x, 1), screenWidth);
  y = min (max (y, 1), screenHeight);
  w = min (max (w, 1), screenWidth - x + 1);
  h = min (max (h, 1), screenHeight - y + 1);

  std::string line (w, ' ');

  for (int i = 0; i < h; ++i)
    vapi_pos_color_text (x, y + i, c, line.c_str ());
}

////////////////////////////////////////////////////////////////////////////////
// Set the terminal title.
extern "C" void vapi_title (const char* title)
{
  CHECK0 (title, "Null pointer passed to vapi_title.");

  char ttl[MAX_TAPI_SIZE];
  tapi_get_str ("Ttl", ttl, MAX_TAPI_SIZE, title);
  output << ttl << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
static void setupSignalHandler ()
{
  if (! handled)
  {
    signal (SIGWINCH, handler);
    signal (SIGINT,   handler);
    signal (SIGQUIT,  handler);
    signal (SIGKILL,  handler);

    handled = true;
  }
}

////////////////////////////////////////////////////////////////////////////////
static void restoreSignalHandler ()
{
  if (handled)
  {
    signal (SIGWINCH, SIG_DFL);
    signal (SIGINT,   SIG_DFL);
    signal (SIGQUIT,  SIG_DFL);
    signal (SIGKILL,  SIG_DFL);

    handled = false;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Get the terminal width.
extern "C" int vapi_width ()
{
  return screenWidth;
}

////////////////////////////////////////////////////////////////////////////////
// Get the terminal height.
extern "C" int vapi_height ()
{
#ifdef CYGWIN
  return screenHeight;
#else
  return screenHeight + (has_status ? 1 : 0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
static void getTerminalSize (int& w, int& h)
{
  unsigned short buff[4];
  if (ioctl (fileno(stdout), TIOCGWINSZ, &buff) != -1)
  {
    h = buff[0];
    w = buff[1];
  }
}

////////////////////////////////////////////////////////////////////////////////
static void handler (int sig)
{
  if (sig == SIGWINCH)
  {
    getTerminalSize (screenWidth, screenHeight);
//    ungetc (0432, stdin);
    ungetc (65, stdin);
  }

  // NOP for all other (trapped) signals.
}

////////////////////////////////////////////////////////////////////////////////

