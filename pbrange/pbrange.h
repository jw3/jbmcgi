
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

#ifndef _PBRANGE_H_
#define _PBRANGE_H_

#include <stdio.h>

#include "bc.h"
#include "atmos.h"
#include "drag.h"

#define  PBR_DX                (3.0)
#define  PBR_ABSMAXVEL         (5000.0)
#define  PBR_ABSMINVX          (50.0)
#define  PBR_MAXIMUM_ERROR     (0.02)
#define  PBR_MAXIMUM_ITCNT     (20)

#define  PBR_GM                (-32.17)
#define  PBR_GRAVITY           (vector(0.0, PBR_GM, 0.0))
#define  PBR_ENERGY(w, v)      (0.5*w*v*v/(-PBR_GM*7000.0))

typedef struct
{ 
  double  muzzle_velocity;
  double  bullet_weight;
  double  vital_zone;
  double  sight_height;
  double  muzzle_energy;
  double  terminal_energy;
  double  pbrange;
  double  pbzero;
  int     found;
  int     alc;
  lpAtmos atmos;
  lpBC    bc;
} PointBlankRange, *lpPointBlankRange;

#define PBR_POINTBLANKRANGE_SIZE  (sizeof(PointBlankRange))

/* Function prototypes */
lpPointBlankRange pointblankrange_create(void);
void pointblankrange_destroy(lpPointBlankRange);
int pointblankrange_calculate(lpPointBlankRange);

#endif /* _PBRANGE_H_ */
