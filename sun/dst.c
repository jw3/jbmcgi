
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

#include <math.h>

#include "dst.h"

int dst_isleapyear(int year)
{
  return ((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0));
}

int dst_correction(int year, int month, int day)
{
  double d;
  int apr_s, oct_s;

  if ((month < 4) || (month > 9)) return 0;
  if ((month > 4) && (month < 9)) return 1;

  if (month == 4)
  {
    /* find first Sunday in April */
    apr_s = 0;
    do
    {
      apr_s++;
      d = dst_encodedate(year, 4, apr_s);
    }
    while (dst_dayofweek(d) > 1);
  
    if (day >= apr_s) return 1;
    return 0;
  }

  if (month == 9)
  {
    /* find last Sunday in October */
    oct_s = 32;
    do
    {
      oct_s--;
      d = dst_encodedate(year, 10, oct_s);
    }
    while (dst_dayofweek(d) > 1);
   
    if (day >= oct_s) return 0;
    return 1;
  } 
  
}

int dst_dayofweek(double datetime)
{
  double dt;
  long day;
  datetime = modf(datetime + 0.5/_MSECSPERDAY, &dt);
  day = (long)dt;
  return (day % 7) + 1;
}

#define D1    (365)
#define D4    (D1*4 + 1)
#define D100  (D4*25 - 1)
#define D400  (D100*4 + 1)

void dst_decodedate(double datetime, int *year, int *month, int *day)
{
  long i, t, y, m, d;
  double dt;

  datetime = modf(datetime + 0.5/_MSECSPERDAY, &dt);
  t = (int)dt;
  if (t <= 0)
  {
    *year = 0;
    *month = 0;
    *day = 0;
  }
  else
  {
    t--;
    y = 1;
    while (t >= D400)
    {
      t -= D400;
      y += 400;
    }
    DIVMOD(t, D100, i, d);
    if (i == 4)
    {
      i--;
      d += D1;
    }
    y += i;
    m = 1;
    for(;;)
    {
      i = daytable[y][m];
      if (d < i) break;
      d -= i;
      m++;
    }
    *year = y;
    *month = m;
    *day = d + 1;
  }
}

double dst_encodedate(int year, int month, int day)
{
  long i;
  int  leap;

  leap = dst_isleapyear(year);
  for (i = 1; i < month; i++) day += daytable[leap][i];
  i = year - 1;
  return (i*365.0 + day + i/4 - i/100 + i/400);
}


