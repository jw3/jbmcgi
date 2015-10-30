
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "sunrise.h"

/* Function Implementations */
lpSunrise sunrise_create(void)
{
  lpSunrise sunrise;
  sunrise = malloc(SUNRISE_SIZE); 
  return sunrise;
}

void sunrise_destroy(lpSunrise sunrise)
{
  free(sunrise);
}

int sunrise_calculate(lpSunrise sunrise)
{
  int y, m, d;
  double lat, lon;

  y = sunrise->year;
  m = sunrise->month;
  d = sunrise->day;
  lat = sunrise->lat;
  lon = sunrise->lon;

/*  sunrise->length = day_length(y, m, d, lon, lat); */
  sunrise->length = day_civil_twilight_length(y, m, d, lon, lat);

/*  sunrise->sr = sun_rise_set(y, m, 
    d, lon, lat, &sunrise->rise, &sunrise->set); */
  sunrise->sr = civil_twilight(y, m, 
    d, lon, lat, &sunrise->rise, &sunrise->set);

  sunrise->rise += sunrise->tz;
  sunrise->set += sunrise->tz;

  if (sunrise->dst)
  {
    if (dst_correction(y, m, d))
    {
      sunrise->rise += 1;
      sunrise->set += 1;
    }
  }

  return 0;
}
