
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

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include <math.h>

#define DOT_X(u, v)      ((u.x)*(v.x))
#define DOT_Y(u, v)      ((u.y)*(v.y))
#define DOT_Z(u, v)      ((u.z)*(v.z))
#define DOT(u, v)        (DOT_X(u, v) + DOT_Y(u, v) + DOT_Z(u, v))
#define MODULUS(v)       (DOT(v, v))
#define LENGTH(v)        (sqrt(MODULUS(v)))
#define CROSS_X(u, v)    ((u.y)*(v.z) - (u.z)*(v.y))
#define CROSS_Y(u, v)    ((u.z)*(v.x) - (u.x)*(v.z))
#define CROSS_Z(u, v)    ((u.x)*(v.y) - (u.y)*(v.x))
#define CROSS(u, v)      (vector(CROSS_X(u, v), CROSS_Y(u, v), CROSS_Z(u, v)))
#define STRIPLE(u, v, w) (DOT((u), CROSS((v), (w))))
#define VTRIPLE(u, v, w) (CROSS((u), CROSS((v), (w))))
#define MULTIPLY(a, v)   (vector((a)*(v.x), (a)*(v.y), (a)*(v.z)))
#define ADD(u, v)        (vector((u.x)+(v.x), (u.y)+(v.y), (u.z)+(v.z)))
#define SUBTRACT(u, v)   (vector((u.x)-(v.x), (u.y)-(v.y), (u.z)-(v.z)))
#define REVERSE(u)       (vector(-u.x, -u.y, -u.z))
#define DISTANCE(u, v)   (LENGTH(SUBTRACT(u, v)))

#define NULL_VECTOR      (vector(0.0, 0.0, 0.0))

#define PRINTVECTOR(v)   (printf("%f %f %f\n", v.x, v.y, v.z))

typedef struct
{
  double x;
  double y;
  double z;
} Vector, *lpVector;

/* function prototypes... */
Vector vector(double, double, double);
Vector vector_normalize(Vector);

#endif /* _VECTORS_H_ */
