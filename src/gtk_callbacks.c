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

#include "gtk_callbacks.h"
#include "gtk_interface.h"
#include "gtk_support.h"

#include "knocker_core.h"
#include "knocker_args.h"
#include "knocker_log.h"
#include "license.h"
#include "global.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>


/* global variables */
gint  scan_open_ports   = 0;
gint  scan_current_port = 0;
gint  scan_start_port   = 0;
gint  scan_end_port     = 0;
gint  scan_active; /* If a port scan is already running */
gchar *scan_hostname;
char  *scan_port_string;
guint scan_timeout_tag;



gint gtk_knocker_port_scan      (gpointer user_data);
void gtk_knocker_port_scan_stop (gpointer user_data);
void gtk_knocker_quit           (gpointer user_data);



/*
   ============================================================================
   ============================================================================
*/
void gtk_knocker_quit (gpointer user_data)
{
 if (scan_active)
   gtk_knocker_port_scan_stop (user_data);

 gtk_main_quit();
}

/*
   ============================================================================
   ============================================================================
*/

void gtk_knocker_port_scan_stop (gpointer user_data)
{
  GtkWidget *scan_progressbar, *text_output;

  char *tmpbuf;

  if (!scan_active)
     return;


  scan_progressbar = lookup_widget (user_data, "scan_progressbar");
  text_output      = lookup_widget (user_data, "text_output_view");

  gtk_timeout_remove(scan_timeout_tag);

  tmpbuf = malloc (40);

  gtk_progress_set_value (GTK_PROGRESS (scan_progressbar), 0);
  gtk_progress_set_show_text( GTK_PROGRESS (scan_progressbar), FALSE );


  if(scan_port_string != NULL)
    free(scan_port_string);

  scan_active = 0;

  if ( scan_current_port != scan_end_port )
    {
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n> Port scan on host: ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, knocker_core_last_hostip, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " stopped.\n", -1);

    }
  else if ( scan_current_port == scan_end_port)
    {
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n> Port scan on host: ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, knocker_core_last_hostip, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " completed.\n", -1);

    }
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Scanned ", -1);
      sprintf(tmpbuf, "%d", scan_current_port - scan_start_port);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, tmpbuf, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " ports of ", -1);
      sprintf(tmpbuf, "%d", scan_end_port - scan_start_port);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, tmpbuf, -1);

      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n> Found ", -1);
      sprintf(tmpbuf, "%d", scan_open_ports);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, tmpbuf, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " open ports.\n", -1);

  knocker_log_results (knocker_core_last_hostname, knocker_core_last_hostip, scan_current_port - scan_start_port, scan_open_ports, NULL);

  knocker_log_close ();

  free(tmpbuf);

  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Ready.\n\n", -1);
}


/*
   ============================================================================
   ============================================================================
*/
gint gtk_knocker_port_scan (gpointer user_data)
{
  GtkWidget *scan_progressbar, *results_clist, *text_output;
  float temp;

  char *clist_row[2][2];

  scan_progressbar = lookup_widget (user_data, "scan_progressbar");
  results_clist    = lookup_widget (user_data, "results_clist");
  text_output      = lookup_widget (user_data, "text_output_view");

  //printf("scanning port: %d\n", scan_current_port);

  temp = (float) scan_current_port / (float) scan_end_port ;
  temp = temp * 100;

  gtk_progress_set_value (GTK_PROGRESS (scan_progressbar), (int)temp);

  //printf("percentage: %f\n", temp);

  scan_active = 1;

  if (knocker_core_portscan_tcp_connnect (&pscan_data, scan_current_port) == PORT_OPEN)
    {
      scan_open_ports++;

      knocker_core_get_service (scan_current_port, PROTO_TCP);

      sprintf(scan_port_string, "%d", scan_current_port);

      clist_row[0][0] = knocker_core_last_hostip;
      clist_row[0][1] = scan_port_string;
      clist_row[0][2] = knocker_core_last_service;

      gtk_clist_append(GTK_CLIST (results_clist),  clist_row[0]);

      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Port ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, scan_port_string, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "/", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, "tcp", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, ", ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, knocker_core_last_service, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " * ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, "open", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " * \n", -1);

      knocker_log_open_port (scan_current_port, knocker_core_last_service);
    }


  if (scan_start_port < scan_end_port)
    {
      if (scan_current_port < scan_end_port)
        {
	  scan_current_port++;
        }
      else if (scan_current_port == scan_end_port)
        {
	  gtk_knocker_port_scan_stop(user_data);
        }
    }
  else if (scan_start_port > scan_end_port)
    {
      if (scan_current_port > scan_end_port)
        {
	  scan_current_port--;
        }
      else if (scan_current_port == scan_start_port)
        {
	  gtk_knocker_port_scan_stop(user_data);
        }
    }

   return 1;
}


/*
   ============================================================================
   ============================================================================
*/

void
on_button_stop_the_scan_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_knocker_port_scan_stop(user_data);
}


/*
   ============================================================================
   ============================================================================
*/
void
on_button_start_the_scan_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *host_entry, *start_port_entry, *end_port_entry,
            *logfile_entry, *logfile_check_b, *results_clist,
	    *text_output, *scan_progressbar;

  char *tmpbuf;

  if (scan_active)
    return;

  tmpbuf = malloc(40);

  host_entry       = lookup_widget (user_data, "host_entry");
  start_port_entry = lookup_widget (user_data, "start_port_n_spinbutton");
  end_port_entry   = lookup_widget (user_data, "stop_port_n_spinbutton");
  logfile_entry    = lookup_widget (user_data, "logfile_entry");
  logfile_check_b  = lookup_widget (user_data, "logfile_checkbutton");
  results_clist    = lookup_widget (user_data, "results_clist");
  text_output      = lookup_widget (user_data, "text_output_view");
  scan_progressbar = lookup_widget (user_data, "scan_progressbar");


  scan_hostname    = gtk_entry_get_text(GTK_ENTRY( host_entry ));
  scan_start_port  = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (start_port_entry));
  scan_end_port    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (end_port_entry));

  if (GTK_TOGGLE_BUTTON ( logfile_check_b )->active)
    {
	/* If control reaches here, the toggle button is down */
	temp_workaround_args.lfname  = realloc ( temp_workaround_args.lfname, strlen(gtk_entry_get_text(GTK_ENTRY(logfile_entry))) + 1);
	strcpy(temp_workaround_args.lfname,gtk_entry_get_text(GTK_ENTRY(logfile_entry)));
	temp_workaround_args.logfile = TRUE;
    }
  else
    {
      temp_workaround_args.logfile = FALSE;
    }

  knocker_log_open (temp_workaround_args.lfname);

  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Resolving host: ", -1);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, scan_hostname, -1);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " ... ", -1);

  if (knocker_core_resolve_host (&pscan_data, scan_hostname) == NULL)
    {
      //knocker_output_resolve_error (knocker_args.hname);
      //knocker_log_resolve_error (knocker_args.hname);
      //quit();
      //gtk_main_quit(EXIT_FAILURE);

      /* spit out error then returns */

      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "failed.\n", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Couldn't resolve host: ", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, scan_hostname, -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n> Aborting...\n", -1);
      gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Ready.\n\n", -1);

      knocker_log_resolve_error (scan_hostname);

      return;
    }

  knocker_core_get_hostname (&pscan_data);
  knocker_core_get_hostip   (&pscan_data);

  gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, knocker_core_last_hostip, -1);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n", -1);

  knocker_log_host_info (knocker_core_last_hostname ,knocker_core_last_hostip);

  /* clear the content of the results clist */
  gtk_clist_clear(GTK_CLIST (results_clist));

  /* clears the content of the text_output_view */
  /* gtk_text_backward_delete(GTK_TEXT (text_output), gtk_text_get_length(GTK_TEXT (text_output))); */


  if (scan_start_port < scan_end_port)
    {
      scan_current_port = scan_start_port;
    }
  else if (scan_start_port > scan_end_port)
    {
      scan_current_port = scan_start_port;
    }


  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "> Scanning from port: ", -1);
  sprintf(tmpbuf, "%d", scan_start_port);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, tmpbuf, -1);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, " to port: ", -1);
  sprintf(tmpbuf, "%d", scan_end_port);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cwhite, &cblack, tmpbuf, -1);
  free(tmpbuf);
  gtk_text_insert (GTK_TEXT (text_output), NULL, &cgreen, &cblack, "\n", -1);


   knocker_log_ports_info (scan_start_port, scan_end_port);
/*
 %p - percentage
 %v - value
 %l - lower range value
 %u - upper range value
*/

  gtk_progress_set_show_text( GTK_PROGRESS (scan_progressbar), TRUE );


  gtk_progress_set_value (GTK_PROGRESS (scan_progressbar), 0);

  scan_active = 1;
  scan_port_string = malloc (40);

  /* timeout = 125;  1/8 a second  */
  scan_timeout_tag = gtk_timeout_add(10, gtk_knocker_port_scan, user_data);
}

/*
   ============================================================================
   ============================================================================
*/

void
on_menubar_file_quit_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_knocker_quit (user_data);
}


/*
   ============================================================================
   ============================================================================
*/
void
on_help_about_menubar_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *about_window, *license_text_view;

  about_window = create_about_window ();

  license_text_view = lookup_widget (about_window, "license_text");

  /* assure that text widget is realized before setting background: */
  if(!GTK_WIDGET_REALIZED(license_text_view ))
    gtk_widget_realize(license_text_view );

  gdk_window_set_background (GTK_TEXT (license_text_view )->text_area, &cblack);

  gtk_text_insert (GTK_TEXT (license_text_view), NULL, &cgreen, &cblack, LICENSE , -1);

  set_window_icon (about_window->window, "knocker_ico.xpm");

  gtk_widget_show (about_window);

}


/*
   ============================================================================
   ============================================================================
*/
gboolean
on_main_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_knocker_quit (user_data);

  return FALSE;
}

/*
   ============================================================================
   ============================================================================
*/

void
on_close_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_knocker_quit (user_data);
}


/*
   ============================================================================
   ============================================================================
*/
void
on_about_window_close_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *about_window  = lookup_widget (user_data, "about_window");

   gtk_widget_destroy(about_window);
}


/*
   ============================================================================
   ============================================================================
*/
gboolean
on_about_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
   GtkWidget *about_window  = lookup_widget (user_data, "about_window");

   gtk_widget_destroy(about_window);

  return FALSE;
}

/*
   ============================================================================
   ============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/* This is an internally used function to check if a pixmap file exists. */
static gchar*
check_file_exists                      (const gchar     *directory,
                                        const gchar     *filename)
{
  gchar *full_filename;
  struct stat s;
  gint status;

  full_filename = (gchar*) g_malloc (strlen (directory) + 1
                                     + strlen (filename) + 1);
  strcpy (full_filename, directory);
  strcat (full_filename, G_DIR_SEPARATOR_S);
  strcat (full_filename, filename);

  status = stat (full_filename, &s);
  if (status == 0 && S_ISREG (s.st_mode))
    return full_filename;
  g_free (full_filename);
  return NULL;
}

void set_window_icon (GdkWindow *w, const gchar *filename)
{
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;

  gchar *found_filename = NULL;
  static GList *pixmaps_directories = NULL;
  GList *elem;

  /* We first try any pixmaps directories set by the application. */
  elem = pixmaps_directories;
  while (elem)
    {
      found_filename = check_file_exists ((gchar*)elem->data, filename);
      if (found_filename)
        break;
      elem = elem->next;
    }

  /* If we haven't found the pixmap, try the source directory. */
  if (!found_filename)
    {
      found_filename = check_file_exists ("../pixmaps", filename);
    }

  if (!found_filename)
    {
      g_warning ("Couldn't find pixmap file: %s", filename);
    }


  if(w == NULL)
   {
     printf("error w is null\n");
     return;
   }

  style = gtk_widget_get_default_style();

  if(style == NULL)
   {
     printf("error style is null\n");
     return;
   }

  pixmap = gdk_pixmap_create_from_xpm (
            w, &mask, &style->bg[GTK_STATE_NORMAL], found_filename);

  gdk_window_set_icon(w, NULL, pixmap, mask);
/*      gdk_window_set_icon_name(w, "WMHints Test Icon"); */


        /* Do not unref pixmap and mask. */

	g_free (found_filename);
        return;
}


