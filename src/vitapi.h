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

#ifndef INCLUDED_VITAPI
#define INCLUDED_VITAPI

#include <stdio.h>                      // For size_t

#define IAPI_KEY_MINIMUM   0400         // Lowest value for synthetic key

#define IAPI_KEY_UP        0400
#define IAPI_KEY_DOWN      0401
#define IAPI_KEY_LEFT      0402
#define IAPI_KEY_RIGHT     0403
#define IAPI_KEY_BACKSPACE 0404
#define IAPI_KEY_F1        0410
#define IAPI_KEY_F2        0411
#define IAPI_KEY_F3        0412
#define IAPI_KEY_F4        0413
#define IAPI_KEY_F5        0414
#define IAPI_KEY_F6        0415
#define IAPI_KEY_F7        0416
#define IAPI_KEY_F8        0417
#define IAPI_KEY_F9        0420
#define IAPI_KEY_F10       0421
#define IAPI_KEY_HOME      0422
#define IAPI_KEY_DEL       0423
#define IAPI_KEY_PGUP      0424
#define IAPI_KEY_PGDN      0425
#define IAPI_MOUSE_1_CLICK 0426
#define IAPI_MOUSE_2_CLICK 0427
#define IAPI_MOUSE_3_CLICK 0430
#define IAPI_MOUSE_4_CLICK 0431
#define IAPI_MOUSE_5_CLICK 0432
#define IAPI_MOUSE_1_MOVE  0433
#define IAPI_MOUSE_2_MOVE  0434
#define IAPI_MOUSE_3_MOVE  0435
#define IAPI_MOUSE_4_MOVE  0436
#define IAPI_MOUSE_5_MOVE  0437
#define IAPI_MOUSE_RELEASE 0440         // Release of clicked button

#define IAPI_RESIZE        0441         // Terminal resize
#define IAPI_KEY_MAXIMUM   0441         // Highest value for synthetic key

// color - color API
#define _COLOR_256       0x00200000     // 256-color mode
#define _COLOR_HASBG     0x00100000     // Has background color set
#define _COLOR_HASFG     0x00080000     // Has foreground color set
#define _COLOR_UNDERLINE 0x00040000     // General underline attribute
#define _COLOR_BOLD      0x00020000     // 16-color bold attribute
#define _COLOR_BRIGHT    0x00010000     // 16-color bright background attribute
#define _COLOR_BG        0x0000FF00     // 8-bit background color mask
#define _COLOR_FG        0x000000FF     // 8-bit foreground color mask

#define _COLOR_DEFAULT   0              // Terminal default colors

#ifdef __cplusplus
extern "C"
{
#endif

// color - 16- and 256-color support
typedef int color;

color color_def (const char*);           // Parse definition: "red on white"
const char* color_name (char*, size_t, color); // Convert a color -> description
const char* color_decode (char*, size_t, color); // Convert a color -> bits
color color_upgrade (color);             // Convert 16- to 256-color
color color_downgrade (color);           // Convert 256- to 16-color, with loss
color color_blend (color, color);        // Blend two colors, possible upgrade
const char* color_colorize (char*, size_t, color);
                                         // Colorize a string

// iapi - input processing API
int  iapi_initialize ();                 // Initialize for processed input
void iapi_deinitialize ();               // End of processed input
void iapi_echo ();                       // Enable echo
void iapi_noecho ();                     // Disable echo
void iapi_raw ();                        // Enable raw mode
void iapi_noraw ();                      // Disable raw mode
void iapi_mouse ();                      // Enable mouse clicks
void iapi_nomouse ();                    // Disable mouse clicks
void iapi_mouse_tracking ();             // Enable mouse clicks and tracking
void iapi_nomouse_tracking ();           // Disable mouse clicks and tracking
void iapi_mouse_pos (int*, int*);        // Get last known mouse position
int  iapi_mouse_control ();              // Ctrl key?
int  iapi_mouse_meta ();                 // Meta key?
int  iapi_mouse_shift ();                // Shift key?
int  iapi_getch ();                      // Get processed input
int  iapi_set_delay (int);               // Delay between related keypresses

// vapi - visual primitives API
int  vapi_initialize ();                 // Initialize visual processing
void vapi_deinitialize ();               // End of visual processing
int  vapi_refresh ();                    // Update the display
int  vapi_discard ();                    // Discard accumulated output
void vapi_full_screen ();                // Use the full screen
void vapi_end_full_screen ();            // End use of full screen
void vapi_clear ();                      // Clear the screen
void vapi_moveto (int, int);             // Move cursor
void vapi_text (const char*);            // Draw text at cursor
void vapi_color_text (color, const char*);
                                         // Draw colored text at cursor
void vapi_pos_text (int, int, const char*);
                                         // Draw text at position
void vapi_pos_color_text (int, int, color, const char*);
                                         // Draw colored text at position
void vapi_rectangle (int, int, int, int, color);
                                         // Draw a colored rectangle
int  vapi_width ();                      // Get the terminal width
int  vapi_height ();                     // Get the terminal height
void vapi_title (const char*);           // Set the terminal title

// tapi - terminal API
int  tapi_initialize (const char*);      // Initialize terminal caps
void tapi_add (const char*, const char*);
                                         // Provide new terminal configuration
const char* tapi_get (const char*, char*, size_t);
                                         // Get control string
const char* tapi_get_xy (const char*, char*, size_t, int, int);
                                         // Get control string with x,y subst
const char* tapi_get_str (const char*, char*, size_t, const char*);
                                         // Get control string with string subst

// error messages.
int vitapi_error (char*, size_t);        // Obtain last error

#ifdef __cplusplus
};
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
