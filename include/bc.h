
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

#ifndef _BC_H_
#define _BC_H_

#include "drag.h"

#define BC_PIR         2.08551e-04

typedef struct
{
  double bc;
  enum DRAGFUNC dragfunc;
  double (*drag)(double);
} BC, *lpBC;

#define BC_SIZE   (sizeof(BC))

/* Funciton prototypes */
lpBC bc_create(void);
void bc_destroy(lpBC);
double bc_getbc(lpBC, double);
double bc_getdrag(lpBC, double);
void bc_setdragfunc(lpBC, enum DRAGFUNC);

#endif /* _BC_H_ */
