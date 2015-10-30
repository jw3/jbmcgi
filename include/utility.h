

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

#ifndef _UTILITY_H_
#define _UTILITY_H_

#define DEG(X)           (X*180.0/M_PI)
#define RAD(X)           (X*M_PI/180.0)
#define MOA(X)           (60.0*DEG(X))
#define MIL(X)           (X*3200.0/M_PI)
#define SEC(X)           (60.0*MOA(X))
#define FTTOIN(X)        (X*12.0)
#define INTOFT(X)        (X/12.0)
#define YRDTOFT(X)       (X*3.0)
#define YRDTOIN(X)       (12.0*YRDTOFT(X))
#define MPHTOFPS(X)      (X*5280.0/3600.0)
#define FPSTOMPH(X)      (X*3600.0/5280.0)
#define INSIDE(X, L, H)  ((X > L) && (X < H))
#define NOTIN(X, L, H)   ((X > H) || (X < L))
#define MIN(A, B)        ((A) < (B) ? (A) : (B))
#define MAX(A, B)        ((A) > (B) ? (A) : (B))
#define BRACKET(X, L, H) (MAX(MIN(X, H), L))
#define SQR(x)           ((x)*(x))

#define BRACKETDEF(X, L, H, D) ((X) < (L) ? (D) : ((X) > (H) ? (D) : (X)))

#endif /* _UTILITY_H_ */
