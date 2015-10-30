
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
#include <stdlib.h>

#include "mcdrag.h"
#include "utility.h"

lpMcDrag mcdrag_create(void)
{
  lpMcDrag mcdrag;
  mcdrag = malloc(sizeof(McDrag));  
  mcdrag->atmos = atmos_create();
  mcdrag->drag = drag_cd_g1;
  return mcdrag;
}

double mcdrag_getrtr(lpMcDrag mcdrag)
{
  double x0, y0, b, h, tns;
  switch (mcdrag->nst)
  {
    case tangent: return 1.0;
    case conical: return 0.0;
    case secant:
      h   = sqrt(SQR(mcdrag->nsl) + SQR(0.5 - 0.5*mcdrag->dmp));
      b   = acos(0.5*h/mcdrag->rad) - acos(mcdrag->nsl/h);
      x0  = mcdrag->rad*cos(b);
      y0  = mcdrag->rad*sin(b);
      tns = mcdrag->nsl + sqrt(SQR(mcdrag->rad) - 
            SQR(y0 - 0.5*mcdrag->dmp)) - x0;
    return (SQR(tns) + 0.25)/mcdrag->rad;
  }
  return 0.0;
}

double mcdrag_cd_nose(lpMcDrag mcdrag, double mach)
{
  double tau, xmc, m2, be, ze, ptp, nd;
  double c1, c2, c3, c4;
  double cmep, cdhm, cdht, ssmc, chi;
  m2  = mach*mach;
  tau = (1.0 - mcdrag->dmp)/mcdrag->nsl;
  chi = (m2-1.0)/(2.4*m2);

  if (mach <= 1.0) 
    ptp = pow(1.0 + 0.2*m2,3.5);
  else 
    ptp = pow(1.2*m2, 3.5)*pow(6.0/(7.0*m2 - 1.0), 2.5);
  cmep = 1.122*(ptp - 1.0)*SQR(mcdrag->dmp)/m2;

  if (mach <= 0.91) 
    cdhm = 0.0;
  else if (mach >= 1.41) 
    cdhm = 0.85*cmep;
  else 
    cdhm = (0.254 + 2.88*chi)*cmep;

  if (mach <= 1.0)
  {
    xmc = 1.0/sqrt(1.0 + 0.552*pow(tau, 0.8));
    if (mach <= xmc) 
      nd = cdhm;
    else 
      nd = 0.368*pow(tau, 1.8) + 1.6*tau*chi + cdhm;
  }
  else
  {
    c1 = 0.7156 - mcdrag->rtr*(0.5313 - 0.595*mcdrag->rtr);
    c2 = 0.0796 + 0.0779*mcdrag->rtr;
    c3 = 1.587 + 0.049*mcdrag->rtr;
    c4 = 0.1122 + 0.1658*mcdrag->rtr;

    be = sqrt(m2 - 1.0);
    ssmc = 1.0 + 0.368*pow(tau, 1.85);
    if (mach < ssmc) 
      ze = sqrt(ssmc*ssmc - 1.0);
    else 
      ze = be;
    cdht = (c1 - c2*tau*tau)/(ze*ze)*pow(tau*ze, c3 + c4*tau);
    nd = cdht + cdhm;
  }
  return (nd < 0.0 ? 0.0 : nd);
}

double mcdrag_cd_bttl(lpMcDrag mcdrag, double mach)
{
  double r5, e2, e3, x3, bbt;
  double a11, a12, a1, a2;
  double chi, tb, tb2, fbt;
  double m2, b, b3, tau, btld;
  m2 = mach*mach;
  tau = (1.0 - mcdrag->dmp)/mcdrag->nsl;

  chi = (m2 - 1.0)/(2.4*m2);
  if ((mach <= 0.85) || (mcdrag->btl == 0.0)) btld = 0.0;
  else if (mach <= 1.101)
  {
    tb = (1.0 - mcdrag->dba)/(2.0*mcdrag->btl);
    tb2 = 2.0*tb*tb + (tb*tb*tb);
    fbt = exp(-2.0*mcdrag->btl);
    bbt = 1.0 - fbt + 2.0*tb*((fbt*(mcdrag->btl + 0.5)) - 0.5);
    if (mach <= 1.0)
      btld = 2.0*tb2*bbt/(0.564 + 1250.0*chi*chi);
    else
      btld = 2.0*tb2*bbt*(1.774 - 9.3*chi);
  }
  else
  {
    tb = (1.0 - mcdrag->dba)/(2.0*mcdrag->btl);
    b = sqrt(m2-1.0);
    b3 = 0.85/b;
    a12 = (5.0*tau)/(6.0*b) + pow(0.5*tau, 2.0) -
          (0.7435/m2)*pow(tau*mach, 1.6);
    a11 = (1.0-(0.6*mcdrag->rtr/mach))*a12;
    e2 = exp((-1.1952/mach)*(mcdrag->tln - mcdrag->nsl - mcdrag->btl));
    x3 = ((2.4*m2*m2 - 4.0*b*b)*tb*tb)/(2.0*b*b*b*b);
    a1 = a11*e2 - x3+(2.0*tb/b);
    r5 = 1.0/b3;
    e3 = exp(-b3*mcdrag->btl);
    a2 = 1.0 - e3 + (2.0*tb*(e3*(mcdrag->btl + r5) - r5));
    btld = 4.0*a1*tb*a2*r5;
  }
  return (btld < 0.0 ? 0.0 : btld);
}

double mcdrag_cd_base(lpMcDrag mcdrag, double mach)
{
  double pb2, pb4, m2, bd;
  m2 = mach*mach;
  if (mach < 1.0) 
    pb2 = 1.0/(1.0 + m2*(0.1875 + 0.0531*m2));
  else 
    pb2 = 1.0/(1.0 + m2*(0.2477 + 0.0345*m2));
  pb4 = (1.0 + 0.09*m2*(1.0 - exp(mcdrag->nsl - 
        mcdrag->tln)))*(1.0 + 0.25*m2*(1.0 - mcdrag->dba));
  bd = 1.4286*(1.0 - pb2*pb4)*SQR(mcdrag->dba)/m2;
  return (bd < 0.0 ? 0.0 : bd);
}

double mcdrag_cd_rbnd(lpMcDrag mcdrag, double mach)
{
  double rbd;
  if (mach < 0.85) 
    rbd = pow(mach, 12.5)*(mcdrag->dbd - 1.0);
  else 
    rbd = (0.21+0.28/mach*mach)*(mcdrag->dbd - 1.0);
  return (rbd < 0.0 ? 0.0 : rbd);
}

double mcdrag_cd_skin(lpMcDrag mcdrag, double mach)
{
  double re, nsl2, m2, dum;
  double swn, swcyl, sw, skin;
  double cft, cfl, cdsfl, cdsft;
  m2 = mach*mach;
  nsl2 = mcdrag->nsl*mcdrag->nsl;

  dum = 1.0 + ((0.333 + (0.02/nsl2))*mcdrag->rtr);
  swn = 1.5708*mcdrag->nsl*dum*(1.0 + 1.0/(8.0*nsl2));
  swcyl = M_PI*(mcdrag->tln - mcdrag->nsl);
  sw = swn + swcyl;

  re = 591726.02*mach*mcdrag->tln*mcdrag->dia;
  cfl = (1.328/sqrt(re))*pow(1.0 + 0.12*m2, -0.12);
  cft = (0.455/pow(log10(re), 2.58))*pow(1.0 + 0.21*m2, -0.32);
  switch (mcdrag->blc)
  {
    case laminar_laminar:
      cdsfl = 1.2732*cfl*sw;
      cdsft = cdsfl;
      break;
    case laminar_turbulent:
      cdsfl = 1.2732*cfl*sw;
      cdsft = 1.2732*cft*sw;
      break;
    case turbulent_turbulent:
      cdsfl = 1.2732*cft*sw;
      cdsft = cdsfl;
      break;
  }
  skin = (cdsfl*swn + cdsft*swcyl)/sw;
  return (skin < 0.0 ? 0.0 : skin);
}

double mcdrag_cd_total(lpMcDrag mcdrag, double mach)
{
  double total;
  total = mcdrag_cd_nose(mcdrag, mach);
  total += mcdrag_cd_bttl(mcdrag, mach);
  total += mcdrag_cd_base(mcdrag, mach);
  total += mcdrag_cd_rbnd(mcdrag, mach);
  total += mcdrag_cd_skin(mcdrag, mach);
  return total;
}

double mcdrag_twist(lpMcDrag mcdrag, double mach, double *stability)
{
  double a, a1, b, b1, s1, s2, g1;
  double gt, m2, st;
  m2 = mach*mach;
  s1 = sqrt(1.0 - mcdrag->dmp);
  s2 = 1.0 - SQR(mcdrag->dba);
  if (mach <= 0.95)
    g1 = 20.082 + 3.726*(mach/sqrt(1.0 - m2));
  else if (mach >= 1.1)
    g1 = 35.079 - 24.066*(sqrt(m2 - 1.0)/mach);
  else
    g1 = 71.73001 - 42.433*mach;
  a1 = g1*s2;
  if (mach < 1.0)
  {
    b1 = sqrt(1.0 - m2);
    b = 0.82112 + 0.36971*b1;
    a = 34.779 + (24.091 + (8.977  + mcdrag->rtr*(-12.804 + 
        8.38*mcdrag->rtr))*mcdrag->nsl)*s1*b1 - a1;
  }
  else
  {
    b1 = sqrt(m2 - 1.0)/mach;
    b = 1.0528 + 0.23379*b1 - 0.004884*(mach - 1.0);
    a = 58.873 + (8.115 + (14.15  + mcdrag->rtr*(-15.348 + 
        7.216*mcdrag->rtr))*mcdrag->nsl)*s1*b1*b1 - a1;
  }
  gt = mcdrag->dia*a*sqrt(mcdrag->spg)/pow(mcdrag->tln, b);
  if (mcdrag->twt != 0.0) 
    st = 1.5*SQR(gt/mcdrag->twt);
  else 
    st = 0.0;

  mcdrag->max_stab = MAX(mcdrag->max_stab, st);
  mcdrag->min_stab = MIN(mcdrag->min_stab, st);
  mcdrag->max_twt  = MAX(mcdrag->min_stab, gt);
  mcdrag->min_twt  = MIN(mcdrag->min_stab, gt);

  *stability = st;
  return gt;
}

double mcdrag_rpm(double twist, double velocity)
{
  return FTTOIN(velocity/(twist/60.0));
}

void mcdrag_setdrag(lpMcDrag mcdrag)
{
  switch (mcdrag->dgf)
  {
    case g1 :
      mcdrag->drag = drag_cd_g1;
      break;
    case g2 :
      mcdrag->drag = drag_cd_g2;
      break;
    case g5 :
      mcdrag->drag = drag_cd_g5;
      break;
    case g6 :
      mcdrag->drag = drag_cd_g6;
      break;
    case g7 :
      mcdrag->drag = drag_cd_g7;
      break;
    case g8 :
      mcdrag->drag = drag_cd_g8;
      break;
    case gl :
      mcdrag->drag = drag_cd_gl;
      break;
    case gi :
      mcdrag->drag = drag_cd_gi;
      break;
  }
}

void mcdrag_calculate(lpMcDrag mcdrag)
{
  McDragData data;
  double m, mch, v;
  int    i = 0;
  mcdrag_setdrag(mcdrag);
  if (mcdrag->alc) atmos_standardalt(mcdrag->atmos);
  atmos_atmos(mcdrag->atmos);
  mcdrag->blc = laminar_turbulent;
  mcdrag->rtr = mcdrag_getrtr(mcdrag);
  mcdrag->sds = mcdrag->wgt/(7000.0*SQR(mcdrag->dia));
  mcdrag->max_stab = -10000.0;
  mcdrag->min_stab =  10000.0;
  mcdrag->max_twt  = -10000.0;
  mcdrag->min_twt  =  10000.0;
  mch = mcdrag->atmos->mach;
  for (i = 0; i < DRAG_VELCOUNT; i++)
  {
    v = velocities[i];
    m = v/mch;
    data.velocity = v;
    data.mach     = m;
    data.cd_nose  = mcdrag_cd_nose(mcdrag, m);
    data.cd_bttl  = mcdrag_cd_bttl(mcdrag, m);
    data.cd_base  = mcdrag_cd_base(mcdrag, m);
    data.cd_rbnd  = mcdrag_cd_rbnd(mcdrag, m);
    data.cd_skin  = mcdrag_cd_skin(mcdrag, m);
    data.cd_total = data.cd_nose + data.cd_bttl + 
                    data.cd_base + data.cd_rbnd + data.cd_skin;
    data.form     = data.cd_total/mcdrag->drag(m);
    data.bc       = mcdrag->sds/data.form;
    data.twt      = mcdrag_twist(mcdrag, m, &data.stab);
    data.rpm      = mcdrag_rpm(mcdrag->twt, v);
    mcdrag->data[i] = data;
  }
}
