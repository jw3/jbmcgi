

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
#include "setup.h"
#include "maxdist.h"
#include "errors.h"
#include "utility.h"
#include "post.h"

/*
  version
*/
#define MAXDIST_VERSION  "jbmmdist, GNU version 2.0, March 2000"

/*
  define default and minimum and maximum values...
*/
#define   DEF_MV         3000.0
#define   DEF_BC         0.500
#define   DEF_WT         220.0
#define   DEF_TP         59.0
#define   DEF_PR         29.92
#define   DEF_HU         0.0
#define   DEF_AL         0.0

#define   MIN_MV         500.0
#define   MIN_BC         0.050
#define   MIN_WT         10.0
#define   MIN_TP         -40.0
#define   MIN_PR         15.0
#define   MIN_HU         0.0
#define   MIN_AL         -1000.0

#define   MAX_MV         5000.0
#define   MAX_BC         2.000
#define   MAX_WT         5000.0
#define   MAX_TP         140.0
#define   MAX_PR         40.0
#define   MAX_HU         100.0
#define   MAX_AL         15000.0


/*
 define search strings to be used...
*/
#define   MV_STR         "mv"
#define   BC_STR         "bc"
#define   DF_STR         "df"
#define   WT_STR         "wt"
#define   TP_STR         "tp"
#define   PR_STR         "pr"
#define   HU_STR         "hu"
#define   AL_STR         "al"
#define   ALC_STR        "alc"

int main()
{
  lpMaxDistance maxdistance;
  int error;

  html_setfile(stdout);

  maxdistance = maxdistance_create();
  if (maxdistance)
  {
    error = doinput(maxdistance);
    if (!error) maxdistance_calculate(maxdistance);
    dooutput(maxdistance, error);
    maxdistance_destroy(maxdistance);
  }
  return 0;
}

/*
 define arrays for default, maximum and minimum values...
*/
int doinput(lpMaxDistance maxdistance)
{
  lpPostElement elements;
  double tmp;
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
  tmp = post_asfloat(elements, MV_STR);
  maxdistance->muzzle_velocity = BRACKETDEF(tmp, MIN_MV, MAX_MV, DEF_MV);

  tmp = post_asfloat(elements, WT_STR);
  maxdistance->weight = BRACKETDEF(tmp, MIN_WT, MAX_WT, DEF_WT);

  tmp = post_asfloat(elements, BC_STR);
  maxdistance->bc->bc = BRACKETDEF(tmp, MIN_BC, MAX_BC, DEF_BC);

  bc_setdragfunc(maxdistance->bc, post_asint(elements, DF_STR) - 1);

  tmp = post_asfloat(elements, TP_STR);
  maxdistance->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  maxdistance->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  maxdistance->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  maxdistance->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  maxdistance->alc = post_ascheck(elements, ALC_STR);

  post_destroyelements(elements);

  return ERROR_NOERROR;
}

/* writes the input data to the web page... */
void writedata(lpMaxDistance maxdistance)
{
  double tmp;

  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Muzzle Velocity:", " ft/sec", 
    "%6.1f", maxdistance->muzzle_velocity);
  html_writetablevalue("Bullet Weight:", " grains", 
    "%6.1f", maxdistance->weight);
  html_writetablevalue("Ballistic Coefficient:", " ", 
    "%5.3f", maxdistance->bc->bc);

  html_writetablestring("Drag Function:", "", "%s",
    dragstr[maxdistance->bc->dragfunc]);

  html_writetablevalue("Temperature:", " °F", 
    "%5.1f", maxdistance->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", " in Hg",  
    "%5.2f", maxdistance->atmos->pressure);
  html_writetablevalue("Relative Humidity:", " %", 
    "%5.1f", maxdistance->atmos->humidity);
  html_writetablevalue("Altitude:", " feet", 
    "%6.0f", maxdistance->atmos->altitude);
  tmp = 100.0*maxdistance->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", "% of Sea Level", "%5.0f", tmp);
  html_closetable();
  html_break();
}

/* writes the calculated data to the web page... */
void writetable(lpMaxDistance maxdistance)
{
  /* write the maxdistance data... */
  html_writeheader("Calculated Data", 3);
  html_writeheader("Maximum Distance", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue("Firing Elevation:", " degrees", 
    "%5.1f", maxdistance->muzzle_elevation);
  html_writetablevalue("Terminal Range:", " yards", 
    "%5.1f", maxdistance->terminal_range);
  html_writetablevalue("Maximum Height:", " feet", 
    "%5.1f", maxdistance->terminal_height);
  html_writetablevalue("Terminal Angle:", " degrees", 
    "%5.1f", maxdistance->terminal_angle);
  html_writetablevalue("Terminal Velocity:", " ft/sec", 
    "%5.1f", maxdistance->terminal_velocity);
  html_writetablevalue("Terminal Energy:", " ft-lbs", 
    "%5.1f", maxdistance->terminal_energy);
  html_writetablevalue("Time of Flight:", " secs", 
    "%5.1f", maxdistance->terminal_time);
  html_closetable();
  html_break();

  html_writeheader("Absolute Maximum Height", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue("Maximum Height:", " feet", 
    "%5.1f", maxdistance->maximum_height);
  html_writetablevalue("Time of Flight:", " secs", 
    "%5.1f", maxdistance->maximum_time);
  html_closetable();
  html_break();
}

/* main output routine... */
int dooutput(lpMaxDistance maxdistance, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_maxdist[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(MAXDIST_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(maxdistance);

    html_horzrule(NULL, 0, 0, 0, 0);

    writetable(maxdistance);

    html_closebody();
    html_closehead();
  }
  return 0;
}
