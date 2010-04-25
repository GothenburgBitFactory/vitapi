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

#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
void show (char** p, const char* key)
{
  const char* value = tgetstr ((char*) key, p);
  if (value)
  {
    const char* i;

    printf ("%s:", key);
    for (i = value; *i != '\0'; ++i)
    {
      if (isprint (*i))      printf ("%c", *i);
      else if (*i == '\033') printf ("_E_");
      else                   printf ("%d", (int)*i);
    }

    printf ("\n");
  }
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  char* term = getenv ("TERM");
  printf ("TERM=%s\n", term);

  char buffer[2048];
  if (term && tgetent (buffer, term) > 0)
  {
    char* bufp = buffer;
    show (&bufp, "ku");
    show (&bufp, "kd");
    show (&bufp, "kr");
    show (&bufp, "kl");
    show (&bufp, "k1");
    show (&bufp, "k2");
    show (&bufp, "k3");
    show (&bufp, "k4");
    show (&bufp, "k5");
    show (&bufp, "k6");
    show (&bufp, "k7");
    show (&bufp, "k8");
    show (&bufp, "k9");
    show (&bufp, "k0");
    show (&bufp, "kH");
    show (&bufp, "kb");
    show (&bufp, "kD");
    show (&bufp, "kP");
    show (&bufp, "kN");
    show (&bufp, "ti");
    show (&bufp, "te");
    show (&bufp, "hs");
    show (&bufp, "cl");
  }
  else
    printf ("TERM %s not found.\n", term);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
