
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

#ifndef _ERRORS_H_
#define _ERRORS_H_

/* BC - Velocity */

#define  ERROR_BCV_NOERROR        0
#define  ERROR_BCV_INVALIDFARVEL  1
#define  ERROR_BCV_INVALIDBC      2

static char *errors_bcv[] =
  {"None.",
   "Near velocity must be greater than the far velocity.",
   "BC is higher than 4.0 (please send me that bullet!)"};

/* BC - Time */

#define  ERROR_BCT_NOERROR        0
#define  ERROR_BCT_INVALIDNVEL    1
#define  ERROR_BCT_INVALIDTIME    2
#define  ERROR_BCT_INVALIDBC      3

static char *errors_bct[] =
  {"None.",
   "Invalid near velocity.",
   "Entered time of flight is impossible for entered velocity and time.",
   "BC can not be calculated."};

/* Drag and Twist */

#define  ERROR_DRAG_NOERROR        0
#define  ERROR_DRAG_INVALIDDBA     1
#define  ERROR_DRAG_INVALIDDMP     2
#define  ERROR_DRAG_INVALIDNSL     3
#define  ERROR_DRAG_INVALIDRAD     4
#define  ERROR_DRAG_INVALIDTLN     5

static char *errors_drag[] =
  {"None.",
   "Base diameter must be less than one caliber.",
   "Meplat diameter must be less than half the caliber.",
   "Nose length must be longer than one caliber.",
   "Nose radius must be greater than one caliber.",
   "Total length must be greater than the nose length and boattail length"};


#define  ERROR_NOERROR          0

static char *errors[] =
  {
    "None.",
  };

/* Maximum distance errors */

#define  ERROR_MAXDIST_NOERROR          0

static char *errors_maxdist[] =
  {
    "None.",
  };

/* Point Blank Range */

#define  ERROR_PBR_NOERROR          0
#define  ERROR_PBR_SH               1

static char *errors_pbr[] =
  {
    "None.",
    "Sight Height must be inside the vital zone"
    " (|sight height| < vital zone radius."
  };
   

#define  ERROR_TRAJ_NOERROR          0
#define  ERROR_TRAJ_INVALIDRANGEMAX  1
#define  ERROR_TRAJ_INVALIDRANGEMIN  2
#define  ERROR_TRAJ_INVALIDRANGEZERO 3

static char *errors_traj[] =
  {
    "None.",
    "Maximum range must be greater than the zero range and minimum range.",
    "Minimum range must be less than the zero range and maximum range.",
    "Zero range must be between the minimum and maximum range."
  };

/* Sunrise & Sunset */

#define  ERROR_SUNRISE_NOERROR          0
#define  ERROR_SUNRISE_INVALIDYEAR      1
#define  ERROR_SUNRISE_INVALIDMONTH     2
#define  ERROR_SUNRISE_INVALIDDAY       3
#define  ERROR_SUNRISE_INVALIDTIME      4

static char *errors_sun[] =
  {
    "None.",
    "",
    "",
    "",
    ""
  };


#endif /* _ERRORS_H_ */
