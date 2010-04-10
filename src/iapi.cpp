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
#include <map>
#include <deque>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#ifdef SOLARIS
#define CBREAK O_CBREAK
#include <sys/ttold.h>
#endif

#include <vitapi.h>
#include <check.h>

static struct termios tty;                    // Original I/O state.
static std::map <int, std::string> sequences; // Key -> sequence mapping.
static int mouse_x = -1, mouse_y = -1;        // Last known mouse position.

#define MAX_TAPI_SIZE 64                      // Max expected key size.

static void translate (std::deque <int>&);
static void translateMouse (std::deque <int>&);
static bool same (const std::string&, const std::deque <int>&);

////////////////////////////////////////////////////////////////////////////////
// Initialize for processed input
extern "C" int iapi_initialize ()
{
  // Save the initial state for later restoration.
  tcgetattr (0, &tty);

  char* term = getenv ("TERM");
  if (term)
  {
    if (! tapi_initialize (term))
    {
      char value[MAX_TAPI_SIZE];
      tapi_get ("ku", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_UP]        = value;
      tapi_get ("kd", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_DOWN]      = value;
      tapi_get ("kr", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_RIGHT]     = value;
      tapi_get ("kl", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_LEFT]      = value;
      tapi_get ("k1", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F1]        = value;
      tapi_get ("k2", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F2]        = value;
      tapi_get ("k3", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F3]        = value;
      tapi_get ("k4", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F4]        = value;
      tapi_get ("k5", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F5]        = value;
      tapi_get ("k6", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F6]        = value;
      tapi_get ("k7", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F7]        = value;
      tapi_get ("k8", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F8]        = value;
      tapi_get ("k9", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F9]        = value;
      tapi_get ("k0", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_F10]       = value;
      tapi_get ("kH", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_HOME]      = value;
      tapi_get ("kb", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_BACKSPACE] = value;
      tapi_get ("kD", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_DEL]       = value;
      tapi_get ("kP", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_PGUP]      = value;
      tapi_get ("kN", value, MAX_TAPI_SIZE); sequences[IAPI_KEY_PGDN]      = value;

      tapi_get ("AM", value, MAX_TAPI_SIZE);
      std::cout << value;        // Application mode.
      return 0;
    }
  }

  return -1;
}

////////////////////////////////////////////////////////////////////////////////
// End of processed input
extern "C" void iapi_deinitialize ()
{
  char value[MAX_TAPI_SIZE];
  tapi_get ("NM", value, MAX_TAPI_SIZE);
  std::cout << value;            // Normal mode.

  tcsetattr (0, TCSANOW, &tty);  // Restore initial state.
}

////////////////////////////////////////////////////////////////////////////////
// Enable echo
extern "C" void iapi_echo ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag |= ECHO;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
// Disable echo
extern "C" void iapi_noecho ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag &= ~ECHO;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
// Enable raw mode
extern "C" void iapi_raw ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag &= ~ICANON;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
// Disable raw mode
extern "C" void iapi_noraw ()
{
  struct termios tmp;
  tcgetattr (0, &tmp);
  tmp.c_lflag |= ICANON;
  tcsetattr (0, TCSANOW, &tmp);
}

////////////////////////////////////////////////////////////////////////////////
// Enable mouse clicks
// Note: Without the std::flush, no mouse events are received, until a key is
// pressed.
extern "C" void iapi_mouse ()
{
  char value[MAX_TAPI_SIZE];
  tapi_get ("Ms1", value, MAX_TAPI_SIZE);
  std::cout << value << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Disable mouse clicks
extern "C" void iapi_nomouse ()
{
  char value[MAX_TAPI_SIZE];
  tapi_get ("Ms0", value, MAX_TAPI_SIZE);
  std::cout << value;
}

////////////////////////////////////////////////////////////////////////////////
// Enable mouse clicks and tracking
// Note: Without the std::flush, no mouse events are received, until a key is
// pressed.
extern "C" void iapi_mouse_tracking ()
{
  char value[MAX_TAPI_SIZE];
  tapi_get ("Mt1", value, MAX_TAPI_SIZE);
  std::cout << value << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Disable mouse clicks and tracking
extern "C" void iapi_nomouse_tracking ()
{
  char value[MAX_TAPI_SIZE];
  tapi_get ("Mt0", value, MAX_TAPI_SIZE);
  std::cout << value;
}

////////////////////////////////////////////////////////////////////////////////
// Get last known mouse position
extern "C" void iapi_mouse_pos (int* x, int* y)
{
  CHECK0 (x, "Null pointer to x coordinate passed to iapi_mouse_pos");
  CHECK0 (y, "Null pointer to y coordinate passed to iapi_mouse_pos");

  *x = mouse_x;
  *y = mouse_y;
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
extern "C" int iapi_getch ()
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
    if (it->second.length () > 0 &&   // Some sequences have zero length.
        same (it->second, sequence))
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

