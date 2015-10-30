
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

#include "bc.h"
#include "bcv.h"
#include "drag.h"
#include "atmos.h"
#include "errors.h"
#include "utility.h"

lpBCV bcv_create(void)
{
  lpBCV bcv;
  bcv = malloc(BCV_SIZE);
  bcv->atmos = atmos_create();
  return bcv;
}

void bcv_destroy(lpBCV bcv)
{
  if (bcv->atmos) free(bcv->atmos);
  free(bcv);
}

void bcv_setdrag(lpBCV bcv)
{
  switch (bcv->dragfunc)
  {
    case g1 :
      bcv->drag = drag_cd_g1;
      break;
    case g2 :
      bcv->drag = drag_cd_g2;
      break;
   case g5 :
      bcv->drag = drag_cd_g5;
      break;
   case g6 :
      bcv->drag = drag_cd_g6;
      break;
    case g7 :
      bcv->drag = drag_cd_g7;
      break;
    case g8 :
      bcv->drag = drag_cd_g8;
      break;
    case gl :
      bcv->drag = drag_cd_gl;
      break;
    case gi :
      bcv->drag = drag_cd_gi;
      break;
  }
}

double bcv_totaldrag(lpBCV bcv, double velocity, double mach)
{
  return (BC_PIR*velocity*bcv->drag(mach));
}

int bcv_getbc(lpBCV bcv)
{
  double eq, mch, tv, v, dg;
  double dx, dv, dbc, sum;
  int    i;
  dx = bcv->dist/BCV_NSTEPS;

  bcv->bc = BCV_START;
  do
  {
    eq = dx*bcv->atmos->density/ATMOS_DENSSTD/bcv->bc;
    v = bcv->nvel;
    mch = 1.0/bcv->atmos->mach;
    sum = 0.0;
    bcv->time = 0.0;
    for (i = 0; i < BCV_NSTEPS; i++)
    {
  	 tv = v - 0.5*eq*bcv_totaldrag(bcv, v, v*mch);
  	 dg = eq*bcv_totaldrag(bcv, tv, tv*mch);
  	 sum += dg;
  	 v  = v - dg;
  	 bcv->time += dx/tv;
    }
    dv = (sum)/(bcv->nvel - bcv->fvel);
    dbc = (1.0 - dv);
    if (dbc >= BCV_ERROR) bcv->bc *= dv;
  }
  while (dbc >= BCV_ERROR);
  return (bcv->bc < BCV_START);
}

int bcv_calculate(lpBCV bcv)
{
  bcv_setdrag(bcv);
  if (bcv->alc) atmos_standardalt(bcv->atmos);
  else atmos_atmos(bcv->atmos);
  if (!bcv_getbc(bcv)) return ERROR_BCV_INVALIDBC;
  else return 0;
}

