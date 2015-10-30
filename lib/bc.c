
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

#include <stdlib.h>

#include "bc.h"
#include "drag.h"

/* Function implementations */
lpBC bc_create(void)
{
  lpBC bc;
  bc = malloc(BC_SIZE);
  bc->bc = 0.65;
  bc_setdragfunc(bc, g1);
  return bc;
}

void bc_destroy(lpBC bc)
{
  free(bc);
}

double bc_getbc(lpBC bc, double mach)
{
  return bc->bc;
}

double bc_getdrag(lpBC bc, double mach)
{
  return BC_PIR*bc->drag(mach)/bc_getbc(bc, mach);
}

void bc_setdragfunc(lpBC bc, enum DRAGFUNC dragfunc)
{
  bc->dragfunc = dragfunc;
  switch (bc->dragfunc)
  {
    case g1:
      bc->drag = drag_cd_g1;
      break;
    case g2:
      bc->drag = drag_cd_g2;
      break;
    case g5:
      bc->drag = drag_cd_g5;
      break;
    case g6:
      bc->drag = drag_cd_g6;
      break;
    case g7:
      bc->drag = drag_cd_g7;
      break;
    case g8:
      bc->drag = drag_cd_g8;
      break;
    case gl:
      bc->drag = drag_cd_gl;
      break;
    case gi:
      bc->drag = drag_cd_gi;
      break;
  }
}
