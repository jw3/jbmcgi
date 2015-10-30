
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
#include "setup.h"
#include "errors.h"
#include "mcdrag.h"
#include "utility.h"

/*
  version
*/
#define DRAG_VERSION   "jbmdrag, GNU version 2.0, March 2000"  

/*
 define default and minimum and maximum values...
*/
#define   DEF_DIA      0.308
#define   DEF_NSL      0.740/DEF_DIA
#define   DEF_DMP      0.062/DEF_DIA
#define   DEF_RAD      15.0
#define   DEF_BTL      0.18/DEF_DIA
#define   DEF_DBA      0.251/DEF_DIA
#define   DEF_TLN      1.464/DEF_DIA
#define   DEF_DBD      0.308/DEF_DIA
#define   DEF_WGT      210.0
#define   DEF_TWT      10
#define   DEF_SPG      10.7
#define   DEF_TP       59.0
#define   DEF_PR       29.92
#define   DEF_HU       0.0
#define   DEF_AL       0.0

#define   MIN_DIA      0.1
#define   MIN_NSL      0.1
#define   MIN_DMP      0.0
#define   MIN_RAD      1.0
#define   MIN_BTL      0.0
#define   MIN_DBA      0.1
#define   MIN_TLN      0.1
#define   MIN_DBD      0.1
#define   MIN_WGT      10.0
#define   MIN_TWT      2.0
#define   MIN_SPG      2.0
#define   MIN_TP       -40.0
#define   MIN_PR       15.0
#define   MIN_HU       0.0
#define   MIN_AL       -1000.0

#define   MAX_DIA      2.0
#define   MAX_NSL      6.0
#define   MAX_DMP      1.0
#define   MAX_RAD      1000.0
#define   MAX_BTL      2.0
#define   MAX_DBA      1.0
#define   MAX_TLN      10.0
#define   MAX_DBD      1.2
#define   MAX_WGT      5000.0
#define   MAX_TWT      100.0
#define   MAX_SPG      20.0
#define   MAX_TP       140.0
#define   MAX_PR       40.0
#define   MAX_HU       100.0
#define   MAX_AL       15000.0

/*
 define search strings to be used...
*/
#define   DIA_STR  "dia"
#define   NSL_STR  "nsl"
#define   DMP_STR  "dmp"
#define   RAD_STR  "rad"
#define   BTL_STR  "btl"
#define   DBA_STR  "dba"
#define   TLN_STR  "tln"
#define   DBD_STR  "dbd"
#define   WGT_STR  "wgt"
#define   TWT_STR  "twt"
#define   SPG_STR  "spg"
#define   DGF_STR  "dgf"
#define   NST_STR  "nst"
#define   TP_STR   "tp"
#define   PR_STR   "pr"
#define   HU_STR   "hu"
#define   AL_STR   "al"
#define   ALC_STR  "alc"
#define   SHC_STR  "shc"

/* defines the maximum size of the buffer used in output... */
#define  NUMCOLUMNS 13

static char *headerstr[] =
  {"Velocity", "Mach", "CD", "CD", "CD", "CD",
   "CD", "CD", "Form", "BC", "Twist", "Stability", "RPM"};

static char *headerunits[] =
  {"(ft/sec)", "Number", "Nose", "Bttl", "Base", "Band",
   "Skin", "", "Factor", "(%s)", "(inches)", "", ""};

static char *tableformats[] =
  {"%6.0f", "%5.3f", "%5.3f", "%5.3f", "%5.3f", "%5.3f", "%5.3f",
   "%5.3f", "%5.3f", "%5.3f", "%5.2f", "%5.3f", "%6.0f"};

int main()
{
  int      error;
  lpMcDrag mcdrag;

  html_setfile(stdout);

  mcdrag = mcdrag_create();
  if (mcdrag)
  {
    error = doinput(mcdrag);
    if (!error) mcdrag_calculate(mcdrag);
    dooutput(mcdrag, error);
    free(mcdrag);
  }
  return 0;
}

int doinput(lpMcDrag mcdrag)
{
  lpPostElement elements;
  double dia, tmp;
  int err;

  elements = post_processelements(&err);
  if (err < 0) return err;

  /*
    Get setup stuff
  */
  setup_input(elements);

  /*
   check for input values...
  */
  dia = post_asfloat(elements, DIA_STR);
  dia = BRACKETDEF(dia, MIN_DIA, MAX_DIA, DEF_DIA);
  mcdrag->dia = dia;

  mcdrag->nsl = post_asfloat(elements, NSL_STR)/dia;
  mcdrag->nsl = BRACKETDEF(mcdrag->nsl, MIN_NSL, MAX_NSL, DEF_NSL);

  mcdrag->dmp = post_asfloat(elements, DMP_STR)/dia;
  mcdrag->dmp = BRACKETDEF(mcdrag->dmp, MIN_DMP, MAX_DMP, DEF_DMP);

  mcdrag->rad = post_asfloat(elements, RAD_STR);
  mcdrag->rad = BRACKETDEF(mcdrag->rad, MIN_RAD, MAX_RAD, DEF_RAD);

  mcdrag->btl = post_asfloat(elements, BTL_STR)/dia;
  mcdrag->btl = BRACKETDEF(mcdrag->btl, MIN_BTL, MAX_BTL, DEF_BTL);

  mcdrag->dba = post_asfloat(elements, DBA_STR)/dia;
  mcdrag->dba = BRACKETDEF(mcdrag->dba, MIN_DBA, MAX_DBA, DEF_DBA);

  mcdrag->tln = post_asfloat(elements, TLN_STR)/dia;
  mcdrag->tln = BRACKETDEF(mcdrag->tln, MIN_TLN, MAX_TLN, DEF_TLN);

  mcdrag->dbd = post_asfloat(elements, DBD_STR)/dia;
  mcdrag->dbd = BRACKETDEF(mcdrag->dbd, MIN_DBD, MAX_DBD, DEF_DBD);

  mcdrag->wgt = post_asfloat(elements, WGT_STR);
  mcdrag->wgt = BRACKETDEF(mcdrag->wgt, MIN_WGT, MAX_WGT, DEF_WGT);

  mcdrag->twt = post_asfloat(elements, TWT_STR);
  mcdrag->twt = BRACKETDEF(mcdrag->twt, MIN_TWT, MAX_TWT, DEF_TWT);

  mcdrag->spg = post_asfloat(elements, SPG_STR);
  mcdrag->spg = BRACKETDEF(mcdrag->spg, MIN_SPG, MAX_SPG, DEF_SPG);

  mcdrag->dgf = post_asint(elements, DGF_STR) - 1;
  mcdrag->dgf = BRACKETDEF(mcdrag->dgf, g1, gi, g1); 
  mcdrag->nst = post_asint(elements, NST_STR) - 1;
  mcdrag->nst = BRACKETDEF(mcdrag->nst, secant, conical, secant); 

  tmp = post_asfloat(elements, TP_STR);
  mcdrag->atmos->temperature = BRACKETDEF(tmp, MIN_TP, MAX_TP, DEF_TP);

  tmp = post_asfloat(elements, PR_STR);
  mcdrag->atmos->pressure = BRACKETDEF(tmp, MIN_PR, MAX_PR, DEF_PR);

  tmp = post_asfloat(elements, HU_STR);
  mcdrag->atmos->humidity = BRACKETDEF(tmp, MIN_HU, MAX_HU, DEF_HU);

  tmp = post_asfloat(elements, AL_STR);
  mcdrag->atmos->altitude = BRACKETDEF(tmp, MIN_AL, MAX_AL, DEF_AL);

  mcdrag->alc = post_ascheck(elements, ALC_STR);
  mcdrag->shc = post_ascheck(elements, SHC_STR);

  post_destroyelements(elements);

  if (mcdrag->dba > 1.0) return ERROR_DRAG_INVALIDDBA;
  if (mcdrag->dmp > 0.5) return ERROR_DRAG_INVALIDDMP;
  if (mcdrag->nsl < 1.0) return ERROR_DRAG_INVALIDNSL;
  if (mcdrag->rad < 1.0) return ERROR_DRAG_INVALIDRAD;
  if (mcdrag->tln < mcdrag->nsl + mcdrag->btl) return ERROR_DRAG_INVALIDTLN;
  return ERROR_DRAG_NOERROR;
}

/* writes the input data to the web page... */
void writedata(lpMcDrag mcdrag)
{
  double dia, tmp;
  html_writeheader("Input Data", 3);
  dia = mcdrag->dia;
  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);
  html_writetablevalue("Caliber :", " inches", "%5.3f", mcdrag->dia);
  html_writetablevalue("Rotating Band Diameter:", 
    " inches", "%5.3f", dia*mcdrag->dbd);
  html_writetablevalue("Total Length:", " inches", "%5.3f", dia*mcdrag->tln);
  html_writetablevalue("Nose Length:", " inches", "%5.3f", dia*mcdrag->nsl);
  html_writetablestring("Nose Type", "", " %s",   nosetypes[(int)mcdrag->nst]);
  html_writetablevalue("Meplat Diameter:", " inches", "%5.3f", dia*mcdrag->dmp);
  html_writetablevalue("Nose Radius:", " cal",    "%5.3f", mcdrag->rad);
  html_writetablevalue("Boattail Length:", " inches", "%5.3f", dia*mcdrag->btl);
  html_writetablevalue("Base Diameter:", " inches", "%5.3f", dia*mcdrag->dba);
  html_writetablevalue("Weight:", " grains", "%6.0f", mcdrag->wgt);
  html_writetablevalue("Twist:", " inches", "%5.1f", mcdrag->twt);
  html_writetablevalue("Specific Gravity:", "", "%5.2f", mcdrag->spg);

  html_writetablestring("Drag Function", "", " %s", dragstr[(int)mcdrag->dgf]);
  html_writetablevalue("Temperature:", " °F", 
    "%5.1f", mcdrag->atmos->temperature);
  html_writetablevalue("Barometric Pressure:", " in Hg", 
    "%5.2f", mcdrag->atmos->pressure);
  html_writetablevalue("Relative Humidity:", " %", 
    "%5.1f", mcdrag->atmos->humidity);
  html_writetablevalue("Altitude:", " feet", "%6.1f", mcdrag->atmos->altitude);
  tmp = 100.0*mcdrag->atmos->density/ATMOS_DENSSTD;
  html_writetablevalue("Air Density:", "% of Sea Level", "%5.0f", tmp);
  html_closetable();
}

void writecolumnheaders(lpMcDrag mcdrag)
{
  int i, shc;
  char tmp[32];
  shc = mcdrag->shc;

  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  for (i = 0; i < NUMCOLUMNS; i++)
    if ((shc) || (i < 2 || i > 6))
      html_tableheader(HTML_TABLE_ALIGN_NONE, 1, 1, 0, headerstr[i]);
  html_tablecloserow();
  html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  for (i = 0; i < NUMCOLUMNS; i++)
  {
    if ((shc) || (i < 2 || i > 6))
    {
      if (i == 9)
      {
        sprintf(tmp, headerunits[i], dragstr[(int)mcdrag->dgf]);
        html_tableheader(HTML_TABLE_ALIGN_NONE, 1, 1, 0, tmp);
      }
      else html_tableheader(HTML_TABLE_ALIGN_NONE, 1, 1, 0, headerunits[i]);
    }
  }
  html_tablecloserow();
}

/* writes the calculated data to the web page... */
void writetable(lpMcDrag mcdrag)
{
  int i;


  /* write the trajectory data... */
  html_writeheader("Calculated Table", 3);

  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  html_writetablevalue("Tangent Radius / Radius:", "", "%5.3f", mcdrag->rtr);
  html_writetablevalue("Sectional Density:", "", "%5.3f", mcdrag->sds);
  html_closetable();
  html_break();

  html_opentable(HTML_TABLE_ALIGN_NONE, 0, 0);

  writecolumnheaders(mcdrag);
  for (i = 0; i < DRAG_VELCOUNT; i++)
  {
    html_tableopenrow(HTML_TABLE_ALIGN_NONE);
    html_writetabledata(tableformats[0], mcdrag->data[i].velocity);
    html_writetabledata(tableformats[1], mcdrag->data[i].mach);
    if (mcdrag->shc)
    {
      html_writetabledata(tableformats[2], mcdrag->data[i].cd_nose);
      html_writetabledata(tableformats[3], mcdrag->data[i].cd_bttl);
      html_writetabledata(tableformats[4], mcdrag->data[i].cd_base);
      html_writetabledata(tableformats[5], mcdrag->data[i].cd_rbnd);
      html_writetabledata(tableformats[6], mcdrag->data[i].cd_skin);
    }
      html_writetabledata(tableformats[7], mcdrag->data[i].cd_total);
      html_writetabledata(tableformats[8], mcdrag->data[i].form);
      html_writetabledata(tableformats[9], mcdrag->data[i].bc);
      html_writetabledata(tableformats[10], mcdrag->data[i].twt);
      html_writetabledata(tableformats[11], mcdrag->data[i].stab);
      html_writetabledata(tableformats[12], mcdrag->data[i].rpm);
      html_tablecloserow();
  }
  html_closetable();
}

/* main output routine... */
int dooutput(lpMcDrag mcdrag, int error)
{
  if (error) posterr_error(error, (error > 0) ? errors_drag[error] : NULL);
  else
  {
    html_cgitext();
    setup_openhead();
    setup_body();
    setup_info(DRAG_VERSION);
    setup_header(2);
    html_horzrule(NULL, 0, 0, 0, 0);

    writedata(mcdrag);

    html_horzrule(NULL, 0, 0, 0, 0);

    writetable(mcdrag);
   
    html_closebody();
    html_closehead();
  }
  return 0;
}

