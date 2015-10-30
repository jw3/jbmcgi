

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


#ifndef _MCDRAG_H_
#define _MCDRAG_H_

#include "atmos.h"
#include "drag.h"

#define DRAG_VELCOUNT (15)

enum NOSETYPE {secant, tangent, conical};

static char *nosetypes[] = {"Secant", "Tangent", "Conical"};

enum BOUNDARYLAYER {laminar_laminar, laminar_turbulent, turbulent_turbulent};

static double velocities[] =
  {500.0,  700.0,  900.0,  950.0, 1000.0, 1050.0, 1100.0, 1150.0,
   1200.0, 1500.0, 2000.0, 2500.0, 3000.0, 4000.0, 4500.0};

typedef struct 
{
  double velocity;
  double mach;
  double cd_nose;
  double cd_bttl;
  double cd_base;
  double cd_rbnd;
  double cd_skin;
  double cd_total;
  double form;
  double bc;
  double twt;
  double stab;
  double rpm;
} McDragData, *lpMcDragData;

typedef struct
{
  double  dia;         /* {caliber} */
  double  nsl;         /* {nose length} */
  double  dmp;         /* {meplat diameter} */
  double  rad;         /* {nose radius} */
  double  btl;         /* {boattail length} */
  double  dba;         /* {base diameter} */
  double  tln;         /* {total length} */
  double  dbd;         /* {Rotating band diameter} */
  double  wgt;         /* {weight in grains} */
  double  twt;         /* {twist} */
  double  spg;         /* {Temporary specific gravity} */
  double  sds;         /* {sectional density} */
  enum    DRAGFUNC dgf;      /* {Drag function} */
  enum    NOSETYPE nst;
  enum    BOUNDARYLAYER blc;
  int     alc;
  int     shc;
  lpAtmos atmos;
  double  rtr;         /* {ratio of tangent to actual nose radius} */
  double  min_twt;
  double  max_twt;
  double  min_stab;
  double  max_stab;
  double  (*drag)(double mach);
  McDragData data[DRAG_VELCOUNT];
} McDrag, *lpMcDrag;

lpMcDrag mcdrag_create(void);
double mcdrag_getrtr(lpMcDrag mcdrag);
double mcdrag_cd_nose(lpMcDrag mcdrag, double mach);
double mcdrag_cd_bttl(lpMcDrag mcdrag, double mach);
double mcdrag_cd_base(lpMcDrag mcdrag, double mach);
double mcdrag_cd_rbnd(lpMcDrag mcdrag, double mach);
double mcdrag_cd_skin(lpMcDrag mcdrag, double mach);
double mcdrag_cd_total(lpMcDrag mcdrag, double mach);
double mcdrag_twist(lpMcDrag mcdrag, double mach, double *stability);
double mcdrag_rpm(double twist, double velocity);
void mcdrag_setdrag(lpMcDrag mcdrag);
void mcdrag_calculate(lpMcDrag mcdrag);

#endif /* _MCDRAG_H_ */
