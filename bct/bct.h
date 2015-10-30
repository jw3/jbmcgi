
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

#ifndef _BCT_H_
#define _BCT_H_

#include <math.h>
#include <stdlib.h>

#include "atmos.h"
#include "drag.h"

#define BCT_NSTEPS   (200)
#define BCT_BCMIN    (0.01)
#define BCT_BCMAX    (4.00)
#define BCT_VMIN     (500.0)
#define BCT_VMAX     (4500.0)

/* 
  Safety value for endless loops.  It should never hit this
  number of iterations...knock on wood.
*/
#define BCT_ITMAX    (500)

typedef struct
{
  double  nvel;
  double  time;
  double  dist;
  enum    DRAGFUNC dragfunc;      /* {Drag function} */
  double  (*drag)(double);
  int     alc;
  lpAtmos atmos;
  double  bc;
  double  fvel;
} BCT, *lpBCT;

#define BCT_SIZE  (sizeof(BCT))

lpBCT bct_create();
void bct_destroy(lpBCT bct);
void bct_setdrag(lpBCT bct);
double bct_totaldrag(lpBCT bct, double velocity, double mach);
int bct_getbc(lpBCT bct);
int bct_calculate(lpBCT bct);

#endif // _BCT_H_


