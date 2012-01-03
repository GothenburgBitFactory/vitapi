////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010-2012, Göteborg Bit Factory.
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
static int utf8_length (const std::string&);

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

  output << tapi_get ("ti", ti, MAX_TAPI_SIZE)
         << tapi_get ("Alt", alt, MAX_TAPI_SIZE);

  full_screen = true;
}

////////////////////////////////////////////////////////////////////////////////
// End use of full screen.
extern "C" void vapi_end_full_screen ()
{
  char te[MAX_TAPI_SIZE];

  output << tapi_get ("te", te, MAX_TAPI_SIZE);

  full_screen = false;
}

////////////////////////////////////////////////////////////////////////////////
// Clear the screen.
extern "C" void vapi_clear ()
{
  char cl[MAX_TAPI_SIZE];

  output << tapi_get ("cl", cl, MAX_TAPI_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
// Move cursor.
extern "C" void vapi_moveto (int x, int y)
{
  CHECKX0 (x, "Invalid x coordinate passed to vapi_moveto.");
  CHECKY0 (y, "Invalid y coordinate passed to vapi_moveto.");

  char mv[MAX_TAPI_SIZE];
  output << tapi_get_xy ("Mv", mv, MAX_TAPI_SIZE, x, y);
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

/*
  TODO Why does this code not work?

  // The longest color sequence is:   ^[ [ 4 ; 5 ; 38 ; 100 m <string> ^[ [ 0 m
  // That adds up to 13 (prologue) + 4 (epilogue) + 1 (null character) = 18.
  int safe_size = strlen (text) + 18;
  char* buf = new char [safe_size];
  if (buf)
  {
    strncpy (buf, text, safe_size);
    output << color_colorize (buf, safe_size, c);
    delete [] buf;
  }
*/

  char buf [4096];
  strncpy (buf, text, 4096);
  output << color_colorize (buf, 4096, c);
}

////////////////////////////////////////////////////////////////////////////////
// Draw text at position.
// If only part of the string is visible, truncate it.
extern "C" void vapi_pos_text (int x, int y, const char* text)
{
  CHECK0  (text, "Null pointer passed to vapi_pos_text.");

  // Don't bother displaying off-screen text.
  int full_len = strlen (text);
  int len = utf8_length (text);
  if (y < 1            ||
      y > screenHeight ||
      x > screenWidth  ||
      x + len - 1 < 1)
    return;

  // Amount of text that should be truncated off the left or right.
  int ltrunc = 0;
  int rtrunc = 0;

  if (x < 1)
  {
    ltrunc = 1 - x;
    x = 1;
  }

  if (x + len - 1 > screenWidth)
    rtrunc = x + len - 1 - screenWidth;

  vapi_moveto (x, y);
  if (ltrunc != 0 ||
      rtrunc != 0)
    vapi_text (std::string (text).substr (ltrunc, full_len - ltrunc - rtrunc).c_str ());
  else
    vapi_text (text);
}

////////////////////////////////////////////////////////////////////////////////
// Draw colored text at position
// If only part of the string is visible, truncate it.
extern "C" void vapi_pos_color_text (int x, int y, color c, const char* text)
{
  CHECKC0 (c,    "Invalid color passed to vapi_pos_color_text.");
  CHECK0  (text, "Null pointer passed to vapi_pos_color_text.");

  // Don't bother displaying off-screen text.
  int full_len = strlen (text);
  int len = utf8_length (text);
  if (y < 1            ||
      y > screenHeight ||
      x > screenWidth  ||
      x + len - 1 < 1)
    return;

  // Amount of text that should be truncated off the left or right.
  int ltrunc = 0;
  int rtrunc = 0;

  if (x < 1)
  {
    ltrunc = 1 - x;
    x = 1;
  }

  if (x + len - 1 > screenWidth)
    rtrunc = x + len - 1 - screenWidth;

  vapi_moveto (x, y);
  if (ltrunc != 0 ||
      rtrunc != 0)
    vapi_color_text (
      c, std::string (text).substr (ltrunc, full_len - ltrunc - rtrunc).c_str ());
  else
    vapi_color_text (c, text);
}

////////////////////////////////////////////////////////////////////////////////
// Draw a colored rectangle, cropping if necessary.
extern "C" void vapi_rectangle (int x, int y, int w, int h, color c)
{
  CHECKC0 (c, "Invalid color passed to vapi_rectangle.");
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
  output << tapi_get_str ("Ttl", ttl, MAX_TAPI_SIZE, title)
         << std::flush;
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
static int utf8_length (const std::string& str)
{
  int byteLength = str.length ();
  int charLength = byteLength;
  const char* data = str.data ();

  // Decrement the number of bytes for each byte that matches 0b10??????
  // this way only the first byte of any utf8 sequence is counted.
  for (int i = 0; i < byteLength; i++)
  {
    // Extract the first two bits and check whether they are 10
    if ((data[i] & 0xC0) == 0x80)
      charLength--;
  }

  return charLength;
}
////////////////////////////////////////////////////////////////////////////////

