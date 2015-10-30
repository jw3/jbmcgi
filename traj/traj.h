
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

#ifndef _TRAJ_H_
#define _TRAJ_H_

#include <math.h>
#include <stdio.h>

#include "bc.h"
#include "atmos.h"
#include "drag.h"
#include "options.h"
#include "vectors.h"

#define  TRAJ_DX               (3.0)
#define  TRAJ_ERROR            (0.02/12.0)
#define  TRAJ_NSTEPS           (100)
#define  TRAJ_MINCHRONO        (0.1)
#define  TRAJ_ABSMAXVEL        (5000.0)
#define  TRAJ_ABSMINVX         (50.0)
#define  TRAJ_ABSMINY          (-1999.9/12.0)
#define  TRAJ_MAXRANGE         (2000)
#define  TRAJ_MAXITCNT         (10)

#define  TRAJ_OPT_AZIM         0x01
#define  TRAJ_OPT_ELEV         0x02
#define  TRAJ_OPT_ALTI         0x04
#define  TRAJ_OPT_MOA          0x08
#define  TRAJ_OPT_MIL          0x10
#define  TRAJ_OPT_IN           0x20
#define  TRAJ_OPT_BASIC        0x40

#define  TRAJ_GM               (-32.17)
#define  TRAJ_GRAVITY          (vector(0.0, TRAJ_GM, 0.0))
#define  TRAJ_LEAD(t, s, a)    (sin(a)*(t)*(s))
#define  TRAJ_ENERGY(w, v)     (0.5*(w)*(v)*(v)/(-TRAJ_GM*7000.0))
#define  TRAJ_MOMENTUM(w, v)   ((w)*(v)/(-TRAJ_GM*7000.0))

typedef struct 
{
  double range;
  double velocity;
  double energy;
  double momentum;
  double drop;
  double windage;
  double lead;
  double time;
} Range, *lpRange;

#define  TRAJ_RANGE_SIZE   (sizeof(Range))

typedef struct
{
  double    velocity;
  double    chronodist;
  double    weight;
  double    azimuth;
  double    elevation;
  double    los_angle;
  double    cant_angle;
  double    sight_height;
  double    sight_offset;
  double    speed;
  double    speed_angle;
  int       range_min;
  int       range_max;
  int       range_inc;
  Vector    zero;
  Vector    wind;
  Vector    gravity;
  lpOptions options;
  lpAtmos   atmos;
  lpBC      bc;
  lpRange   ranges;
} Trajectory, *lpTrajectory;

#define TRAJ_TRAJECTORY_SIZE  (sizeof(Trajectory))

/* Function prototypes */
Trajectory *trajectory_create(void);
void trajectory_destroy(lpTrajectory);
Vector trajectory_correctgravity(lpTrajectory);
double trajectory_correctvelocity(lpTrajectory);
Vector trajectory_correctwind(lpTrajectory);
int trajectory_calculate(lpTrajectory);

#endif /* _TRAJ_H_ */
