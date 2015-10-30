
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

#ifndef _DST_H_
#define _DST_H_

#define _SECSPERDAY           (86400.0)     /* 24*60*60 */
#define _MSECSPERDAY          (86400000.0)  /* 1000*24*60*60 */
#define _MINSPERDAY           (1440.0)
#define _MINSPERWEEK          (10080.0)
#define _HOURSPERWEEK         (168.0)

#define _YEAR                 (365.0)
#define _WEEK                 (7.0)
#define _DAY                  (1.0)
#define _HOUR                 (1.0/24.0)
#define _MINUTE               (_HOUR/60.0)
#define _SECOND               (_MINUTE/60.0)

#define _SUN                  (1)
#define _MON                  (2)
#define _TUE                  (3)
#define _WED	               (4)
#define _THU                  (5)
#define _FRI                  (6)
#define _SAT                  (7)

#define DIVMOD(a, b, d, m)    (d) = (a)/(b); (m) = (a)%(b)

static int daytable[2][13] =
  {
	 {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	 {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  };

int dst_isleapyear(int year);
int dst_correction(int year, int month, int day);
int dst_dayofweek(double datetime);
void dst_decodedate(double datetime, int *year, int *month, int *day);
double dst_encodedate(int year, int month, int day);

#endif /* _DST_H_ */
