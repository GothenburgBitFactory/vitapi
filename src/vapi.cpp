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
//#include <ncurses.h>
#include <term.h>
#include "vapi.h"

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
static char* ti_string  = NULL;  // Full screen mode
static char* te_string  = NULL;  // End full screen mode
static char* cl_string  = NULL;  // Clear screen
static bool hs          = true;  // Has status line
static bool full_screen = false; // Should deinitialize restore?

////////////////////////////////////////////////////////////////////////////////
static void handler (int sig)
{
  if (sig == SIGWINCH)
  {
    unsigned short buff[4];
    if (ioctl (0, TIOCGWINSZ, &buff) != -1)
    {
      screenHeight = buff[0];
      screenWidth  = buff[1];
    }
  }

  // NOP for all other (trapped) signals.
}

////////////////////////////////////////////////////////////////////////////////
bool vapi_initialize ()
{
  output.str ("");

  char* term = getenv ("TERM");
  if (term)
  {
/*
    if (tgetent (buffer, term) > 0)
    {
      screenWidth  = tgetnum  ((char*) "co");
      screenHeight = tgetnum  ((char*) "li");

      char *bufp = buffer;
      ti_string    = tgetstr  ((char*) "ti", &bufp);
      te_string    = tgetstr  ((char*) "te", &bufp);
      cl_string    = tgetstr  ((char*) "cl", &bufp);

      hs           = tgetflag ((char*) "hs") != ERR ? true : false;

      // Handle assorted signals.
      signal (SIGWINCH, handler);
      signal (SIGINT,   handler);
      signal (SIGQUIT,  handler);
      signal (SIGKILL,  handler);

      return true;
    }
*/
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
  if (ti_string)
    output << ti_string;

  output << "\033[1049h";  // Alt screen
  full_screen = true;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_end_full_screen ()
{
  if (te_string)
    output << te_string;

  full_screen = false;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_clear ()
{
  if (cl_string)
    output << cl_string;
}

////////////////////////////////////////////////////////////////////////////////
void vapi_moveto (int x, int y)
{
  output << "\033[" << y << ";" << x << "H";
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
  output << "\033]2;" << title << "\007";
}

////////////////////////////////////////////////////////////////////////////////
