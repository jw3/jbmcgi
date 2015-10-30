
/*
  Copyright (C) 2000 James B. Millard, jbm@cybermesa.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
  02111-1307, USA.
*/


#include <stdlib.h>

#include "options.h"

/* Function implementations */

/* Allocate memory and set to zero (no options...) */
lpOptions options_create(void)
{
  lpOptions options;
  options = malloc(OPTIONS_SIZE);
  options->bits = 0;
  return options;
}

/* Free the memory associated with the options */
void options_destroy(lpOptions options)
{
  free(options);
}

/* return 1 if the options is set, else 0 */
int options_getoption(lpOptions options, long option)
{
  return ((options->bits & option) ? 1 : 0);
}

/* if (onoff) then set the option to true, else set it to false */
void options_setoption(lpOptions options, long option, int onoff)
{
  if (onoff) options->bits = options->bits | option;
  else options->bits = options->bits & (~option);
}
