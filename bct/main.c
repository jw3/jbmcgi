
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

#include "bct.h"
#include "post.h"
#include "html.h"
#include "setup.h"
#include "errors.h"
#include "htmlcolor.h"
#include "utility.h"

/* 
  version
*/
#define   BCT_VERSION  "jbmbct, GNU version 2.0, March 2000"

/*
  define default and minimum and maximum values...
*/
#define   DEF_NV       3000.0
#define   DEF_TM       0.001
#define   DEF_XC       100.0
#define   DEF_TP       59.0
#define   DEF_PR       29.92
#define   DEF_HU       0.0
#define   DEF_AL       0.0

#define   MIN_NV       600.0
#define   MIN_TM       0.000001
#define   MIN_XC       1.0
#define   MIN_TP       -40.0
#define   MIN_PR       15.0
#define   MIN_HU       0.0
#define   MIN_AL       -1000.0

#define   MAX_NV       4500.0
#define   MAX_TM       6.0
#define   MAX_XC       1000.0
#define   MAX_TP       140.0
#define   MAX_PR       40.0
#define   MAX_HU       100.0
#define   MAX_AL       15000.0

/*
 define search strings to be used...
*/
#define   NV_STR       "nv"
#define   TM_STR       "tm"
#define   XC_STR       "xc"
#define   DF_STR       "df"
#define   TP_STR       "tp"
#define   PR_STR       "pr"
#define   HU_STR       "hu"
#define   AL_STR       "al"
#define   ALC_STR      "alc"

int doinput(lpBCT bct);
int dooutput(lpBCT bct, int error);

int main()
{
  int   error;
  lpBCT bct;

  html_setfile(stdout);

  bct = bct_create();
  if (bct)
  {
    error = doinput(bct);
    if (!error) error = bct_calculate(bct);
    dooutput(bct, error);
    bct_destroy(bct);
  }
  return 0;
}

/* Input */

int doinput(lpBCT bct)
{
  lpPostElement elements;
  double tmp;
  int err;

  err = 0;
  elements = post_processelements(&err);
  if (err < 0) return err;

  /*
    Check for setup values...
  */
  setup_input(elements);

  /*
   check for input values...
  */
  bct->nvel = post_asfloat(elements, NV_STR);
  bct->nvel = BRACKETDEF(bct->nvel, MIN_NV, MAX_NV, DEF_NV);

  bct->time = post_asfloat(elements, TM_STR);
  bct->time = BRACKETDEF(bct->time, MIN_TM, MAX_TM, DEF_TM);

  bct->dist = post_asfloat(elements, XC_STR);
  bct->dist = BRACKETDEF(bct->dist, MIN_XC, MAX_XC, DEF_XC);

  bct->dragfunc = post_asint(elements, DF_STR) - 1;
  bct->dragfunc = BRACKETDEF(bct->dragfunc, g1, gi, g1);

  tmp = post_asfloat(elements, TP_STR);
  bct->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  bct->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  bct->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  bct->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  bct->alc = post_ascheck(elements, ALC_STR);

  post_destroyelements(elements);

  if (bct->nvel < 500.0) return ERROR_BCT_INVALIDNVEL;
  if (bct->dist > bct->nvel*bct->time) return ERROR_BCT_INVALIDTIME;
  return ERROR_BCT_NOERROR;
}

/* Output */

/* writes the input data to the web page... */
void writedata(lpBCT bct)
{
  double tmp;
  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Near Velocity:", " ft/sec", "%6.1f", bct->nvel);
  html_writetablevalue("Flight Time:",  " sec", "%8.6f", bct->time);
  html_writetablevalue("Distance:", " feet", "%8.3f", bct->dist);

  html_writetablevalue("Temperature:", " °F", "%5.1f", bct->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", 
    " in Hg", "%5.2f", bct->atmos->pressure);
  html_writetablevalue("Relative Humidity:", 
    " %", "%5.2f", bct->atmos->humidity);
  html_writetablevalue("Altitude:", " feet", "%6.1f", bct->atmos->altitude);

  tmp = 100.0*bct->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", " % of Sea Level", "%5.0f", tmp);
  html_closetable();
  html_break();
}

/* writes the calculated data to the web page... */
void writeoutputdata(lpBCT bct)
{
  char buf[64];
  sprintf(buf, "%s Ballistic Coefficient:", dragstr[(int)bct->dragfunc]);

  html_writeheader("Output Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue(buf, "", "%5.3f", bct->bc);
  html_writetablevalue("Far Velocity:", " ft/sec", "%6.1f", bct->fvel);
  html_closetable();
}

/* main output routine... */
int dooutput(lpBCT bct, int error)
{
  extern char setup_title[];

  if (error) posterr_error(error, (error > 0) ? errors_bct[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(BCT_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);
    writedata(bct);
    html_horzrule(NULL, 0, 0, 0, 0);
    writeoutputdata(bct);
    html_closebody();
    html_closehead();
  }
  return 0;
}
