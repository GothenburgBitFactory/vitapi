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
#include <map>
#include <deque>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <termios.h>
#include "iapi.h"

#ifdef SOLARIS
#define CBREAK O_CBREAK
#include <sys/ttold.h>
#endif

static void translate (std::deque <int>&);
static void translateMouse (std::deque <int>&);
static bool same (const std::string&, const std::deque <int>&);
static void assign (char*, int, const char*);

////////////////////////////////////////////////////////////////////////////////
static struct termios tty;                    // Original I/O state.
static std::map <int, std::string> sequences; // Key -> sequence mapping.
static int mouse_x = -1, mouse_y = -1;        // Last known mouse position.

////////////////////////////////////////////////////////////////////////////////
bool iapi_initialize ()
{
  // Save the initial state for later restoration.
  tcgetattr (0, &tty);
/*
  std::cout << "# tty.c_iflag " << tty.c_iflag << std::endl;
  if (tty.c_iflag & IGNBRK)  std::cout << "#   IGNBRK"  << std::endl;
  if (tty.c_iflag & BRKINT)  std::cout << "#   BRKINT"  << std::endl;
  if (tty.c_iflag & IGNPAR)  std::cout << "#   IGNPAR"  << std::endl;
  if (tty.c_iflag & PARMRK)  std::cout << "#   PARMRK"  << std::endl;
  if (tty.c_iflag & INPCK)   std::cout << "#   INPCK"   << std::endl;
  if (tty.c_iflag & ISTRIP)  std::cout << "#   ISTRIP"  << std::endl;
  if (tty.c_iflag & INLCR)   std::cout << "#   INCLR"   << std::endl;
  if (tty.c_iflag & ICRNL)   std::cout << "#   ICRNL"   << std::endl;
  if (tty.c_iflag & IXON)    std::cout << "#   IXON"    << std::endl;
  if (tty.c_iflag & IXANY)   std::cout << "#   IXANY"   << std::endl;
  if (tty.c_iflag & IXOFF)   std::cout << "#   IXOFF"   << std::endl;
#ifndef HAIKU
  if (tty.c_iflag & IMAXBEL) std::cout << "#   IMAXBEL" << std::endl;
#endif

  std::cout << "# tty.c_oflag " << tty.c_oflag << std::endl;
  if (tty.c_oflag & OPOST)   std::cout << "#   OPOST"   << std::endl;
  if (tty.c_oflag & ONLCR)   std::cout << "#   ONLCR"   << std::endl;
  if (tty.c_oflag & OCRNL)   std::cout << "#   OCRNL"   << std::endl;
  if (tty.c_oflag & ONOCR)   std::cout << "#   ONOCR"   << std::endl;
  if (tty.c_oflag & ONLRET)  std::cout << "#   ONLRET"  << std::endl;

  std::cout << "# tty.c_lflag " << tty.c_lflag << std::endl;
  if (tty.c_lflag & ISIG)    std::cout << "#   ISIG"    << std::endl;
  if (tty.c_lflag & ICANON)  std::cout << "#   ICANON"  << std::endl;
  if (tty.c_lflag & NOFLSH)  std::cout << "#   NOFLSH"  << std::endl;
#ifndef HAIKU
  if (tty.c_lflag & FLUSHO)  std::cout << "#   FLUSHO"  << std::endl;
  if (tty.c_lflag & PENDIN)  std::cout << "#   PENDIN"  << std::endl;
#endif
  if (tty.c_lflag & IEXTEN)  std::cout << "#   IEXTEN"  << std::endl;
  if (tty.c_lflag & TOSTOP)  std::cout << "#   TOSTOP"  << std::endl;
  if (tty.c_lflag & ECHO)    std::cout << "#   ECHO"    << std::endl;
*/

/*
  char* term = getenv ("TERM");
  char buffer[2048];
  if ((term && tgetent (buffer, term) > 0) ||
      tgetent (buffer, "xterm-color") > 0  ||
      tgetent (buffer, "xterm")       > 0  ||
      tgetent (buffer, "vt100")       > 0)
  {
    char *bufp = buffer;

    assign (bufp, IAPI_KEY_LEFT,      "kl");
    assign (bufp, IAPI_KEY_RIGHT,     "kr");
    assign (bufp, IAPI_KEY_UP,        "ku");
    assign (bufp, IAPI_KEY_DOWN,      "kd");
*/
/*
    sequences[IAPI_KEY_LEFT]          = "\033[D";
    sequences[IAPI_KEY_RIGHT]         = "\033[C";
    sequences[IAPI_KEY_UP]            = "\033[A";
    sequences[IAPI_KEY_DOWN]          = "\033[B";
*/
/*
    assign (bufp, IAPI_KEY_F1,        "k1");
    assign (bufp, IAPI_KEY_F2,        "k2");
    assign (bufp, IAPI_KEY_F3,        "k3");
    assign (bufp, IAPI_KEY_F4,        "k4");
    assign (bufp, IAPI_KEY_F5,        "k5");
    assign (bufp, IAPI_KEY_F6,        "k6");
    assign (bufp, IAPI_KEY_F7,        "k7");
    assign (bufp, IAPI_KEY_F8,        "k8");
    assign (bufp, IAPI_KEY_F9,        "k9");
    assign (bufp, IAPI_KEY_F10,       "k0");

    assign (bufp, IAPI_KEY_HOME,      "kH");
    assign (bufp, IAPI_KEY_BACKSPACE, "kb");
    assign (bufp, IAPI_KEY_DEL,       "kD");
    assign (bufp, IAPI_KEY_PGUP,      "kP");
    assign (bufp, IAPI_KEY_PGDN,      "kN");

    std::cout << "\033[?1h";          // Application cursor keys
    return true;
  }
*/

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void iapi_deinitialize ()
{
  std::cout << "\033[?1l";            // Normal cursor keys
  tcsetattr (0, TCSANOW, &tty);
}

////////////////////////////////////////////////////////////////////////////////
void iapi_echo ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag |= ECHO;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
void iapi_noecho ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag &= ~ECHO;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
void iapi_raw ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag &= ~ICANON;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
void iapi_noraw ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag |= ICANON;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
// Without the std::flush, no mouse events are received, unless a key is
// pressed.
void iapi_mouse ()
{
  std::cout << "\033[?1000h" << std::flush;  // Mouse press & release
}

////////////////////////////////////////////////////////////////////////////////
void iapi_nomouse ()
{
  std::cout << "\033[?1000l";  // Mouse press & release
}

////////////////////////////////////////////////////////////////////////////////
// Without the std::flush, no mouse events are received, unless a key is
// pressed.
void iapi_mouse_tracking ()
{
  std::cout << "\033[?1002h" << std::flush;  // Cell motion tracking
}

////////////////////////////////////////////////////////////////////////////////
void iapi_nomouse_tracking ()
{
  std::cout << "\033[?1002l";  // Cell motion tracking
}

////////////////////////////////////////////////////////////////////////////////
void iapi_mouse_pos (int& x, int& y)
{
  x = mouse_x;
  y = mouse_y;
}

////////////////////////////////////////////////////////////////////////////////
// This routine reads typed characters and translates certain sequences into
// single key values.  Examples:
//
// Sequence                Key
//   a                       a
//   <Escape>                <Escape>
//   <Escape> [ A            <Up>
//   <Escape> [ 1 5 ~        <F5>
//
// It is also reasonable for the user to enter <Escape> [ A directly from the
// keyboard in three keystrokes, and in this case it should not be recognized as
// <Up>, but as three separate keystrokes.
//
// In addition, we need to make sure that keystrokes were entered before we read
// them, so that the getchar() call does not block.
//
// The solution to this problem involves select, and a tricky algorithm, which
// is:
//
// 1. If there are any characters in the overflow buffer, serve those first.
// 2. If there are none, the use select on stdin with no timeout to block until
//    a key is pressed.
// 3. Once a keypress is detected by select, call ioctl FIONREAD to determine
//    how many keys were pressed, and read them.
// 4. Scan the keys read to determine whether any recognized sequences are found
//    and replace them with a composite key value.
// 5. Return the first key press, and append any residual keys to the overflow
//    buffer.
//
int iapi_getch ()
{
  // An 'ungetchar' buffer for sequences that were read, but not recognized.
  // This buffer should be depleted before calling getchar again.
  static std::deque <int> overflow;
  std::deque <int> sequence;

  // Get the (first?) char.
  if (overflow.size ())
  {
    sequence.push_back (overflow[0]);
    overflow.pop_front ();
  }
  else
  {
    int fd = fileno (stdin);
    fd_set readset;
    FD_ZERO (&readset);
    FD_SET (fd, &readset);
    if (select (fd + 1, &readset, NULL, NULL, NULL) > 0 &&
        FD_ISSET (0, &readset))
    {
      int quantity;
#ifdef CYGWIN
      ioctl (0, TIOCINQ, &quantity);
#else
      ioctl (0, FIONREAD, &quantity);
#endif
      for (int i = 0; i < quantity; ++i)
        sequence.push_back (getchar ());
    }
  }

  // Convert sequences into single key values.
  translate (sequence);
  translateMouse (sequence);

  // Return the first (perhaps only) key pressed.
  int key = sequence[0];
  sequence.pop_front ();

  // Untranslated keystrokes are sent to overflow.
  std::deque <int>::iterator it;
  for (it = sequence.begin (); it != sequence.end (); ++it)
    overflow.push_back (*it);

  return key;
}

////////////////////////////////////////////////////////////////////////////////
// Replace successive keys in the sequence with aggregate codes.
static void translate (std::deque <int>& sequence)
{
  std::map <int, std::string>::iterator it;
  for (it = sequences.begin (); it != sequences.end (); ++it)
  {
    if (same (it->second, sequence))
    {
      std::deque <int> translated;
      translated.push_back (it->first);

      for (unsigned int i = it->second.size (); i < sequence.size (); ++i)
        translated.push_back (sequence[i]);

      sequence = translated;
      break;  // Iterator no longer valid.
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//    <Escape> [ M b x y
//
//    b: 000CMSxx
//    x: 32 + col
//    y: 32 + row
//
//    xx: 00  b1 pressed
//        01  b2 pressed
//        10  b3 pressed
//        11  release
//
static void translateMouse (std::deque <int>& sequence)
{
  if (sequence.size () >=  6  &&
      sequence[0]      == 27  &&
      sequence[1]      == '[' &&
      sequence[2]      == 'M')
  {
/*
    std::cout << "# event "
              << (sequence[3] & 0x8000 ? 1 : 0)
              << (sequence[3] & 0x4000 ? 1 : 0)
              << (sequence[3] & 0x2000 ? 1 : 0)
              << (sequence[3] & 0x1000 ? 1 : 0)
              << (sequence[3] & 0x0800 ? 1 : 0)
              << (sequence[3] & 0x0400 ? 1 : 0)
              << (sequence[3] & 0x0200 ? 1 : 0)
              << (sequence[3] & 0x0100 ? 1 : 0)
              << (sequence[3] & 0x0080 ? 1 : 0)
              << (sequence[3] & 0x0040 ? 1 : 0)
              << (sequence[3] & 0x0020 ? 1 : 0)
              << (sequence[3] & 0x0010 ? 1 : 0)
              << (sequence[3] & 0x0008 ? 1 : 0)
              << (sequence[3] & 0x0004 ? 1 : 0)
              << (sequence[3] & 0x0002 ? 1 : 0)
              << (sequence[3] & 0x0001 ? 1 : 0)
              << std::endl;
*/

    int key;
    switch (sequence[3] & 0x0003)
    {
    case 0: key = IAPI_MOUSE_1_CLICK; break;
    case 1: key = IAPI_MOUSE_2_CLICK; break;
    case 2: key = IAPI_MOUSE_3_CLICK; break;
    case 3: key = IAPI_MOUSE_RELEASE; break;
    }

    mouse_x = sequence[4] - 32;
    mouse_y = sequence[5] - 32;

    std::deque <int> translated;
    translated.push_back (key);

    for (unsigned int i = 6; i < sequence.size (); ++i)
      translated.push_back (sequence[i]);

    sequence = translated;
  }
}

////////////////////////////////////////////////////////////////////////////////
static bool same (const std::string& known, const std::deque <int>& unknown)
{
  unsigned int len = known.length ();
  unsigned int size = unknown.size ();

  // Sequence is too short.
  if (len > size)
    return false;

  // Different.
  for (unsigned int i = 0; i < len && i < size; ++i)
    if (unknown[i] != (int) known[i])
      return false;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
static void assign (char* buffer, int key, const char* name)
{
  // char* value = tgetstr ((char*) name, &buffer);
  char* value;
  if (value)
    sequences[key] = value;
}

////////////////////////////////////////////////////////////////////////////////

