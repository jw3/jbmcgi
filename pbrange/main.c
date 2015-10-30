
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"
#include "post.h"
#include "setup.h"
#include "errors.h"
#include "pbrange.h"
#include "utility.h"

/*
  version
*/
#define   PBR_VERSION  "jbmpbr, version 2.0, March 2000"

/*
  define default and minimum and maximum values...
*/
#define   DEF_MV       3000.0
#define   DEF_BC       0.500
#define   DEF_WT       220.0
#define   DEF_SH       1.5
#define   DEF_VZ       5.0
#define   DEF_TP       59.0
#define   DEF_PR       29.92
#define   DEF_HU       0.0
#define   DEF_AL       0.0

#define   MIN_MV       500.0
#define   MIN_BC       0.050
#define   MIN_WT       10.0
#define   MIN_SH       0.0
#define   MIN_VZ       0.1
#define   MIN_TP       -40.0
#define   MIN_PR       15.0
#define   MIN_HU       0.0
#define   MIN_AL       -1000.0

#define   MAX_MV       5000.0
#define   MAX_BC       2.000
#define   MAX_WT       5000.0
#define   MAX_SH       100.0
#define   MAX_VZ       100.0
#define   MAX_TP       140.0
#define   MAX_PR       40.0
#define   MAX_HU       100.0
#define   MAX_AL       15000.0

/*
 define search strings to be used...
*/
#define   MV_STR       "mv"
#define   BC_STR       "bc"
#define   DF_STR       "df"
#define   WT_STR       "wt"
#define   SH_STR       "sh"
#define   VZ_STR       "vz"
#define   TP_STR       "tp"
#define   PR_STR       "pr"
#define   HU_STR       "hu"
#define   AL_STR       "al"
#define   ALC_STR      "alc"

int main()
{
  lpPointBlankRange pointblankrange;
  int error;

  html_setfile(stdout);

  pointblankrange = pointblankrange_create();
  if (pointblankrange)
  {
    error = doinput(pointblankrange);
    if (!error) pointblankrange_calculate(pointblankrange);
    dooutput(pointblankrange, error);
    pointblankrange_destroy(pointblankrange);
  }
  return 0;
}

/*
 define arrays for default, maximum and minimum values...
*/
int doinput(lpPointBlankRange pointblankrange)
{
  lpPostElement elements;
  double tmp;
  int    err;

  elements = post_processelements(&err);
  if (err < 0) return err;

  /*
    Setup...
  */
  setup_input(elements);

  /*
   check for input values...
  */
  tmp = post_asfloat(elements, MV_STR);
  pointblankrange->muzzle_velocity = BRACKETDEF(tmp, MIN_MV, MAX_MV, DEF_MV);

  tmp = post_asfloat(elements, BC_STR);
  pointblankrange->bc->bc = BRACKETDEF(tmp, MIN_BC, MAX_BC, DEF_BC);

  bc_setdragfunc(pointblankrange->bc, post_asint(elements, DF_STR) - 1);

  tmp = post_asfloat(elements, WT_STR);
  pointblankrange->bullet_weight = BRACKETDEF(tmp, MIN_WT, MAX_WT, DEF_WT);

  tmp = post_asfloat(elements, VZ_STR);
  pointblankrange->vital_zone = BRACKETDEF(tmp, MIN_VZ, MAX_VZ, DEF_VZ);

  tmp = post_asfloat(elements, SH_STR);
  pointblankrange->sight_height = BRACKETDEF(tmp, MIN_SH, MAX_SH, DEF_SH);

  tmp = post_asfloat(elements, TP_STR);
  pointblankrange->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  pointblankrange->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  pointblankrange->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  pointblankrange->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  pointblankrange->alc = post_ascheck(elements, ALC_STR);

  post_destroyelements(elements);

  if (fabs(pointblankrange->sight_height) > pointblankrange->vital_zone) 
    return ERROR_PBR_SH;
  return ERROR_NOERROR;
}


/* writes the input data to the web page... */
void writedata(lpPointBlankRange pointblankrange)
{
  double tmp;

  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Muzzle Velocity:", " ft/sec", 
    "%6.1f", pointblankrange->muzzle_velocity);
  html_writetablevalue("Bullet Weight:", " grains", 
    "%5.0f", pointblankrange->bullet_weight);
  html_writetablevalue("Ballistic Coefficient:", " ", 
    "%5.3f", pointblankrange->bc->bc);

  html_writetablestring("Drag Function:", "", "%s", 
    dragstr[pointblankrange->bc->dragfunc]);

  html_writetablevalue("Sight Height:", " inches", 
    "%5.3f", pointblankrange->sight_height);
  html_writetablevalue("Vital Zone Radius:", " inches", 
    "%5.3f", pointblankrange->vital_zone);
  html_writetablevalue("Temperature:", " °F", 
    "%5.1f", pointblankrange->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", " in Hg", 
    "%5.2f", pointblankrange->atmos->pressure);
  html_writetablevalue("Relative Humidity:", " %", 
    "%5.1f", pointblankrange->atmos->humidity);
  html_writetablevalue("Altitude:", " feet",   
    "%6.0f", pointblankrange->atmos->altitude);
  tmp = 100.0*pointblankrange->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", "% of Sea Level", "%5.0f", tmp);

  html_closetable();
  html_break();
}

/* writes the calculated data to the web page... */
void writetable(lpPointBlankRange pointblankrange)
{
  /* write the point blank range data... */
  html_writeheader("Calculated Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue("Maximum Point Blank Range:", " yards", 
    "%5.0f", pointblankrange->pbrange);
  html_writetablevalue("Maximum Point Blank Range Zero:", " yards", 
    "%5.0f", pointblankrange->pbzero);
  html_writetablevalue("Muzzle Energy:", " ft-lbs", 
    "%6.1f", pointblankrange->muzzle_energy);
  html_writetablevalue("Energy at Maximum Point Blank Range:", " ft-lbs",
    "%6.1f", pointblankrange->terminal_energy);
  html_closetable();
  html_break();
}

/* main output routine... */
int dooutput(lpPointBlankRange pointblankrange, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_pbr[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(PBR_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(pointblankrange);

    html_horzrule(NULL, 0, 0, 0, 0);

    writetable(pointblankrange);

    html_closebody();
    html_closehead();

  }
  return 0;
}
