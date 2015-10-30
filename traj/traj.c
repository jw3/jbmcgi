
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
#include "traj.h"
#include "atmos.h"
#include "drag.h"
#include "options.h"
#include "utility.h"
#include "vectors.h"

/* Function Implementations */
lpTrajectory trajectory_create(void)
{
  lpTrajectory trajectory;
  lpOptions  options;
  trajectory = malloc(TRAJ_TRAJECTORY_SIZE); 
  options = options_create();
  options_setoption(options, TRAJ_OPT_ELEV, 1);
  trajectory->gravity = TRAJ_GRAVITY;
  trajectory->wind = vector(0.0, 0.0, 0.0);
  trajectory->options = options;
  trajectory->atmos = atmos_create();
  trajectory->bc = bc_create();
  trajectory->zero = vector(100*TRAJ_DX, 0.0, 0.0);
  return trajectory;
}

void trajectory_destroy(lpTrajectory trajectory)
{
  options_destroy(trajectory->options);
  atmos_destroy(trajectory->atmos);
  bc_destroy(trajectory->bc);
  if (trajectory->ranges) free(trajectory->ranges);
  free(trajectory);
}

Vector trajectory_correctgravity(lpTrajectory trajectory)
{
  double cl, sl, cc, sc;
  cl = cos(trajectory->los_angle);
  sl = sin(trajectory->los_angle);
  cc = cos(trajectory->cant_angle);
  sc = sin(trajectory->cant_angle);

  return vector(TRAJ_GM*sl, TRAJ_GM*cl*cc, -TRAJ_GM*cl*sc);
}

/* return the corrected muzzle velocity */
double trajectory_correctvelocity(lpTrajectory trajectory)
{
  double dx, eq, tv, m, v;
  int i;
  v = trajectory->velocity;
  if (trajectory->chronodist > TRAJ_MINCHRONO)
  {
    dx = -trajectory->chronodist/TRAJ_NSTEPS;
    eq = dx*trajectory->atmos->density/ATMOS_DENSSTD;
    for (i = 0; i < TRAJ_NSTEPS; i++)
    {
      m = v/trajectory->atmos->mach;
      tv = v - 0.5*eq*v*bc_getdrag(trajectory->bc, m);
      m = tv/trajectory->atmos->mach;
      v = v - eq*tv*bc_getdrag(trajectory->bc, m);
      if (v > TRAJ_ABSMAXVEL) break;
    }
  }
  return v;
}

Vector trajectory_correctwind(lpTrajectory trajectory)
{
  double cl, sl, cc, sc, tmp;
  Vector w;
  cl = cos(trajectory->los_angle);
  sl = sin(trajectory->los_angle);
  cc = cos(trajectory->cant_angle);
  sc = sin(trajectory->cant_angle);

  w = trajectory->wind;
  tmp = w.y*cl - w.x*sl;
  w = vector(w.x*cl + w.y*sl, tmp*cc + w.z*sc, w.z*cc - tmp*sc);
  w.x = MPHTOFPS(w.x);
  w.y = MPHTOFPS(w.y);
  w.z = MPHTOFPS(w.z);
  return w;
}

int trajectory_calculate(lpTrajectory trajectory)
{
  lpOptions o;
  Vector dr, r, tv, v, w, g, z;
  double mv, vm, azim, elev, sp, sa;
  double dt, eq, t, mach, drg, err, dy, dz;
  int    i, j, k, itcnt;

  i = (trajectory->range_max - trajectory->range_min + 1) / 
       trajectory->range_inc + 1;

  z = trajectory->zero;
  z = vector(z.x*TRAJ_DX, INTOFT(z.y), INTOFT(z.z));

  trajectory->ranges = malloc(i*TRAJ_RANGE_SIZE);

/* Set values that don't change for the trajectory... */
  o = trajectory->options;
  if (options_getoption(o, TRAJ_OPT_ALTI)) atmos_standardalt(trajectory->atmos);
  atmos_atmos(trajectory->atmos);

  mach = trajectory->atmos->mach;
  eq = trajectory->atmos->density/ATMOS_DENSSTD;
  sp = trajectory->speed;
  sa = trajectory->speed_angle;

/* correct the gravity vectory */
  g = trajectory_correctgravity(trajectory);
  w = trajectory_correctwind(trajectory);

/* Correct the muzzle velocity for chronograph distance if necessary... */
  mv = trajectory_correctvelocity(trajectory);

  if (options_getoption(o, TRAJ_OPT_AZIM)) azim = 0.0;
  else azim = trajectory->azimuth;

  if (options_getoption(o, TRAJ_OPT_ELEV)) elev = 0.0;
  else elev = trajectory->elevation;

  err = 0.0;
  itcnt = 0;
  while (((err > TRAJ_ERROR) && (itcnt < TRAJ_MAXITCNT)) || (itcnt == 0))
  {
    vm = mv;
    t = 0.0;
    r = vector(0.0, -trajectory->sight_height, -trajectory->sight_offset);
    v = MULTIPLY(vm, vector(cos(elev)*cos(azim), sin(elev), cos(elev)*sin(azim)));

    j = 0;
    k = MAX(trajectory->range_max, z.x);
    for (i = 0; i <= k; i++)
    {

      if ((vm < TRAJ_ABSMINVX) || (r.y < TRAJ_ABSMINY)) break;
      if ((i >= trajectory->range_min) &&
          (i <= trajectory->range_max) &&
          (i % trajectory->range_inc == 0))
      {
        trajectory->ranges[j].range    = r.x/TRAJ_DX;
        trajectory->ranges[j].velocity = vm;
        trajectory->ranges[j].energy   = TRAJ_ENERGY(trajectory->weight, vm);
        trajectory->ranges[j].momentum = TRAJ_MOMENTUM(trajectory->weight, vm);
        trajectory->ranges[j].drop     = r.y;
        trajectory->ranges[j].windage  = r.z;
        trajectory->ranges[j].lead     = TRAJ_LEAD(t, sp, sa);
        trajectory->ranges[j].time     = t;
        j++;
      }

      dt  = 0.5*TRAJ_DX/v.x;
      tv  = SUBTRACT(v, w);
      vm  = LENGTH(tv);
      drg = eq*vm*bc_getdrag(trajectory->bc, vm/mach);
      tv  = SUBTRACT(v, MULTIPLY(dt, SUBTRACT(MULTIPLY(drg, tv), g)));

      dt  = TRAJ_DX/tv.x;
      tv  = SUBTRACT(tv, w);
      vm  = LENGTH(tv);
      drg = eq*vm*bc_getdrag(trajectory->bc, vm/mach);
      v   = SUBTRACT(v, MULTIPLY(dt, SUBTRACT(MULTIPLY(drg, tv), g)));

      dr  = vector(TRAJ_DX, v.y*dt, v.z*dt);
      r   = ADD(r, dr);
      vm  = LENGTH(v);
      t   = t + LENGTH(dr)/vm;

/* Correct trajectory for elevation and azimuth... */
      if (fabs(r.x - z.x) < 0.5*TRAJ_DX)
      {
        dy = r.y - z.y;
        dz = r.z - z.z;
        err = 0.0;
        if (options_getoption(o, TRAJ_OPT_ELEV))
        {
          err = err + fabs(dy);
          elev = elev - dy/r.x;
        }
        if (options_getoption(o, TRAJ_OPT_AZIM))
        {
          err = err + fabs(dz);
          azim = azim - dz/r.x;
        }
        if (err > TRAJ_ERROR) break;
      }
    }
    itcnt++;
  }
  trajectory->elevation = elev;
  trajectory->azimuth = azim;
  return 0;
}
