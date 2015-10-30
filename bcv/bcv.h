
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


#ifndef _BCV_H_
#define _BCV_H_

#include "atmos.h"
#include "drag.h"

#define BCV_NSTEPS (200)
#define BCV_ERROR  (0.0002)
#define BCV_START  (4.0)

typedef struct
{
  double  nvel;
  double  fvel;
  double  dist;
  enum    DRAGFUNC dragfunc;      /* {Drag function} */
  double  (*drag)(double);
  int     alc;
  lpAtmos atmos;
  double  bc;
  double  time;
} BCV, *lpBCV;

#define BCV_SIZE  (sizeof(BCV))

lpBCV bcv_create(void);
void bcv_destroy(lpBCV bcv);
void bcv_setdrag(lpBCV bcv);
double bcv_totaldrag(lpBCV bcv, double velocity, double mach);
int bcv_getbc(lpBCV bcv);
int bcv_calculate(lpBCV bcv);

#endif /* _BCV_H_ */
