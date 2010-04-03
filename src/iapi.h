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

#ifndef INCLUDED_IAPI
#define INCLUDED_IAPI

#define IAPI_KEY_MINIMUM   0400             // Lowest value for synthetic key.

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
#define IAPI_MOUSE_RELEASE 0431

#define IAPI_KEY_MAXIMUM   0500             // Highest value for synthetic key.

#define IAPI_KEY_ERR       IAPI_KEY_MAXIMUM // When something goes wrong.

bool iapi_initialize ();
void iapi_deinitialize ();

void iapi_echo ();
void iapi_noecho ();

void iapi_raw ();
void iapi_noraw ();

void iapi_mouse ();
void iapi_nomouse ();
void iapi_mouse_tracking ();
void iapi_nomouse_tracking ();
void iapi_mouse_pos (int&, int&);

int  iapi_getch ();

#endif

////////////////////////////////////////////////////////////////////////////////
