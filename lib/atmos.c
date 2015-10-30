
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
#include <stdlib.h>

#include "atmos.h"

/* function implementations */
lpAtmos atmos_create(void)
{
  lpAtmos atmos;
  atmos = malloc(ATMOS_SIZE);
  atmos_standardatmos(atmos);
  return atmos;
}

/* Free the memory associated with the atmos structure */
void atmos_destroy(lpAtmos atmos)
{
  free(atmos);
}

/* Set the temperature */
double atmos_settemperature(lpAtmos atmos, double temperature)
{
  atmos->temperature = temperature;
  return atmos->temperature;
}

/* Set the pressure */
double atmos_setpressure(lpAtmos atmos, double pressure)
{
  atmos->pressure = pressure;
  return atmos->pressure;
}

/* Set the humidity */
double atmos_sethumidity(lpAtmos atmos, double humidity)
{
  atmos->humidity = humidity;
  return atmos->humidity;
}

/* Set the altitude */
double atmos_setaltitude(lpAtmos atmos, double altitude)
{
  atmos->altitude = altitude;
  return atmos->altitude;
}

/* Set the passed structure to the standard ICAO atmosphere at sea level */
void atmos_standardatmos(lpAtmos atmos)
{
  atmos_settemperature(atmos, ATMOS_TEMPSTD);
  atmos_setpressure(atmos, ATMOS_PRESSSTD);
  atmos_sethumidity(atmos, ATMOS_HUMSTD);
  atmos_setaltitude(atmos, ATMOS_ALTSTD);
  atmos_atmos(atmos);
};

/* Adjust the passed ATMOS's density and mach number for temperature, pressure
   and humdity.  */
void atmos_atmos(lpAtmos atmos)
{
  double t, p, hc, et, et0;
  t = atmos->temperature;
  p = atmos->pressure;
  if (t > 0.0)
  {
    et0 = ATMOS_A0 + t*(ATMOS_A1 + t*(ATMOS_A2 + t*(ATMOS_A3 + t*ATMOS_A4)));
    et = ATMOS_ETCONV*atmos->humidity*et0;
    hc = (p - 0.3783*et)/ATMOS_PRESSSTD;
  }
  else hc = 1.0;
  atmos->density = 
    ATMOS_DENSSTD*(ATMOS_TSTDABS/(t + ATMOS_T0))*hc;
  atmos->mach = sqrt(t + ATMOS_T0)*ATMOS_VV1;

};

/* set the ICAO Standard Conditions for the current altitude */
void atmos_standardalt(lpAtmos atmos)
{
  atmos->temperature = ATMOS_TSTDABS + atmos->altitude*ATMOS_TEMPGRAD;
  atmos->pressure = 
    ATMOS_PRESSSTD*pow(ATMOS_TSTDABS/atmos->temperature, ATMOS_PRESSEXPNT);
  atmos->temperature = 
    atmos->temperature - ATMOS_T0; /* line above need absolute! */
  atmos->humidity = ATMOS_HUMSTD;
  atmos_atmos(atmos);
}
