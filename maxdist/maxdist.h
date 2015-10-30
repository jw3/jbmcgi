
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

#ifndef _MAXDIST_H_
#define _MAXDIST_H_

#include "bc.h"
#include "atmos.h"

#define  MAXDIST_DX                (2.0*3.0)
#define  MAXDIST_ABSMAXVEL         (5000.0)
#define  MAXDIST_ABSMINVX          (50.0)
#define  MAXDIST_MINIMUM_ELEV      (RAD(10.0))

#define  MAXDIST_GM                (-32.17)
#define  MAXDIST_GRAVITY           (vector(0.0, MAXDIST_GM, 0.0))
#define  MAXDIST_ENERGY(w, v)      (0.5*w*v*v/(-MAXDIST_GM*7000.0))

typedef struct 
{ 
  double  muzzle_velocity;
  double  muzzle_elevation;
  double  weight;
  double  terminal_angle;
  double  terminal_energy;
  double  terminal_range;
  double  terminal_time;
  double  terminal_velocity;
  double  terminal_height;
  double  maximum_height;
  double  maximum_time;
  int     alc;
  lpAtmos atmos;
  lpBC    bc;
} MaxDistance, *lpMaxDistance;

#define MAXDIST_MAXDISTANCE_SIZE  (sizeof(MaxDistance))

/* Function prototypes */

lpMaxDistance maxdistance_create(void);
void maxdistance_destroy(lpMaxDistance);
int maxdistance_calculate(lpMaxDistance);

#endif /* _MAXDIST_H_ */
