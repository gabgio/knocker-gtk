/* gtk-knocker version 0.6.6
 * Release date: 31 March 2002
 *
 * Project homepage: http://knocker.sourceforge.net
 *
 * Copyright 2001,2002 Gabriele Giorgetti <g.gabriele79@genie.it>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "gtk_interface.h"
#include "gtk_callbacks.h"
#include "gtk_support.h"

#include "knocker_args.h"
#include "knocker_core.h"
#include "global.h"

/* COMMAND LINE ARGUMENTS DEFINITION  */
/* ********************************** */
#define HELP_SHORT_OPT    "-h"
#define HELP_LONG_OPT     "--help"

#define VERSION_SHORT_OPT "-v"
#define VERSION_LONG_OPT  "--version"
/* ********************************** */


void print_version_info (void)
{
  fprintf (stdout, "%s %s (%s)\n", PACKAGE, VERSION, RELEASE_DATE);
  fprintf (stdout, "\n");
  fprintf (stdout, "Written by Gabriele Giorgetti %s\n", _KNOCKER_CORE_AUTHOR_EMAIL_);
  fprintf (stdout, "Copyright (C) 2001, 2002 Gabriele Giorgetti\n");
  fprintf (stdout, "\n");
  fprintf (stdout, "This software is released under the GNU GPL\n");
  fprintf (stdout, "\n");
  fprintf (stdout, "%s can be found at %s\n", PACKAGE, _KNOCKER_CORE_HOMEPAGE_ );
}


void print_help_info (void)
{
  fprintf (stdout, "%s, the net portscanner. Version %s (%s)\n", PACKAGE, VERSION, RELEASE_DATE);
  fprintf (stdout, "\n");
  fprintf (stdout, "Usage: %s\n", PACKAGE);
  fprintf (stdout, "\n");
  fprintf (stdout, "      %s,  %s             display this help and exit\n", HELP_SHORT_OPT, HELP_LONG_OPT);
  fprintf (stdout, "      %s,  %s          output version information and exit\n", VERSION_SHORT_OPT, VERSION_LONG_OPT);
  fprintf (stdout, "\n");
  fprintf (stdout, "SEE THE MAN PAGE FOR MORE DESCRIPTIONS, AND EXAMPLES\n");
  fprintf (stdout, "Report bugs to %s.\n", _KNOCKER_CORE_AUTHOR_EMAIL_);
}



int parse_args (int argc, char *argv[])
{
  int i;

  for (i = 1; i <= argc - 1; i++)
    {
      if((!strcmp (argv[i], VERSION_SHORT_OPT))
         || (!strcmp (argv[i], VERSION_LONG_OPT)))
	{
	  print_version_info ();
	  exit (0);
	}
      else if((!strcmp (argv[i], HELP_SHORT_OPT))
         || (!strcmp (argv[i], HELP_LONG_OPT)))
        {
	  print_help_info();
	  exit(0);
	}

    }				/* end of for loop */

  return 0;
}


int main (int argc, char *argv[])
{
  GtkWidget *main_window;
  GtkWidget *text_output_view;

  char *tmpbuf;

  parse_args (argc, argv);

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  knocker_args_init (&temp_workaround_args, 0, 0 ,0);


  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
  add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");

  main_window = create_main_window ();

  tmpbuf = malloc (255);

  sprintf(tmpbuf, "%s  %s -- %s", PACKAGE, VERSION, _KNOCKER_CORE_HOMEPAGE_);

  gtk_window_set_title (GTK_WINDOW (main_window), tmpbuf);

  free(tmpbuf);

  /* Get the system color map and allocate the color black */
  cmap = gdk_colormap_get_system();

  cblack.red = COLOR_MIN;
  cblack.green = COLOR_MIN;
  cblack.blue = COLOR_MIN;

  cgreen.red = 10000;
  cgreen.green = 50000;
  cgreen.blue = 10000;

  cwhite.red = COLOR_MAX;
  cwhite.green = COLOR_MAX;
  cwhite.blue = COLOR_MAX;

  if (!gdk_color_alloc(cmap, &cblack)) {
    g_error("couldn't allocate color");
  }
  if (!gdk_color_alloc(cmap, &cgreen)) {
    g_error("couldn't allocate color");
  }
  if (!gdk_color_alloc(cmap, &cwhite)) {
    g_error("couldn't allocate color");
  }

  text_output_view = lookup_widget (main_window, "text_output_view");

  /* assure that text widget is realized before setting background: */
  if(!GTK_WIDGET_REALIZED(text_output_view))
    gtk_widget_realize(text_output_view);

  gdk_window_set_background (GTK_TEXT (text_output_view)->text_area, &cblack);

  /* Thaw the text widget, allowing the updates to become visible */
  gtk_text_thaw (GTK_TEXT (text_output_view));

  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cgreen, &cblack, "> Welcome to ", -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cwhite, &cblack, PACKAGE, -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cgreen, &cblack, " version ", -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cwhite, &cblack, VERSION, -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cgreen, &cblack, " released on ", -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cwhite, &cblack, RELEASE_DATE, -1);
  gtk_text_insert (GTK_TEXT (text_output_view), NULL, &cgreen, &cblack, "\n> Ready.\n\n", -1);

  gtk_widget_show (main_window);

  set_window_icon (main_window->window, "knocker_ico.xpm");

  knocker_core_init ();

  gtk_main ();

  knocker_core_free_portscan_data (&pscan_data);

  knocker_args_free (&temp_workaround_args);

  knocker_core_quit ();

  return 0;
}

