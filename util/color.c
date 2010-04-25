#include <stdio.h>
#include <string.h>
#include <vitapi.h>

int main (int argc, char** argv)
{
  if (argc == 1)
  {
    printf ("\n"
            "Usage: color                                     Shows command usage\n"
            "       color all                                 Shows full color palette\n"
            "       color <code>                              Shows color sample by code\n"
            "       color <name>                              Shows color sample by name\n"
            "       color <code>|<name> blend <code>|<name>   Blends two color samples\n"
            "\n");
  }
  else if (argc == 2 && !strcmp (argv[1], "all"))
  {
    char sample[128];
    char def[128];
    int r, g, b, c;

    printf ("\nBasic colors\n");
    strcpy (sample, " black ");   color_colorize (sample, 128, color_def ("black"));   printf (" %s", sample);
    strcpy (sample, " red ");     color_colorize (sample, 128, color_def ("red"));     printf (" %s", sample);
    strcpy (sample, " blue ");    color_colorize (sample, 128, color_def ("blue"));    printf (" %s", sample);
    strcpy (sample, " green ");   color_colorize (sample, 128, color_def ("green"));   printf (" %s", sample);
    strcpy (sample, " magenta "); color_colorize (sample, 128, color_def ("magenta")); printf (" %s", sample);
    strcpy (sample, " cyan ");    color_colorize (sample, 128, color_def ("cyan"));    printf (" %s", sample);
    strcpy (sample, " yellow ");  color_colorize (sample, 128, color_def ("yellow"));  printf (" %s", sample);
    strcpy (sample, " white ");   color_colorize (sample, 128, color_def ("white"));   printf (" %s", sample);
    printf ("\n");
    strcpy (sample, " black ");   color_colorize (sample, 128, color_def ("white on black"));   printf (" %s", sample);
    strcpy (sample, " red ");     color_colorize (sample, 128, color_def ("white on red"));     printf (" %s", sample);
    strcpy (sample, " blue ");    color_colorize (sample, 128, color_def ("white on blue"));    printf (" %s", sample);
    strcpy (sample, " green ");   color_colorize (sample, 128, color_def ("black on green"));   printf (" %s", sample);
    strcpy (sample, " magenta "); color_colorize (sample, 128, color_def ("black on magenta")); printf (" %s", sample);
    strcpy (sample, " cyan ");    color_colorize (sample, 128, color_def ("black on cyan"));    printf (" %s", sample);
    strcpy (sample, " yellow ");  color_colorize (sample, 128, color_def ("black on yellow"));  printf (" %s", sample);
    strcpy (sample, " white ");   color_colorize (sample, 128, color_def ("black on white"));   printf (" %s", sample);

    printf ("\n\nEffects\n");
    strcpy (sample, " red ");  color_colorize (sample, 128, color_def ("red"));  printf (" %s", sample);
    strcpy (sample, " bold red ");  color_colorize (sample, 128, color_def ("bold red"));  printf (" %s", sample);
    strcpy (sample, " underline on blue ");  color_colorize (sample, 128, color_def ("underline on blue"));  printf (" %s", sample);
    strcpy (sample, " on green ");  color_colorize (sample, 128, color_def ("black on green"));  printf (" %s", sample);
    strcpy (sample, " on bright green ");  color_colorize (sample, 128, color_def ("black on bright green"));  printf (" %s", sample);

    // 16 system colors.
    printf ("\n\ncolor0 - color15\n  0 1 2 . . .\n");
    for (r = 0; r < 2; ++r)
    {
      printf ("  ");
      for (c = 0; c < 8; ++c)
      {
        sprintf (def, "on color%d", (r*8 + c));
        strcpy (sample, "  ");
        color_colorize (sample, 128, color_def (def));
        printf ("%s", sample);
      }

      printf ("\n");
    }

    printf ("          . . . 15\n");

    // Color cube.
    printf ("\nColor cube rgb");
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("red")); printf ("%s", sample);
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("green")); printf ("%s", sample);
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("blue")); printf ("%s", sample);
    printf (" - rgb");
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("red")); printf ("%s", sample);
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("green")); printf ("%s", sample);
    strcpy (sample, "0");  color_colorize (sample, 128, color_def ("blue")); printf ("%s", sample);
    printf (" (also color16 - color231)\n");

    strcpy (sample, "0            "
                    "1            "
                    "2            "
                    "3            "
                    "4            "
                    "5");
    color_colorize (sample, 128, color_def ("bold red"));
    printf ("  %s\n", sample);

    strcpy (sample, "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5  "
                    "0 1 2 3 4 5");
    color_colorize (sample, 128, color_def ("bold blue"));
    printf ("  %s\n", sample);

    for (g = 0; g < 6; ++g)
    {
      sprintf (sample, " %d", g);
      color_colorize (sample, 128, color_def ("bold green"));
      printf ("%s", sample);

      for (r = 0; r < 6; ++r)
      {
        for (b = 0; b < 6; ++b)
        {
          sprintf (def, "on rgb%d%d%d", r, g, b);
          strcpy (sample, "  ");
          color_colorize (sample, 128, color_def (def));
          printf ("%s", sample);
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
      color_colorize (sample, 128, color_def (def));
      printf ("%s", sample);
    }

    printf ("\n");
  }
  else
  {
    // TODO "./color 123456789"                 -> decode sample
    // TODO "./color red on blue [blend green]" -> sample

    color one = atoi (argv[1]);
/*
    char name[128];
    color_

    char sample[256];

    std::cout << c1.colorize ("Color Sample: " + ((std::string) c1))
              << " "
              << (int) c1
              << " "
              << c1.binary ()
              << std::endl;
*/
  }

  return 0;
}

