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
#include <sys/fcntl.h>

#ifdef USE_OLD_NON_PORTABLE_TECHNIQUE
#include <sys/ioctl.h>
#endif

#include <vitapi.h>
#include <check.h>

static struct termios tty;                    // Original I/O state.
static std::map <int, std::string> sequences; // Key -> sequence mapping.
static int mouse_x = -1, mouse_y = -1;        // Last known mouse position.
static bool mouse_control = false;            // Mouse modifier key.
static bool mouse_meta    = false;            // Mouse modifier key.
static bool mouse_shift   = false;            // Mouse modifier key.
static int sequence_delay = 1000;             // 10ms delay.  Note: 1ms reduces
                                              // missed sequences in translateMouse.

#define MAX_TAPI_SIZE 64                      // Max expected key size.

static void translate (std::deque <int>&);
static void translateMouse (std::deque <int>&);
static bool same (const std::string&, const std::deque <int>&);
static void blocking (int);
static void non_blocking (int);

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
      sequences[IAPI_KEY_UP]        = tapi_get ("ku", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_DOWN]      = tapi_get ("kd", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_RIGHT]     = tapi_get ("kr", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_LEFT]      = tapi_get ("kl", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F1]        = tapi_get ("k1", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F2]        = tapi_get ("k2", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F3]        = tapi_get ("k3", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F4]        = tapi_get ("k4", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F5]        = tapi_get ("k5", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F6]        = tapi_get ("k6", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F7]        = tapi_get ("k7", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F8]        = tapi_get ("k8", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F9]        = tapi_get ("k9", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_F10]       = tapi_get ("k0", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_HOME]      = tapi_get ("kH", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_BACKSPACE] = tapi_get ("kb", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_DEL]       = tapi_get ("kD", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_PGUP]      = tapi_get ("kP", value, MAX_TAPI_SIZE);
      sequences[IAPI_KEY_PGDN]      = tapi_get ("kN", value, MAX_TAPI_SIZE);

      std::cout << tapi_get ("AM", value, MAX_TAPI_SIZE); // Application mode.
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

  std::cout << tapi_get ("NM", value, MAX_TAPI_SIZE); // Normal mode.

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
// Disable raw mode
extern "C" void iapi_cooked ()
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

  std::cout << tapi_get ("Ms1", value, MAX_TAPI_SIZE)
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Disable mouse clicks
extern "C" void iapi_nomouse ()
{
  char value[MAX_TAPI_SIZE];

  std::cout << tapi_get ("Ms0", value, MAX_TAPI_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
// Enable mouse clicks and tracking
// Note: Without the std::flush, no mouse events are received, until a key is
// pressed.
extern "C" void iapi_mouse_tracking ()
{
  char value[MAX_TAPI_SIZE];

  std::cout << tapi_get ("Mt1", value, MAX_TAPI_SIZE)
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Disable mouse clicks and tracking
extern "C" void iapi_nomouse_tracking ()
{
  char value[MAX_TAPI_SIZE];
  std::cout << tapi_get ("Mt0", value, MAX_TAPI_SIZE);
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
// Ctrl key?
extern "C" int iapi_mouse_control ()
{
  return mouse_control ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
// Meta key?
extern "C" int iapi_mouse_meta ()
{
  return mouse_meta ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////////
// Shift key?
extern "C" int iapi_mouse_shift ()
{
  return mouse_shift ? 1 : 0;
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
// In addition, we need to make sure that the fgetc() call only blocks when we
// want it to.
//
// The solution is:
//
// 1. If there are any characters in the overflow buffer, serve those first.
// 2. If there are none, then make a blocking fgetc(stdin) call to read one
//    character.
// 3. If that character is not <Escape>, push it onto a queue.
// 4. If that character is an <Escape>, then delay for some period of time that
//    is significant from the computer's perspective, but unnoticeable from the
//    user's perspective, say 20ms.  Then make successive non-blocking
//    fgetc(stdin) calls until it returns an error, and push these onto the
//    queue.
// 5. Scan the queue for recognized sequences, and replace them.
// 6. Scan the queue for recognized mouse click/release/track sequences and
//    replace those while capturing the mouse position.
// 7. Return the first item in the queue.
//
extern "C" int iapi_getch ()
{
  // An 'ungetchar' buffer for sequences that were read, but not recognized.
  // This buffer should be depleted before calling fgetc again.
  static std::deque <int> sequence;
  int key;

  // Special case: if the sequence is empty, block on fgetc, waiting for at
  // least one character.
  if (sequence.size () == 0)
  {
/*  TODO Is this the correct way to deal with signals?
    do
    {
      key = fgetc (stdin);
    }
    while (key == -1 && errno == EAGAIN);
*/
    key = fgetc (stdin);
    sequence.push_back (key);
    usleep (sequence_delay);
  }

  // Now read all pending characters.
  usleep (sequence_delay);
  non_blocking (fileno (stdin));

  while ((key = fgetc (stdin)) > 0)
    sequence.push_back (key);

  blocking (fileno (stdin));

  // Convert sequences into single key values.
  translate (sequence);
  translateMouse (sequence);

  // Return the first (perhaps only) key pressed.
  key = sequence[0];
  sequence.pop_front ();

  return key;
}

////////////////////////////////////////////////////////////////////////////////
// Delay between related key presses.  For example, these sequences:
//
//   <Escape> [ A     - sequence generated by up-arrow key, typically with very
//                      little delay between them.
//   <Escape>, [, A   - three keys entered by the user, with a larger delay
//                      between them.
//
// The reason for this is to allow both cases, and to distinguish between them
// by the delay between key presses.
extern "C" int iapi_set_delay (int delay)
{
  if (delay < 0 || delay > 1000000)
  {
    vitapi_set_error (
      "Invalid delay passed to iapi_set_delay.  The value "
      "should be greater than 0 and no greater than 1,000,000.");
    return -1;
  }

  int old_value = sequence_delay;
  sequence_delay = delay;
  return old_value;
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
// TODO Remove most, if not all, of this comment.
//
// The motion reporting modes are strictly xterm extensions, and are not part of
// any standard, though they are analogous to the DEC VT200 DECELR locator
// reports.
//
// Normal tracking mode sends an escape sequence on both button press and
// release. Modifier key (shift, ctrl, meta) information is also sent. It is
// enabled by specifying parameter 1000 to DECSET. On button press or release,
// xterm sends CSI M C b C x C y . The low two bits of C b encode button
// information: 0=MB1 pressed, 1=MB2 pressed, 2=MB3 pressed, 3=release. The next
// three bits encode the modifiers which were down when the button was pressed
// and are added together: 4=Shift, 8=Meta, 16=Control. Note however that the
// shift and control bits are normally unavailable because xterm uses the
// control modifier with mouse for popup menus, and the shift modifier is used
// in the default translations for button events. The Meta modifier recognized
// by xterm is the mod1 mask, and is not necessarily the "Meta" key (see
// xmodmap). C x and C y are the x and y coordinates of the mouse event, encoded
// as in X10 mode.
//
// Wheel mice may return buttons 4 and 5. Those buttons are represented by the
// same event codes as buttons 1 and 2 respectively, except that 64 is added to
// the event code. Release events for the wheel buttons are not reported.
//
// Button-event tracking is essentially the same as normal tracking, but xterm
// also reports button-motion events. Motion events are reported only if the
// mouse pointer has moved to a different character cell. It is enabled by
// specifying parameter 1002 to DECSET. On button press or release, xterm sends
// the same codes used by normal tracking mode. On button-motion events, xterm
// adds 32 to the event code (the third character, C b ). The other bits of the
// event code specify button and modifier keys as in normal mode. For example,
// motion into cell x,y with button 1 down is reported as CSI M @ C x C y .
// ( @ = 32 + 0 (button 1) + 32 (motion indicator) ). Similarly, motion with
// button 3 down is reported as CSI M B C x C y . ( B = 32 + 2 (button 3) +
// 32 (motion indicator) ).
//
// Event:
//    <Escape> [ M b X Y
//
//    b: wmCMSxx
//         w - wheel
//         m - motion indicator
//         C - Ctrl
//         M - Meta
//         S - Shift
//    X: 32 + col
//    Y: 32 + row
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
    std::cout << "# " << sequence[0]
              << ' '  << sequence[1]
              << ' '  << sequence[2]
              << ' '  << sequence[3]
              << ' '  << sequence[4]
              << ' '  << sequence[5]
              << " = "
              << (sequence[3] & 0x0040 ? 'w' : '.')  // Wheel (button 4, 5)
              << (sequence[3] & 0x0020 ? 'm' : '.')  // Motion
              << (sequence[3] & 0x0010 ? 'C' : '.')  // Ctrl
              << (sequence[3] & 0x0008 ? 'M' : '.')  // Meta
              << (sequence[3] & 0x0004 ? 'S' : '.')  // Shift
              << (sequence[3] & 0x0002 ? 1 : 0)
              << (sequence[3] & 0x0001 ? 1 : 0)
              << std::endl;
*/

    // Capture bits.
    bool wheel   = sequence[3] & 0x40 ? true : false;
    bool motion  = sequence[3] & 0x20 ? true : false;
    bool control = sequence[3] & 0x10 ? true : false;
    bool meta    = sequence[3] & 0x08 ? true : false;
    bool shift   = sequence[3] & 0x04 ? true : false;

    // Combined button and motion selection.
    int key;
    if (motion)
    {
      switch (sequence[3] & 0x03)
      {
      case 0: key = (wheel ? IAPI_MOUSE_4_MOVE : IAPI_MOUSE_1_MOVE); break;
      case 1: key = (wheel ? IAPI_MOUSE_5_MOVE : IAPI_MOUSE_2_MOVE); break;
      case 2: key = IAPI_MOUSE_3_MOVE; break;
      case 3: key = IAPI_MOUSE_RELEASE; break;
      }
    }
    else
    {
      switch (sequence[3] & 0x03)
      {
      case 0: key = (wheel ? IAPI_MOUSE_4_CLICK : IAPI_MOUSE_1_CLICK); break;
      case 1: key = (wheel ? IAPI_MOUSE_5_CLICK : IAPI_MOUSE_2_CLICK); break;
      case 2: key = IAPI_MOUSE_3_CLICK; break;
      case 3: key = IAPI_MOUSE_RELEASE; break;
      }
    }

    // Coordinates.
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
static void blocking (int fd)
{
  int flags = fcntl (fd, F_GETFL, 0);
  fcntl (fd, F_SETFL, flags & ~O_NONBLOCK);
}

////////////////////////////////////////////////////////////////////////////////
static void non_blocking (int fd)
{
  int flags = fcntl (fd, F_GETFL, 0);
  fcntl (fd, F_SETFL, flags | O_NONBLOCK);
}

////////////////////////////////////////////////////////////////////////////////

