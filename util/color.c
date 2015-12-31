////////////////////////////////////////////////////////////////////////////////
// VITapi - UI helper library that controls Visuals, Input and Terminals.
//
// Copyright 2010 - 2016, Göteborg Bit Factory.
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
#include <string.h>
#include <vitapi.h>

////////////////////////////////////////////////////////////////////////////////
color get_color (const char* def, int quantization)
{
  color c = color_def (def);

  if (quantization == 8 || quantization == 16)
    c = color_downgrade (c, quantization);

  return c;
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  // No arguments - print usage.
  if (argc == 1)
  {
    printf ("\n"
            "Usage: color                                     Shows command usage\n"
            "       color all                                 Shows full color palette, in 256 colors\n"
            "       color all8                                Shows full color palette, quantized to 8 colors\n"
            "       color all16                               Shows full color palette, quantized to 16 colors\n"
            "       color <code>                              Shows color sample by code\n"
            "       color <name>                              Shows color sample by name\n"
            "       color <code>|<name> blend <code>|<name>   Blends two color samples\n"
            "\n"
            "For example:\n"
            "  ./color all\n"
            "  ./color 1638920\n"
            "  ./color white on bright red\n"
            "  ./color white on red blend underline yellow\n"
            "\n");
  }

  // Two arguments, one of which is 'all', so display all available colors.
  else if (argc == 2 && (!strcmp (argv[1], "all")  ||
                         !strcmp (argv[1], "all8") ||
                         !strcmp (argv[1], "all16")))
  {
    char sample[128];
    char def[128];
    int r, g, b, c;
    int q = 256;

    if (!strcmp (argv[1], "all8"))
      q = 8;
    else if (!strcmp (argv[1], "all16"))
      q = 16;

    printf ("\nBasic colors\n");
    strcpy (sample, " black ");   printf (" %s", color_colorize (sample, 128, get_color ("black", q)));
    strcpy (sample, " red ");     printf (" %s", color_colorize (sample, 128, get_color ("red", q)));
    strcpy (sample, " blue ");    printf (" %s", color_colorize (sample, 128, get_color ("blue", q)));
    strcpy (sample, " green ");   printf (" %s", color_colorize (sample, 128, get_color ("green", q)));
    strcpy (sample, " magenta "); printf (" %s", color_colorize (sample, 128, get_color ("magenta", q)));
    strcpy (sample, " cyan ");    printf (" %s", color_colorize (sample, 128, get_color ("cyan", q)));
    strcpy (sample, " yellow ");  printf (" %s", color_colorize (sample, 128, get_color ("yellow", q)));
    strcpy (sample, " white ");   printf (" %s", color_colorize (sample, 128, get_color ("white", q)));
    printf ("\n");
    strcpy (sample, " black ");   printf (" %s", color_colorize (sample, 128, get_color ("white on black", q)));
    strcpy (sample, " red ");     printf (" %s", color_colorize (sample, 128, get_color ("white on red", q)));
    strcpy (sample, " blue ");    printf (" %s", color_colorize (sample, 128, get_color ("white on blue", q)));
    strcpy (sample, " green ");   printf (" %s", color_colorize (sample, 128, get_color ("black on green", q)));
    strcpy (sample, " magenta "); printf (" %s", color_colorize (sample, 128, get_color ("black on magenta", q)));
    strcpy (sample, " cyan ");    printf (" %s", color_colorize (sample, 128, get_color ("black on cyan", q)));
    strcpy (sample, " yellow ");  printf (" %s", color_colorize (sample, 128, get_color ("black on yellow", q)));
    strcpy (sample, " white ");   printf (" %s", color_colorize (sample, 128, get_color ("black on white", q)));

    printf ("\n\nEffects\n");
    strcpy (sample, " red ");                printf (" %s", color_colorize (sample, 128, get_color ("red", q)));
    strcpy (sample, " bold red ");           printf (" %s", color_colorize (sample, 128, get_color ("bold red", q)));
    strcpy (sample, " underline on blue ");  printf (" %s", color_colorize (sample, 128, get_color ("underline on blue", q)));
    strcpy (sample, " on green ");           printf (" %s", color_colorize (sample, 128, get_color ("black on green", q)));
    strcpy (sample, " on bright green ");    printf (" %s", color_colorize (sample, 128, get_color ("black on bright green", q)));

    // 16 system colors.
    printf ("\n\ncolor0 - color15\n  0 1 2 . . .\n");
    for (r = 0; r < 2; ++r)
    {
      printf ("  ");
      for (c = 0; c < 8; ++c)
      {
        sprintf (def, "on color%d", (r*8 + c));
        strcpy (sample, "  ");
        printf ("%s", color_colorize (sample, 128, get_color (def, q)));
      }

      printf ("\n");
    }

    printf ("          . . . 15\n");

    // Color cube.
    printf ("\nColor cube rgb");
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("red", q)));
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("green", q)));
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("blue", q)));
    printf (" - rgb");
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("red", q)));
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("green", q)));
    strcpy (sample, "0");  printf ("%s", color_colorize (sample, 128, get_color ("blue", q)));
    printf (" (also color16 - color231)\n");

    strcpy (sample, "0            "
                    "1            "
                    "2            "
                    "3            "
                    "4            "
                    "5");
    printf ("  %s\n", color_colorize (sample, 128, get_color ("bold red", q)));

    strcpy (sample, "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5");
    printf ("  %s\n", color_colorize (sample, 128, get_color ("bold blue", q)));

    for (g = 0; g < 6; ++g)
    {
      sprintf (sample, " %d", g);
      printf ("%s", color_colorize (sample, 128, get_color ("bold green", q)));

      for (r = 0; r < 6; ++r)
      {
        for (b = 0; b < 6; ++b)
        {
          sprintf (def, "on rgb%d%d%d", r, g, b);
          strcpy (sample, "  ");
          printf ("%s", color_colorize (sample, 128, get_color (def, q)));
        }

        printf (" ");
      }

      printf ("\n");
    }

    // Grey ramp.
    printf ("\nGray ramp gray0 - gray23 (also color232 - color255)\n"
            "  0 1 2 . . .                             . . . 23\n  ");

    for (g = 0; g < 24; ++g)
    {
      sprintf (def, "on gray%d", g);
      strcpy (sample, "  ");
      printf ("%s", color_colorize (sample, 128, get_color (def, q)));
    }

    printf ("\n\n");
  }

  // Arguments comprise either a color definition, a color code, or
  // two colors to be blended.
  else
  {
    char one[128] = {0};
    char two[128] = {0};
    int blend = 0;
    int i;

    // Determine whether there are two colors to blend.
    for (i = 1; i < argc; ++i)
    {
      if (!strcmp (argv[i], "blend"))
      {
        blend = i;
      }
      else
      {
        if (blend == 0)
        {
          if (strlen (one) != 0)
            strcat (one, " ");

          strcat (one, argv[i]);
        }
        else
        {
          if (strlen (two) != 0)
            strcat (two, " ");

          strcat (two, argv[i]);
        }
      }
    }

    // Just the one color.
    if (blend == 0)
    {
      char sample[128];
      char decode[128];
      char name[128];

      color c = color_def (one);
      strcpy (sample, "Color sample: ");
      color_name (name, 128, c);
      strcat (sample, name);

      printf ("\n%s\nCode:   %d\nDecode: %s\n",
              color_colorize (sample, 128, c),
              c,
              color_decode (decode, 128, c));

      if (strcmp (one, name))
        printf ("Original color '%s' interpreted as '%s'\n", one, name);

      printf ("\n");
    }

    // Two colors.
    else
    {
      color c1, c2, c3;
      char sample1[128], sample2[128], sample3[128];
      char decode1[128], decode2[128], decode3[128];
      char name3[128];

      c1 = color_def (one);
      c2 = color_def (two);
      c3 = color_blend (c1, c2);

      sprintf (sample1, "Color sample: %s", one);
      sprintf (sample2, "Blend sample: %s", two);
      sprintf (sample3, "Blended sample: %s", color_name (name3, 128, c3));

      printf ("\n%s\nCode:   %d\nDecode: %s\n",
              color_colorize (sample1, 128, c1),
              c1,
              color_decode (decode1, 128, c1));

      printf ("\n%s\nCode:   %d\nDecode: %s\n",
              color_colorize (sample2, 128, c2),
              c2,
              color_decode (decode2, 128, c2));

      printf ("\n%s\nCode:   %d\nDecode: %s\n\n",
              color_colorize (sample3, 128, c3),
              c3,
              color_decode (decode3, 128, c3));
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
