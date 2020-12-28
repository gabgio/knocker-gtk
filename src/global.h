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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

GdkColormap *cmap;

GdkColor cblack;
GdkColor cgreen;
GdkColor cwhite;

#define COLOR_MIN 0
#define COLOR_MAX 65535

#define RELEASE_DATE "31 March 2002"

knocker_core_portscan_data_t pscan_data;
#define temp_workaround_args knocker_args

#endif /* _GLOBAL_H_ */
