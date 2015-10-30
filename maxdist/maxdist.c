
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

#include "bc.h"
#include "atmos.h"
#include "drag.h"
#include "utility.h"
#include "vectors.h"
#include "maxdist.h"

/* Function Implementations */
lpMaxDistance maxdistance_create(void)
{
  lpMaxDistance maxdistance;
  maxdistance = malloc(MAXDIST_MAXDISTANCE_SIZE); 
  maxdistance->atmos = atmos_create();
  maxdistance->bc = bc_create();
  return maxdistance;
}

void maxdistance_destroy(lpMaxDistance maxdistance)
{
  atmos_destroy(maxdistance->atmos);
  bc_destroy(maxdistance->bc);
  free(maxdistance);
}

int maxdistance_calculate(lpMaxDistance maxdistance)
{
  Vector dr, r, tv, v, w;
  double mv, vm, tvm, drm, elev, otd, mh;
  double dt, eq, t, mach, drg, dd;
  int    i;

  /* Set values that don't change for the maxdistance... */
  if (maxdistance->alc) 
    atmos_standardalt(maxdistance->atmos);
  else 
    atmos_atmos(maxdistance->atmos);
  mach = maxdistance->atmos->mach;
  eq = maxdistance->atmos->density/ATMOS_DENSSTD;


  dd = 1.0;
  otd = 0.0;
  elev = MAXDIST_MINIMUM_ELEV;
  while (dd > 0.0)
  {
    vm = maxdistance->muzzle_velocity;
    t = 0.0;
    mh = 0.0;
    r = vector(0.0, 0.01, 0.0);
    elev += RAD(1.0);
    v.x = vm*cos(elev);
    v.y = vm*sin(elev);
    v.z = 0.0;

    for (i = 0; r.y > 0.0; i++)
    {

      if (vm < MAXDIST_ABSMINVX) break;

      vm  = LENGTH(v);
      dt  = 0.5*MAXDIST_DX/v.x;
      drg = eq*vm*bc_getdrag(maxdistance->bc, vm/mach);
      tv  = 
        SUBTRACT(v, MULTIPLY(dt, SUBTRACT(MULTIPLY(drg, v), MAXDIST_GRAVITY)));
      vm  = LENGTH(tv);
      dt  = MAXDIST_DX/tv.x;
      drg = eq*vm*bc_getdrag(maxdistance->bc, vm/mach);
      v   = 
        SUBTRACT(v, MULTIPLY(dt, SUBTRACT(MULTIPLY(drg, tv), MAXDIST_GRAVITY)));
      dr  = vector(MAXDIST_DX, v.y*dt, v.z*dt);
      r   = ADD(r, dr);
      t   = t + LENGTH(dr)/vm;
      vm  = LENGTH(v);

      if (r.y > mh) mh = r.y;

    }
    dd = (r.x - otd);
    if (dd > 0.0)
    {
      maxdistance->muzzle_elevation = DEG(elev);
      if (v.x > 0.0) 
	maxdistance->terminal_angle = DEG(fabs(atan(v.y/v.x)));
      else 
	maxdistance->terminal_angle = 90.0;
      maxdistance->terminal_range = r.x/3.0;
      maxdistance->terminal_energy = MAXDIST_ENERGY(maxdistance->weight, vm);
      maxdistance->terminal_time = t;
      maxdistance->terminal_velocity = vm;
      maxdistance->terminal_height = mh;
    }
    otd = r.x;
  }

  /* Now find height if shot straight up... */
  t = 0.0;
  drm = 0.0;
  vm = maxdistance->muzzle_velocity;

  do
  {

    dt  = 0.5*MAXDIST_DX/vm;
    drg = eq*vm*bc_getdrag(maxdistance->bc, vm/mach);
    tvm = vm - dt*(drg*vm - MAXDIST_GM);
    dt  = MAXDIST_DX/tvm;
    drg = eq*tvm*bc_getdrag(maxdistance->bc, tvm/mach);
    vm  = vm - dt*(drg*tvm - MAXDIST_GM);
    drm += MAXDIST_DX;
    t   += MAXDIST_DX/vm;

  }
  while (vm > 0.0);

  maxdistance->maximum_height = drm;
  maxdistance->maximum_time = t;

  return 0;
}
