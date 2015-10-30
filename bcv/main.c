
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

#include "bcv.h"
#include "post.h"
#include "html.h"
#include "setup.h"
#include "errors.h"
#include "utility.h"

/*
  version
*/
#define BCV_VERSION    "jbmbcv, GNU version 2.0, March 2000"

/*
  define default and minimum and maximum values...
*/
#define   DEF_NV       3000.0
#define   DEF_FV       2850.0
#define   DEF_XC       100.0
#define   DEF_TP       59.0
#define   DEF_PR       29.92
#define   DEF_HU       0.0
#define   DEF_AL       0.0

#define   MIN_NV       600.0
#define   MIN_FV       500.0
#define   MIN_XC       1.0
#define   MIN_TP       -40.0
#define   MIN_PR       15.0
#define   MIN_HU       0.0
#define   MIN_AL       -1000.0

#define   MAX_NV       4500.0
#define   MAX_FV       4400.0
#define   MAX_XC       1000.0
#define   MAX_TP       140.0
#define   MAX_PR       40.0
#define   MAX_HU       100.0
#define   MAX_AL       15000.0

/*
 define search strings to be used...
*/
#define   NV_STR       "nv"
#define   FV_STR       "fv"
#define   XC_STR       "xc"
#define   DF_STR       "df"
#define   TP_STR       "tp"
#define   PR_STR       "pr"
#define   HU_STR       "hu"
#define   AL_STR       "al"
#define   ALC_STR      "alc"

int main()
{
  int    error;
  lpBCV bcv;

  html_setfile(stdout);

  bcv = bcv_create();
  if (bcv)
  {
    error = doinput(bcv);
    if (!error) error = bcv_calculate(bcv);
    dooutput(bcv, error);
    bcv_destroy(bcv);
  }
  return 0;
}

int doinput(lpBCV bcv)
{
  lpPostElement elements;
  double tmp;
  int err;

  elements = post_processelements(&err);
  if (err < 0) return err;

  /*
  */
  setup_input(elements);

  /*
   check for input values...
  */
  bcv->nvel = post_asfloat(elements, NV_STR);
  bcv->nvel = BRACKETDEF(bcv->nvel, MIN_NV, MAX_NV, DEF_NV);

  bcv->fvel = post_asfloat(elements, FV_STR);
  bcv->fvel = BRACKETDEF(bcv->fvel, MIN_FV, MAX_FV, DEF_FV);

  bcv->dist = post_asfloat(elements, XC_STR);
  bcv->dist = BRACKETDEF(bcv->dist, MIN_XC, MAX_XC, DEF_XC);

  bcv->dragfunc = post_asint(elements, DF_STR) - 1;
  bcv->dragfunc = BRACKETDEF(bcv->dragfunc, g1, gi, g1);

  tmp = post_asfloat(elements, TP_STR);
  bcv->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  bcv->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  bcv->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  bcv->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  bcv->alc = post_ascheck(elements, ALC_STR);

  post_destroyelements(elements);

  if (bcv->nvel <= bcv->fvel) return ERROR_BCV_INVALIDFARVEL;
  return ERROR_BCV_NOERROR;
}

/* writes the input data to the web page... */
void writedata(lpBCV bcv)
{
  double tmp;
  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Near Velocity:", " ft/sec", "%6.1f", bcv->nvel);
  html_writetablevalue("Far Velocity:", " ft/sec", "%6.1f", bcv->fvel);
  html_writetablevalue("Distance:", " feet", "%8.3f", bcv->dist);
  html_writetablevalue("Temperature:", " °F", 
    "%5.1f", bcv->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", " in Hg", 
    "%5.2f", bcv->atmos->pressure);
  html_writetablevalue("Relative Humidity:", " %", 
    "%5.1f", bcv->atmos->humidity);
  html_writetablevalue("Altitude:", " feet", "%6.1f", bcv->atmos->altitude);
  tmp = 100.0*bcv->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", "% of Sea Level", "%5.0f", tmp);
  html_closetable();
}

/* writes the calculated data to the web page... */
void writeoutputdata(lpBCV bcv)
{
  char tmp[48];
  sprintf(tmp, "%s Ballistic Coefficient:", dragstr[(int)bcv->dragfunc]);

  html_writeheader("Output Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue(tmp, "", "%5.3f", bcv->bc);
  html_writetablevalue("Time of Flight:", " sec", "%7.3f", bcv->time);
  html_closetable();
}

/* main output routine... */
int dooutput(lpBCV bcv, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_bcv[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(BCV_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(bcv);

    html_horzrule(NULL, 0, 0, 0, 0);

    writeoutputdata(bcv);

    html_closebody();
    html_closehead();
  }
  return 0;
}

