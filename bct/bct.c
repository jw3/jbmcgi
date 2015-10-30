
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
#include "bct.h"
#include "atmos.h"
#include "drag.h"
#include "errors.h"
#include "utility.h"

lpBCT bct_create(void)
{
  lpBCT bct;
  bct = malloc(BCT_SIZE);
  bct->atmos = atmos_create();
  return bct;
}

void bct_destroy(lpBCT bct)
{
  if (bct->atmos) free(bct->atmos);
  free(bct);
}

void bct_setdrag(lpBCT bct)
{
  switch (bct->dragfunc)
  {
    case g1 :
      bct->drag = drag_cd_g1;
      break;
    case g2 :
      bct->drag = drag_cd_g2;
      break;
    case g5 :
      bct->drag = drag_cd_g5;
      break;
    case g6 :
      bct->drag = drag_cd_g6;
      break;
    case g7 :
      bct->drag = drag_cd_g7;
      break;
    case g8 :
      bct->drag = drag_cd_g8;
      break;
    case gl :
      bct->drag = drag_cd_gl;
      break;
    case gi :
      bct->drag = drag_cd_gi;
      break;
  }
}

double bct_totaldrag(lpBCT bct, double velocity, double mach)
{
  return (BC_PIR*velocity*bct->drag(mach));
}

int bct_getbc(lpBCT bct)
{
  double eq, mch, tv, v, dg;
  double dx, t;
  int    i, j, b, valid;

  j = 0;
  valid = 0;

  dx = bct->dist/BCT_NSTEPS;
  bct->bc = 0.5*BCT_BCMAX;

  do
  {
    eq = dx*bct->atmos->density/ATMOS_DENSSTD/bct->bc;
    v = bct->nvel;
    t = 0.0;
    mch = 1.0/bct->atmos->mach;
    for (i = 0; i < BCT_NSTEPS; i++)
    {
      tv = v - 0.5*eq*bct_totaldrag(bct, v, v*mch);
      dg = eq*bct_totaldrag(bct, tv, tv*mch);
      v  = v - dg;
      t += dx/tv;
    }
    b = (fabs(bct->time - t) > 1.0e-06);
    if (b)
    {
      bct->bc *= pow(t/bct->time, 3);
      bct->fvel = v;
    }
    valid = (INSIDE(bct->bc, BCT_BCMIN, BCT_BCMAX) && 
             INSIDE(v, BCT_VMIN, BCT_VMAX) && (++j <= BCT_ITMAX));
    if (!valid) break;
  }
  while (b);
  return (valid);
}

int bct_calculate(lpBCT bct)
{
  bct_setdrag(bct);
  if (bct->alc) atmos_standardalt(bct->atmos);
  else atmos_atmos(bct->atmos);

  if (!bct_getbc(bct)) return ERROR_BCT_INVALIDBC;
  else return 0;
}
