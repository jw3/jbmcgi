
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

#include "post.h"
#include "html.h"
#include "traj.h"
#include "setup.h"
#include "errors.h"
#include "utility.h"

/*
  version string
*/
#define   TRAJ_VERSION "jbmtraj, GNU version 2.0, March 2000"

/*
 define default and minimum and maximum values...
*/
#define   DEF_MV       3000.0
#define   DEF_BC       0.500
#define   DEF_WT       220.0
#define   DEF_WX       0.0
#define   DEF_WY       0.0
#define   DEF_WZ       10.0
#define   DEF_XC       10.0
#define   DEF_SH       1.5
#define   DEF_SO       0.0
#define   DEF_SP       10.0
#define   DEF_SA       90.0
#define   DEF_LA       0.0
#define   DEF_CA       0.0
#define   DEF_TP       59.0
#define   DEF_PR       29.92
#define   DEF_HU       0.0
#define   DEF_AL       0.0
#define   DEF_RN       0
#define   DEF_RX       1000
#define   DEF_RI       100
#define   DEF_RZ       100
#define   DEF_AZ       0.0
#define   DEF_EL       0.0
#define   DEF_ZY       0
#define   DEF_ZZ       0

#define   MIN_MV       500.0
#define   MIN_BC       0.010
#define   MIN_WT       5.0
#define   MIN_WX       -100.0
#define   MIN_WY       -100.0
#define   MIN_WZ       -100.0
#define   MIN_XC       0.0
#define   MIN_SH       -100.0
#define   MIN_SO       -100.0
#define   MIN_SP       -100.0
#define   MIN_SA       0.0
#define   MIN_LA       -90.0
#define   MIN_CA       -90.0
#define   MIN_TP       -40.0
#define   MIN_PR       15.0
#define   MIN_HU       0.0
#define   MIN_AL       -1000.0
#define   MIN_RN       0
#define   MIN_RX       100
#define   MIN_RI       1
#define   MIN_RZ       10
#define   MIN_AZ       -300.0
#define   MIN_EL       -300.0
#define   MIN_ZY       -100
#define   MIN_ZZ       -100

#define   MAX_MV       5000.0
#define   MAX_BC       2.000
#define   MAX_WT       5000.0
#define   MAX_WX       100.0
#define   MAX_WY       100.0
#define   MAX_WZ       100.0
#define   MAX_XC       100.0
#define   MAX_SH       100.0
#define   MAX_SO       100.0
#define   MAX_SP       100.0
#define   MAX_SA       360.0
#define   MAX_LA       90.0
#define   MAX_CA       90.0
#define   MAX_TP       140.0
#define   MAX_PR       40.0
#define   MAX_HU       100.0
#define   MAX_AL       15000.0
#define   MAX_RN       1900
#define   MAX_RX       2000
#define   MAX_RI       500
#define   MAX_RZ       2000
#define   MAX_AZ       300.0
#define   MAX_EL       300.0
#define   MAX_ZY       100
#define   MAX_ZZ       100


/*
 define search strings to be used...
*/
#define   MV_STR       "mv"
#define   BC_STR       "bc"
#define   DF_STR       "df"
#define   WT_STR       "wt"
#define   WX_STR       "wx"
#define   WY_STR       "wy"
#define   WZ_STR       "wz"
#define   XC_STR       "xc"
#define   SH_STR       "sh"
#define   SO_STR       "so"
#define   SP_STR       "sp"
#define   SA_STR       "sa"
#define   LA_STR       "la"
#define   CA_STR       "ca"
#define   RN_STR       "rn"
#define   RX_STR       "rx"
#define   RI_STR       "ri"
#define   RZ_STR       "rz"
#define   TP_STR       "tp"
#define   PR_STR       "pr"
#define   HU_STR       "hu"
#define   AL_STR       "al"
#define   ALC_STR      "alc"
#define   AZC_STR      "azc"
#define   ELC_STR      "elc"
#define   UN_STR       "un"
#define   AZ_STR       "az"
#define   EL_STR       "el"
#define   BS_STR       "bs"
#define   ZY_STR       "zy"
#define   ZZ_STR       "zz"

/* defines the maximum size of the buffer used in output... */
#define  TRAJ_NUMCOLUMNS 8

static char *HEADERSTR[] =
  {"Range", "Velocity", "Energy", "Momentum", "Drop",
	"Windage", "Lead", "Time"};

static char *INHEADERUNITS[] =
  {"(yards)", "(ft/sec)", "(ft-lbs)", "(lbs-sec)", 
   "(inches)", "(inches)", "(inches)", "(sec)"};

static char *MOAHEADERUNITS[] =
  {"(yards)", "(ft/sec)", "(ft-lbs)", "(lbs-sec)", 
   "(moa)", "(moa)", "(moa)", "(sec)"};

static char *MILHEADERUNITS[] =
  {"(yards)", "(ft/sec)", "(ft-lbs)", "(lbs-sec)", 
   "(mil)", "(mil)", "(mil)", "(sec)"};

static char *TABLEFORMATS[] =
  {"%5.0f", "%6.1f", "%6.1f", "%5.2f", "%5.1f", "%5.1f", "%5.1f", "%5.3f"};

int main()
{
  lpTrajectory trajectory;
  int error;

  html_setfile(stdout);

  trajectory = trajectory_create();
  if (trajectory)
  {
    error = doinput(trajectory);
    if (!error) trajectory_calculate(trajectory);
    dooutput(trajectory, error);
    trajectory_destroy(trajectory);
  }
  return 0;
}

/*
 define arrays for default, maximum and minimum values...
*/
int doinput(lpTrajectory trajectory)
{
  lpPostElement elements;
  double tmp;
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
  tmp = post_asfloat(elements, MV_STR);
  trajectory->velocity = BRACKETDEF(tmp, MIN_MV, MAX_MV, DEF_MV);

  tmp = post_asfloat(elements, WT_STR);
  trajectory->weight = BRACKETDEF(tmp, MIN_WT, MAX_WT, DEF_WT);

  tmp = post_asfloat(elements, WX_STR);
  trajectory->wind.x = BRACKETDEF(tmp, MIN_WX, MAX_WX, DEF_WX);

  tmp = post_asfloat(elements, WY_STR);
  trajectory->wind.y = BRACKETDEF(tmp, MIN_WY, MAX_WY, DEF_WY);

  tmp = post_asfloat(elements, WZ_STR);
  trajectory->wind.z = BRACKETDEF(tmp, MIN_WZ, MAX_WZ, DEF_WZ);

  tmp = post_asfloat(elements, XC_STR);
  trajectory->chronodist = BRACKETDEF(tmp, MIN_XC, MAX_XC, DEF_XC);

  tmp = post_asfloat(elements, SH_STR);
  trajectory->sight_height = INTOFT(BRACKETDEF(tmp, MIN_SH, MAX_SH, DEF_SH));

  tmp = post_asfloat(elements, SO_STR);
  trajectory->sight_offset = INTOFT(BRACKETDEF(tmp, MIN_SO, MAX_SO, DEF_SO));

  tmp = post_asfloat(elements, SP_STR);
  trajectory->speed = MPHTOFPS(BRACKETDEF(tmp, MIN_SP, MAX_SP, DEF_SP));

  tmp = post_asfloat(elements, SA_STR);
  trajectory->speed_angle = RAD(BRACKETDEF(tmp, MIN_SA, MAX_SA, DEF_SA));

  tmp = post_asfloat(elements, LA_STR);
  trajectory->los_angle = RAD(BRACKETDEF(tmp, MIN_LA, MAX_LA, DEF_LA));

  tmp = post_asfloat(elements, CA_STR);
  trajectory->cant_angle = RAD(BRACKETDEF(tmp, MIN_CA, MAX_CA, DEF_CA));

  tmp = post_asfloat(elements, RN_STR);
  trajectory->range_min = BRACKETDEF(tmp, MIN_RN, MAX_RN, DEF_RN);

  tmp = post_asfloat(elements, RX_STR);
  trajectory->range_max = BRACKETDEF(tmp, MIN_RX, MAX_RX, DEF_RX);

  tmp = post_asfloat(elements, RI_STR);
  trajectory->range_inc = BRACKETDEF(tmp, MIN_RI, MAX_RI, DEF_RI);

  tmp = post_asfloat(elements, RZ_STR);
  trajectory->zero.x = BRACKETDEF(tmp, MIN_RZ, MAX_RZ, DEF_RZ);

  tmp = post_asfloat(elements, ZY_STR);
  trajectory->zero.y = BRACKETDEF(tmp, MIN_ZY, MAX_ZY, DEF_ZY);

  tmp = post_asfloat(elements, ZZ_STR);
  trajectory->zero.z = BRACKETDEF(tmp, MIN_ZZ, MAX_ZZ, DEF_ZZ);

  tmp = post_asfloat(elements, BC_STR);
  trajectory->bc->bc = BRACKETDEF(tmp, MIN_BC, MAX_BC, DEF_BC);

  bc_setdragfunc(trajectory->bc, post_asint(elements, DF_STR) - 1);

  tmp = post_asfloat(elements, TP_STR);
  trajectory->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  trajectory->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  trajectory->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  trajectory->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  tmp = post_asfloat(elements, AZ_STR);
  trajectory->azimuth = RAD(BRACKETDEF(tmp, MIN_AZ, MAX_AZ, DEF_AZ)/60.0);

  tmp = post_asfloat(elements, EL_STR);
  trajectory->elevation = RAD(BRACKETDEF(tmp, MIN_EL, MAX_EL, DEF_EL)/60.0);

  chk = post_ascheck(elements, AZC_STR);
  options_setoption(trajectory->options, TRAJ_OPT_AZIM, chk);
  chk = post_ascheck(elements, ELC_STR);
  options_setoption(trajectory->options, TRAJ_OPT_ELEV, chk);
  chk = post_ascheck(elements, ALC_STR);
  options_setoption(trajectory->options, TRAJ_OPT_ALTI, chk);

  chk = post_ascheck(elements, BS_STR);
  options_setoption(trajectory->options, TRAJ_OPT_BASIC, chk);

  switch (post_asint(elements, UN_STR))
  {
    case 1 :
      options_setoption(trajectory->options, TRAJ_OPT_IN, 1);
      break;
    case 2 :
      options_setoption(trajectory->options, TRAJ_OPT_MOA, 1);
      break;
    case 3 :
      options_setoption(trajectory->options, TRAJ_OPT_MIL, 1);
      break;
  }

  free(elements);

  if (trajectory->range_min > trajectory->range_max) 
    return ERROR_TRAJ_INVALIDRANGEMIN;

  return ERROR_TRAJ_NOERROR;
}

/* writes the input data to the web page... */
void writedata(lpTrajectory trajectory)
{
  double    tmp;
  lpOptions o;
  int       basic;

  o = trajectory->options;
  basic = options_getoption(o, TRAJ_OPT_BASIC);

  html_writeheader("Input Data", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Muzzle Velocity:", " ft/sec",   
    "%6.1f", trajectory->velocity);
  if (!basic) 
    html_writetablevalue("Chronograph Distance:", " feet",   
      "%6.3f", trajectory->chronodist);

  html_writetablevalue("Ballistic Coefficient:", " ",   
    "%5.3f", trajectory->bc->bc);
  html_writetablestring("Drag Function:", "", "%s", 
    dragstr[trajectory->bc->dragfunc]);
  html_writetablevalue("Bullet Weight:", " grains",   
    "%5.0f", trajectory->weight);

  html_writetablevalue("Sight Height:", " inches",   
    "%5.2f", FTTOIN(trajectory->sight_height));
  if (!basic) 
    html_writetablevalue("Sight Offset:", " inches",   
      "%5.2f", FTTOIN(trajectory->sight_offset));

  if (!basic) 
  {
    html_writetablevalue("Wind Range Speed:", " mph",   
      "%4.0f", trajectory->wind.x);
    html_writetablevalue("Wind Vertical Speed:", " mph",   
      "%4.0f", trajectory->wind.y);
  }
  html_writetablevalue("Wind Cross Speed:", " mph",   
    "%4.0f", trajectory->wind.z);

  html_writetablevalue("LOS Angle:", " degrees",   
    "%5.0f", DEG(trajectory->los_angle));
  if (!basic)
    html_writetablevalue("Cant Angle:", " degrees",   
      "%5.0f", DEG(trajectory->cant_angle));

  html_writetablevalue("Target Speed:", " mph",   
    "%5.0f", FPSTOMPH(trajectory->speed));
  if (!basic)
    html_writetablevalue("Target Angle:", " degrees",   
      "%5.0f", DEG(trajectory->speed_angle));

  if (options_getoption(o, TRAJ_OPT_AZIM) || 
      options_getoption(o, TRAJ_OPT_ELEV))
  {
    html_writetablevalue("Zero Range:", 
      "yards", "%6.0f", trajectory->zero.x);
    if (!basic)
    {
      html_writetablevalue("Zero Height:", 
        "inches", "%6.0f", trajectory->zero.y);
      html_writetablevalue("Zero Offset:", 
        "inches", "%6.0f", trajectory->zero.z);
    }
  }

  html_writetablevalue("Temperature:", " °F",   
    "%5.1f", trajectory->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", " in Hg",    
    "%5.2f", trajectory->atmos->pressure);
  html_writetablevalue("Relative Humidity:", " %",   
    "%5.1f", trajectory->atmos->humidity);
  html_writetablevalue("Altitude:", " feet",   
    "%6.0f", trajectory->atmos->altitude);
  tmp = 100.0*trajectory->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", "% of Sea Level", "%5.0f", tmp);
  html_closetable();

  html_break();
}

/* writes the calculated data to the web page... */
void writetable(lpTrajectory trajectory)
{
  double    x;
  int       c, i, k;
  lpOptions o;

  o = trajectory->options;

  /* write the trajectory data... */
  html_writeheader("Calculated Table", 3);
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  if (options_getoption(o, TRAJ_OPT_MIL))
  {
    html_writetablevalue("Elevation:", " mil",   
      "%6.3f", MIL(trajectory->elevation));
    html_writetablevalue("Azimuth:",   " mil",   
      "%6.3f", MIL(trajectory->azimuth));
  }
  else
  {
    html_writetablevalue("Elevation:", " moa",   
      "%6.3f", MOA(trajectory->elevation));
    html_writetablevalue("Azimuth:",   " moa",   
      "%6.3f", MOA(trajectory->azimuth));
  }

  html_closetable();
  html_break();
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  for (i = 0; i < TRAJ_NUMCOLUMNS; i++) 
    html_tableheader(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, HEADERSTR[i]);
  html_tablecloserow();

  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  if (options_getoption(o, TRAJ_OPT_MOA))
    for (i = 0; i < TRAJ_NUMCOLUMNS; i++) 
      html_tableheader(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, MOAHEADERUNITS[i]);

  else if (options_getoption(o, TRAJ_OPT_MIL))
    for (i = 0; i < TRAJ_NUMCOLUMNS; i++) 
      html_tableheader(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, MILHEADERUNITS[i]);
  else
    for (i = 0; i < TRAJ_NUMCOLUMNS; i++) 
      html_tableheader(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, INHEADERUNITS[i]);
  html_tablecloserow();

  c = (trajectory->range_max - trajectory->range_min)/trajectory->range_inc;
  for (i = 0; i <= c; i++)
  {

    html_tableopenrow(HTML_TABLE_ALIGN_NONE);
    html_writetabledata(TABLEFORMATS[0], trajectory->ranges[i].range);
    html_writetabledata(TABLEFORMATS[1], trajectory->ranges[i].velocity);
    html_writetabledata(TABLEFORMATS[2], trajectory->ranges[i].energy);
    html_writetabledata(TABLEFORMATS[3], trajectory->ranges[i].momentum);
    if (options_getoption(o, TRAJ_OPT_IN))
    {
      html_writetabledata(TABLEFORMATS[4], 
        FTTOIN(trajectory->ranges[i].drop));
      html_writetabledata(TABLEFORMATS[5], 
        FTTOIN(trajectory->ranges[i].windage));
      html_writetabledata(TABLEFORMATS[6], 
        FTTOIN(trajectory->ranges[i].lead));
    }
    else
    {
      k = trajectory->range_min + i*trajectory->range_inc;
      if (k)
      {
        x = 1.0/YRDTOFT(k);
        if (options_getoption(o, TRAJ_OPT_MIL))
        {
          html_writetabledata(TABLEFORMATS[4], 
            MIL(trajectory->ranges[i].drop*x));
          html_writetabledata(TABLEFORMATS[5], 
            MIL(trajectory->ranges[i].windage*x));
          html_writetabledata(TABLEFORMATS[6], 
            MIL(trajectory->ranges[i].lead*x));
        }
        else if (options_getoption(o, TRAJ_OPT_MOA))
        {
          html_writetabledata(TABLEFORMATS[4], 
            MOA(trajectory->ranges[i].drop*x));
          html_writetabledata(TABLEFORMATS[5], 
            MOA(trajectory->ranges[i].windage*x));
          html_writetabledata(TABLEFORMATS[6], 
            MOA(trajectory->ranges[i].lead*x));
        }
      }
      else
      {
        html_tabledata(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, "---");
        html_tabledata(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, "---");
        html_tabledata(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, "---");
      }
    }
    html_writetabledata(TABLEFORMATS[7], trajectory->ranges[i].time);
    html_tablecloserow();
  }
  html_closetable();
}

/* main output routine... */
int dooutput(lpTrajectory trajectory, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_traj[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();

    setup_info(TRAJ_VERSION);

    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(trajectory);

    html_horzrule(NULL, 0, 0, 0, 0);

    writetable(trajectory);

    html_closebody();
    html_closehead();
  }
  return 0;
}

