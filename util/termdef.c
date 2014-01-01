////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2014, Göteborg Bit Factory.
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
