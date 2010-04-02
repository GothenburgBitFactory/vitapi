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

#ifndef INCLUDED_VAPI
#define INCLUDED_VAPI

#include <string>
#include "Color.h"

bool vapi_initialize ();
void vapi_deinitialize ();

bool vapi_refresh ();
void vapi_full_screen ();
void vapi_end_full_screen ();
void vapi_clear ();
void vapi_moveto (int, int);
void vapi_text (const std::string&);
void vapi_text (const Color&, const std::string&);
void vapi_text (int, int, const std::string&);
void vapi_text (int, int, const Color&, const std::string&);
void vapi_rectangle (int, int, int, int, const Color&);

int vapi_width ();
int vapi_height ();

void vapi_title (const std::string&);

#endif

