
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


#ifndef _DRAG_H_
#define _DRAG_H_

enum DRAGFUNC {g1, g2, g5, g6, g7, g8, gl, gi};

static char *dragstr[] =
  {"G1", "G2", "G5", "G6", "G7", "G8", "GL", "GI"};

/* Drag functions */
double drag_cd_g1(double mach);
double drag_cd_g2(double mach);
double drag_cd_g5(double mach);
double drag_cd_g6(double mach);
double drag_cd_g7(double mach);
double drag_cd_g8(double mach);
double drag_cd_gl(double mach);
double drag_cd_gi(double mach);

#endif /* _DRAG_H_ */
