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
    // TODO "./color all"                       -> rainbow
    printf ("rainbow\n");
    printf ("\nBasic colors\n");

    char sample[128];
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
    printf ("\n\n");

    printf ("\nBasic colors\n");
/*
        out << std::endl
            << "Basic colors"
            << std::endl
            << " " << Color::colorize (" black ",   "black")
            << " " << Color::colorize (" red ",     "red")
            << " " << Color::colorize (" blue ",    "blue")
            << " " << Color::colorize (" green ",   "green")
            << " " << Color::colorize (" magenta ", "magenta")
            << " " << Color::colorize (" cyan ",    "cyan")
            << " " << Color::colorize (" yellow ",  "yellow")
            << " " << Color::colorize (" white ",   "white")
            << std::endl
            << " " << Color::colorize (" black ",   "white on black")
            << " " << Color::colorize (" red ",     "white on red")
            << " " << Color::colorize (" blue ",    "white on blue")
            << " " << Color::colorize (" green ",   "black on green")
            << " " << Color::colorize (" magenta ", "black on magenta")
            << " " << Color::colorize (" cyan ",    "black on cyan")
            << " " << Color::colorize (" yellow ",  "black on yellow")
            << " " << Color::colorize (" white ",   "black on white")
            << std::endl
            << std::endl;

        out << "Effects"
            << std::endl
            << " " << Color::colorize (" red ",               "red")
            << " " << Color::colorize (" bold red ",          "bold red")
            << " " << Color::colorize (" underline on blue ", "underline on blue")
            << " " << Color::colorize (" on green ",          "black on green")
            << " " << Color::colorize (" on bright green ",   "black on bright green")
            << std::endl
            << std::endl;

        // 16 system colors.
        out << "color0 - color15"
            << std::endl
            << "  0 1 2 . . ."
            << std::endl;
        for (int r = 0; r < 2; ++r)
        {
          out << "  ";
          for (int c = 0; c < 8; ++c)
          {
            std::stringstream s;
            s << "on color" << (r*8 + c);
            out << Color::colorize ("  ", s.str ());
          }

          out << std::endl;
        }

        out << "          . . . 15"
            << std::endl
            << std::endl;

        // Color cube.
        out << "Color cube rgb"
            << Color::colorize ("0", "bold red")
            << Color::colorize ("0", "bold green")
            << Color::colorize ("0", "bold blue")
            << " - rgb"
            << Color::colorize ("5", "bold red")
            << Color::colorize ("5", "bold green")
            << Color::colorize ("5", "bold blue")
            << " (also color16 - color231)"
            << std::endl
            << "  " << Color::colorize ("0            "
                                        "1            "
                                        "2            "
                                        "3            "
                                        "4            "
                                        "5", "bold red")
            << std::endl
            << "  " << Color::colorize ("0 1 2 3 4 5  "
                                        "0 1 2 3 4 5  "
                                        "0 1 2 3 4 5  "
                                        "0 1 2 3 4 5  "
                                        "0 1 2 3 4 5  "
                                        "0 1 2 3 4 5", "bold blue")
            << std::endl;

        char label [12];
        for (int g = 0; g < 6; ++g)
        {
          sprintf (label, " %d", g);
          out << Color::colorize (label, "bold green");
          for (int r = 0; r < 6; ++r)
          {
            for (int b = 0; b < 6; ++b)
            {
              std::stringstream s;
              s << "on rgb" << r << g << b;
              out << Color::colorize ("  ", s.str ());
            }

            out << " ";
          }

          out << std::endl;
        }

        out << std::endl;

        // Grey ramp.
        out << "Gray ramp gray0 - gray23 (also color232 - color255)"
            << std::endl
            << "  0 1 2 . . .                             . . . 23"
            << std::endl
            << "  ";
        for (int g = 0; g < 24; ++g)
        {
          std::stringstream s;
          s << "on gray" << g;
          out << Color::colorize ("  ", s.str ());
        }
*/
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

