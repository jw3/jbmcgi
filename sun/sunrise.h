
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

#ifndef _SUNRISE_H_
#define _SUNRISE_H_

#include <math.h>
#include <stdio.h>

#include "sun.h"
#include "dst.h"

typedef struct { 
  int       year; 
  int       month; 
  int       day; 
  double    tz;
  double    lat; 
  double    lon; 
  int       dst;
  int       sr;
  double    rise; 
  double    set; 
  double    length;
} Sunrise, *lpSunrise;

#define SUNRISE_SIZE  (sizeof(Sunrise))

/* Function prototypes */
lpSunrise sunrise_create(void);
void sunrise_destroy(Sunrise*);
int sunrise_calculate(Sunrise*);

#endif /* _SUNRISE_H_ */
