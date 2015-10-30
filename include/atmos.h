
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


#ifndef  _ATMOS_H_
#define  _ATMOS_H_

/* atmospheric constants */
#define  ATMOS_TEMPSTD        59.0         /* Standard Temperature - °F */
#define  ATMOS_PRESSSTD       29.92        /* Standard Pressure    - in Hg */
#define  ATMOS_HUMSTD         0.0          /* Standard Humidity    -  % */
#define  ATMOS_ALTSTD         0.0          /* Standard Altitude    - feet */
#define  ATMOS_MACHSTD        1116.4499    /* Sound Velocity       - ft/s */
#define  ATMOS_DENSSTD        0.076474     /* Standard air density - lbs/ft^3 */

/* atmospheric constants */
#define  ATMOS_TSTDABS        518.67       /* Standard Temperature - °R */
#define  ATMOS_T0             459.67       /* Freezing Point       - °R */
#define  ATMOS_TEMPGRAD       -3.56616e-03 /* Temperature Gradient - °F/ft */
#define  ATMOS_PRESSEXPNT     -5.255876    /* Pressure Exponent    - none */
#define  ATMOS_VV1            49.0223      /* Sound Speed coefficient */
#define  ATMOS_A0             1.24871      /* Vapor Pressure coefficients */
#define  ATMOS_A1             0.0988438    /* . */
#define  ATMOS_A2             0.00152907   /* . */
#define  ATMOS_A3             -3.07031e-06 /* . */
#define  ATMOS_A4             4.21329e-07  /* . */
#define  ATMOS_ETCONV         3.342e-04

/* basic structure to describe atmospheric conditions */
typedef struct 
{
  double temperature;   /* temperature in °F */
  double pressure;      /* pressure in in Hg    */
  double humidity;      /* relative humdity     */
  double altitude;      /* altitude in feet     */
  double mach;          /* mach 1.0 in feet/sec */
  double density;       /* atmospheric density  */
} Atmos, *lpAtmos;

#define ATMOS_SIZE  (sizeof(Atmos))

/* function prototypes */
lpAtmos atmos_create(void);
void atmos_destroy(lpAtmos);
double atmos_settemperature(lpAtmos, double);
double atmos_setpressure(lpAtmos, double);
double atmos_sethumidity(lpAtmos, double);
double atmos_setaltitude(lpAtmos, double);
void atmos_standardatmos(lpAtmos);
void atmos_atmos(lpAtmos);
void atmos_standardalt(lpAtmos);

#endif  /* _ATMOS_H_ */
