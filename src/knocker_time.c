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

#include <stdio.h>
#include <stdlib.h>
 
#include "knocker_time.h"

#ifdef __WIN32__
#include <windows.h>
#else
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#endif /* __WIN32__ */

#ifdef __WIN32__
static long timer_start;
#else
static struct timeval timer_start;
#endif


void knocker_time_start_timer (void)
{
#ifdef __WIN32__
  timer_start = GetTickCount();
#else
  /* Set first ticks value */
  gettimeofday (&timer_start, NULL);
#endif
}

double knocker_time_get_ticks (void)
{
  unsigned long ticks;
#ifdef __WIN32__
  long timer_now;

  timer_now = GetTickCount();
  ticks = ( (timer_now - timer_start));

#else
  struct timeval timer_now;

  gettimeofday (&timer_now, NULL);
  ticks =
    (timer_now.tv_sec - timer_start.tv_sec) * 1000 + (timer_now.tv_usec - timer_start.tv_usec) / 1000;
#endif
  return (ticks);
}

