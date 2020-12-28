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


#include <gtk/gtk.h>


void on_button_stop_the_scan_clicked (GtkButton *button, gpointer user_data);

void on_button_start_the_scan_clicked (GtkButton *button, gpointer user_data);

void on_menubar_file_quit_activate (GtkMenuItem *menuitem, gpointer user_data);

void on_help_about_menubar_activate (GtkMenuItem *menuitem, gpointer user_data);

gboolean on_main_window_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);

gboolean on_main_window_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);

gboolean on_main_window_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);

void on_close_button_clicked (GtkButton *button, gpointer user_data);

void on_about_window_close_button_clicked (GtkButton *button, gpointer user_data);

gboolean on_about_window_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);


void set_window_icon (GdkWindow *w, const gchar *filename);
