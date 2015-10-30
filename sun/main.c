

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"
#include "post.h"
#include "setup.h"
#include "errors.h"
#include "sunrise.h"
#include "utility.h"

/*
  version
*/
#define   SUN_VERSION  "jbmsun, GNU version 2.0, March, 2000"

/*
  define default and minimum and maximum values...
*/
#define   DEF_YR       1997
#define   DEF_MO       1
#define   DEF_DY       1
#define   DEF_LT       0.0
#define   DEF_LN       0.0
#define   DEF_TZ       -7.0

#define   MIN_YR       1801
#define   MIN_MO       1
#define   MIN_DY       1
#define   MIN_LT      -90.0
#define   MIN_LN      -180.0
#define   MIN_TZ      -12.0

#define   MAX_YR       2099
#define   MAX_MO       12
#define   MAX_DY       31
#define   MAX_LT       90.0
#define   MAX_LN       180.0
#define   MAX_TZ       12.0

/*
 define search strings to be used...
*/
#define   YR_STR       "yr"
#define   MO_STR       "mo"
#define   DY_STR       "dy"
#define   LTD_STR      "lt_d"
#define   LTM_STR      "lt_m"
#define   LTS_STR      "lt_s"
#define   LND_STR      "ln_d"
#define   LNM_STR      "ln_m"
#define   LNS_STR      "ln_s"
#define   TZ_STR       "tz"
#define   DST_STR      "dst"

int main()
{
  lpSunrise sunrise;
  int error;

  html_setfile(stdout);

  sunrise = sunrise_create();
  if (sunrise)
  {
    error = doinput(sunrise);
    if (!error) sunrise_calculate(sunrise);
    dooutput(sunrise, error);
    sunrise_destroy(sunrise);
  }
  return 0;
}


/*
 define arrays for default, maximum and minimum values...
*/
int doinput(lpSunrise sunrise)
{
  lpPostElement elements;
  double ftmp;
  int    itmp;
  int    chk;
  int    err;

  elements = post_processelements(&err);
  if (err < 0) return err;

  /*
    Setup
  */
  setup_input(elements);

  /*
   check for input values...
  */

  itmp = post_asint(elements, YR_STR);
  sunrise->year = BRACKETDEF(itmp, MIN_YR, MAX_YR, DEF_YR);

  itmp = post_asint(elements, MO_STR);
  sunrise->month = BRACKETDEF(itmp, MIN_MO, MAX_MO, DEF_MO);
  
  itmp = post_asint(elements, DY_STR);
  sunrise->day = BRACKETDEF(itmp, MIN_DY, MAX_DY, DEF_DY);
  
  ftmp = post_asfloat(elements, LTD_STR) +   
        post_asfloat(elements, LTM_STR)/60.0 + 
        post_asfloat(elements, LTS_STR)/3600.0;
  sunrise->lat = BRACKETDEF(ftmp, MIN_LT, MAX_LT, DEF_LT);

  ftmp = post_asfloat(elements, LND_STR) +   
        post_asfloat(elements, LNM_STR)/60.0 + 
        post_asfloat(elements, LNS_STR)/3600.0;
  sunrise->lon = BRACKETDEF(ftmp, MIN_LN, MAX_LN, DEF_LN);

  ftmp = post_asfloat(elements, TZ_STR);
  sunrise->tz = BRACKETDEF(ftmp, MIN_TZ, MAX_TZ, DEF_TZ);

  sunrise->dst = post_ascheck(elements, DST_STR);

  free(elements);

  return ERROR_NOERROR;
}

void split_time(double time, double *h, double *m, double *s)
{
  *h = floor(time);
  time = 60.0*(time - *h);
  *m = floor(time);
  *s = 60.0*(time - *m);
}

void writetime(char *descrip, double time)
{
  char buf[64]; 
  double h, m, s;
  split_time(time, &h, &m, &s);

  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, descrip);

  if (h < 12)
  {
    sprintf(buf, "%.0f:%02.0f:%02.0f", h, m, s);
    html_tabledata(HTML_TABLE_ALIGN_RIGHT, 1, 1, 0, buf);
    html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, "am");
  }
  else
  {
    sprintf(buf, "%.0f:%02.0f:%02.0f", h - 12, m, s);
    html_tabledata(HTML_TABLE_ALIGN_RIGHT, 1, 1, 0, buf);
    html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, "pm");
  }
  html_tablecloserow();
}

void writeangle(char *descrip, double angle)
{
  double d, m, s;
  split_time(angle, &d, &m, &s);
  printf("<TR><TD> %s </TD><TD ALIGN = RIGHT>", descrip);
  printf("%.0f%s %2.0f%s %5.3f%s", d, "&deg", m, "\'", s, "\'\'");
  printf("</TD><TD ALIGN = LEFT></TD></TR>\n");
}

void writedate(char *descrip, int y, int m, int d)
{
  char buf[64];
  sprintf(buf, "%02d/%02d/%02d", d, m, y);

  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, descrip);
  html_tabledata(HTML_TABLE_ALIGN_RIGHT, 1, 1, 0, buf);
  html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, "dd/mm/yyyy");
}

/* writes the input data to the web page... */
void writedata(lpSunrise sunrise)
{
  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  writedate("Date:", sunrise->year, sunrise->month, sunrise->day);
  writeangle("Latitude:", sunrise->lat);
  writeangle("Longitude:", sunrise->lon);
  html_writetablestring("DST Correction", "", "%s", 
    sunrise->dst ? "Yes" : "No");
  html_closetable();
  html_break();
}

void writetable(lpSunrise sunrise)
{
  double l;
  html_writeheader("Calculated Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  if (sunrise->sr != 0)
  {
    html_writetablestring("Sunrise:", "", "%s", "None");
    html_writetablestring("Sunset:", "", "%s", "None");
    l = (sunrise->sr == 1 ? 24.0 : 0.0);
    html_writetablevalue("Day Length:", " hours", "%3.1f", l);
  }
  else
  {
    writetime("Sunrise:", sunrise->rise);
    writetime("Sunset:", sunrise->set);
    html_writetablevalue("Day Length:", " hours", "%3.1f", sunrise->length);
  }
  html_closetable();
  html_break();
}

/* main output routine... */
int dooutput(lpSunrise sunrise, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_sun[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(SUN_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(sunrise);

    html_horzrule(NULL, 0, 0, 0, 0);

    writetable(sunrise);

    html_closebody();
    html_closehead();
  }
  return 0;
}

