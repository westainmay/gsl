/* Author: G. Jungman
 * RCS: $Id$
 */
#include <config.h>
#include <gsl_math.h>
#include <gsl_errno.h>
#include <gsl_precision.h>
#include "gsl_sf_ellint.h"


/*-*-*-*-*-*-*-*-*-*-*-* Private Section *-*-*-*-*-*-*-*-*-*-*-*/

inline
static double locMAX3(double x, double y, double z)
{
  double xy = GSL_MAX(x, y);
  return GSL_MAX(xy, z);
}

inline
static double locMAX4(double x, double y, double z, double w)
{
  double xy  = GSL_MAX(x,  y);
  double xyz = GSL_MAX(xy, z);
  return GSL_MAX(xyz, w);
}


/*-*-*-*-*-*-*-*-*-*-*-* (semi)Private Implementations *-*-*-*-*-*-*-*-*-*-*-*/


/* based on Carlson's algorithms:
   [B. C. Carlson Numer. Math. 33, 1 (1979)]
   
   see also:
   [B.C. Carlson, Special Functions of Applied Mathematics (1977)]
 */

/* According to Carlson's algorithm, the errtol parameter
   typically effects the relative error in the following way:

   relative error < 16 errtol^6 / (1 - 2 errtol)

     errtol     precision
     ------     ----------
     0.001       1.0e-17
     0.003       2.0e-14 
     0.01        2.0e-11
     0.03        2.0e-8
     0.1         2.0e-5
*/


int
gsl_sf_ellint_RC_impl(double x, double y, gsl_mode_t mode, gsl_sf_result * result)
{
  const double lolim = 5.0 * GSL_DBL_MIN;
  const double uplim = 0.2 * GSL_DBL_MAX;
  const gsl_prec_t goal = GSL_MODE_PREC(mode);
  const double errtol = ( goal == GSL_PREC_DOUBLE ? 0.001 : 0.03 );
  const double prec   = gsl_prec_eps[goal];

  if(x < 0.0 || y < 0.0 || x + y < lolim) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(GSL_MAX(x, y) < uplim) { 
    const double c1 = 1.0 / 7.0;
    const double c2 = 9.0 / 22.0;
    double xn = x;
    double yn = y;
    double mu, sn, lamda, s;
    while(1) {
      mu = (xn + yn + yn) / 3.0;
      sn = (yn + mu) / mu - 2.0;
      if (fabs(sn) < errtol) break;
      lamda = 2.0 * sqrt(xn) * sqrt(yn) + yn;
      xn = (xn + lamda) * 0.25;
      yn = (yn + lamda) * 0.25;
    }
    s = sn * sn * (0.3 + sn * (c1 + sn * (0.375 + sn * c2)));
    result->val = (1.0 + s) / sqrt(mu);
    result->err = prec * fabs(result->val);
    return GSL_SUCCESS;
  }
  else {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
}


int
gsl_sf_ellint_RD_impl(double x, double y, double z, gsl_mode_t mode, gsl_sf_result * result)
{
  const gsl_prec_t goal = GSL_MODE_PREC(mode);
  const double errtol = ( goal == GSL_PREC_DOUBLE ? 0.001 : 0.03 );
  const double prec   = gsl_prec_eps[goal];
  const double lolim = 2.0/pow(GSL_DBL_MAX, 2.0/3.0);
  const double uplim = pow(0.1*errtol/GSL_DBL_MIN, 2.0/3.0);

  if(GSL_MIN(x,y) < 0.0 || GSL_MIN(x+y,z) < lolim) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(locMAX3(x,y,z) < uplim) {
    const double c1 = 3.0 / 14.0;
    const double c2 = 1.0 /  6.0;
    const double c3 = 9.0 / 22.0;
    const double c4 = 3.0 / 26.0;
    double xn = x;
    double yn = y;
    double zn = z;
    double sigma  = 0.0;
    double power4 = 1.0;
    double ea, eb, ec, ed, ef, s1, s2;
    double mu, xndev, yndev, zndev;
    while(1) {
      double xnroot, ynroot, znroot, lamda;
      double epslon;
      mu = (xn + yn + 3.0 * zn) * 0.2;
      xndev = (mu - xn) / mu;
      yndev = (mu - yn) / mu;
      zndev = (mu - zn) / mu;
      epslon = locMAX3(fabs(xndev), fabs(yndev), fabs(zndev));
      if (epslon < errtol) break;
      xnroot = sqrt(xn);
      ynroot = sqrt(yn);
      znroot = sqrt(zn);
      lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
      sigma  += power4 / (znroot * (zn + lamda));
      power4 *= 0.25;
      xn = (xn + lamda) * 0.25;
      yn = (yn + lamda) * 0.25;
      zn = (zn + lamda) * 0.25;
    }
    ea = xndev * yndev;
    eb = zndev * zndev;
    ec = ea - eb;
    ed = ea - 6.0 * eb;
    ef = ed + ec + ec;
    s1 = ed * (- c1 + 0.25 * c3 * ed - 1.5 * c4 * zndev * ef);
    s2 = zndev * (c2 * ef + zndev * (- c3 * ec + zndev * c4 * ea));
    result->val = 3.0 * sigma + power4 * (1.0 + s1 + s2) / (mu * sqrt(mu));
    result->err = prec * fabs(result->val);
    return GSL_SUCCESS;
  }
  else {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
}


int
gsl_sf_ellint_RF_impl(double x, double y, double z, gsl_mode_t mode, gsl_sf_result * result)
{
  const double lolim = 5.0 * GSL_DBL_MIN;
  const double uplim = 0.2 * GSL_DBL_MAX;
  const gsl_prec_t goal = GSL_MODE_PREC(mode);
  const double errtol = ( goal == GSL_PREC_DOUBLE ? 0.001 : 0.03 );
  const double prec   = gsl_prec_eps[goal];

  if(x < 0.0 || y < 0.0 || z < 0.0) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(x+y < lolim || x+z < lolim || y+z < lolim) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(locMAX3(x,y,z) < uplim) { 
    const double c1 = 1.0 / 24.0;
    const double c2 = 3.0 / 44.0;
    const double c3 = 1.0 / 14.0;
    double xn = x;
    double yn = y;
    double zn = z;
    double mu, xndev, yndev, zndev, e2, e3, s;
    while(1) {
      double epslon, lamda;
      double xnroot, ynroot, znroot;
      mu = (xn + yn + zn) / 3.0;
      xndev = 2.0 - (mu + xn) / mu;
      yndev = 2.0 - (mu + yn) / mu;
      zndev = 2.0 - (mu + zn) / mu;
      epslon = locMAX3(fabs(xndev), fabs(yndev), fabs(zndev));
      if (epslon < errtol) break;
      xnroot = sqrt(xn);
      ynroot = sqrt(yn);
      znroot = sqrt(zn);
      lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
      xn = (xn + lamda) * 0.25;
      yn = (yn + lamda) * 0.25;
      zn = (zn + lamda) * 0.25;
    }
    e2 = xndev * yndev - zndev * zndev;
    e3 = xndev * yndev * zndev;
    s = 1.0 + (c1 * e2 - 0.1 - c2 * e3) * e2 + c3 * e3;
    result->val = s / sqrt(mu);
    result->err = prec * fabs(result->val);
    return GSL_SUCCESS;
  }
  else {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
}


int
gsl_sf_ellint_RJ_impl(double x, double y, double z, double p, gsl_mode_t mode, gsl_sf_result * result)
{
  const gsl_prec_t goal = GSL_MODE_PREC(mode);
  const double errtol = ( goal == GSL_PREC_DOUBLE ? 0.001 : 0.03 );
  const double prec   = gsl_prec_eps[goal];
  const double lolim =       pow(5.0 * GSL_DBL_MIN, 1.0/3.0);
  const double uplim = 0.3 * pow(0.2 * GSL_DBL_MAX, 1.0/3.0);

  if(x < 0.0 || y < 0.0 || y < 0.0) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(x + y < lolim || x + z < lolim || y + z < lolim || p < lolim) {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
  else if(locMAX4(x,y,z,p) < uplim) {
    const double c1 = 3.0 / 14.0;
    const double c2 = 1.0 /  3.0;
    const double c3 = 3.0 / 22.0;
    const double c4 = 3.0 / 26.0;
    double xn = x;
    double yn = y;
    double zn = z;
    double pn = p;
    double sigma = 0.0;
    double power4 = 1.0;
    double mu, xndev, yndev, zndev, pndev;
    double ea, eb, ec, e2, e3, s1, s2, s3;
    while(1) {
      double xnroot, ynroot, znroot;
      double lamda, alfa, beta;
      double epslon;
      gsl_sf_result rcresult;
      int rcstatus;
      mu = (xn + yn + zn + pn + pn) * 0.2;
      xndev = (mu - xn) / mu;
      yndev = (mu - yn) / mu;
      zndev = (mu - zn) / mu;
      pndev = (mu - pn) / mu;
      epslon = locMAX4(fabs(xndev), fabs(yndev), fabs(zndev), fabs(pndev));
      if(epslon < errtol) break;
      xnroot = sqrt(xn);
      ynroot = sqrt(yn);
      znroot = sqrt(zn);
      lamda = xnroot * (ynroot + znroot) + ynroot * znroot;
      alfa = pn * (xnroot + ynroot + znroot) + xnroot * ynroot * znroot;
      alfa = alfa * alfa;
      beta = pn * (pn + lamda) * (pn + lamda);
      rcstatus = gsl_sf_ellint_RC_impl(alfa, beta, mode, &rcresult);
      if(rcstatus != GSL_SUCCESS) {
        result->val = 0.0;
	result->err = 0.0;
        return rcstatus;
      }
      sigma  += power4 * rcresult.val;
      power4 *= 0.25;
      xn = (xn + lamda) * 0.25;
      yn = (yn + lamda) * 0.25;
      zn = (zn + lamda) * 0.25;
      pn = (pn + lamda) * 0.25;
    }
    
    ea = xndev * (yndev + zndev) + yndev * zndev;
    eb = xndev * yndev * zndev;
    ec = pndev * pndev;
    e2 = ea - 3.0 * ec;
    e3 = eb + 2.0 * pndev * (ea - ec);
    s1 = 1.0 + e2 * (- c1 + 0.75 * c3 * e2 - 1.5 * c4 * e3);
    s2 = eb * (0.5 * c2 + pndev * (- c3 - c3 + pndev * c4));
    s3 = pndev * ea * (c2 - pndev * c3) - c2 * pndev * ec;
    result->val = 3.0 * sigma + power4 * (s1 + s2 + s3) / (mu * sqrt(mu));
    result->err = prec * fabs(result->val);
    return GSL_SUCCESS;
  }
  else {
    result->val = 0.0;
    result->err = 0.0;
    return GSL_EDOM;
  }
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.1)] */
int
gsl_sf_ellint_F_impl(double phi, double k, gsl_mode_t mode, gsl_sf_result * result)
{
  double sin_phi  = sin(phi);
  double sin2_phi = sin_phi*sin_phi;
  double x = 1.0 - sin2_phi;
  double y = 1.0 - k*k*sin2_phi;
  gsl_sf_result rf;
  int status = gsl_sf_ellint_RF_impl(x, y, 1.0, mode, &rf);
  result->val = sin_phi * rf.val;
  result->err = GSL_DBL_EPSILON * fabs(result->val) + fabs(sin_phi*rf.err);
  return status;
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.2)] */
int
gsl_sf_ellint_E_impl(double phi, double k, gsl_mode_t mode, gsl_sf_result * result)
{
  double sin_phi  = sin(phi);
  double sin2_phi = sin_phi  * sin_phi;
  double sin3_phi = sin2_phi * sin_phi;
  double x = 1.0 - sin2_phi;
  double y = 1.0 - k*k*sin2_phi;
  gsl_sf_result rf;
  gsl_sf_result rd;
  int rfstatus = gsl_sf_ellint_RF_impl(x, y, 1.0, mode, &rf);
  int rdstatus = gsl_sf_ellint_RD_impl(x, y, 1.0, mode, &rd);
  result->val  = sin_phi * rf.val - k*k/3.0 * sin3_phi * rd.val;
  result->err  = GSL_DBL_EPSILON * fabs(sin_phi * rf.val);
  result->err += fabs(sin_phi*rf.err);
  result->err += k*k/3.0 * GSL_DBL_EPSILON * fabs(sin3_phi * rd.val);
  result->err += k*k/3.0 * fabs(sin3_phi*rd.err);
  return GSL_ERROR_SELECT_2(rfstatus, rdstatus);
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.3)] */
int
gsl_sf_ellint_P_impl(double phi, double k, double n, gsl_mode_t mode, gsl_sf_result * result)
{
  double sin_phi  = sin(phi);
  double sin2_phi = sin_phi  * sin_phi;
  double sin3_phi = sin2_phi * sin_phi;
  double x = 1.0 - sin2_phi;
  double y = 1.0 - k*k*sin2_phi;
  gsl_sf_result rf;
  gsl_sf_result rj;
  int rfstatus = gsl_sf_ellint_RF_impl(x, y, 1.0, mode, &rf);
  int rjstatus = gsl_sf_ellint_RJ_impl(x, y, 1.0, 1.0 + n*sin2_phi, mode, &rj);
  result->val  = sin_phi * rf.val - n/3.0*sin3_phi * rj.val;
  result->err  = GSL_DBL_EPSILON * fabs(sin_phi * rf.val);
  result->err += n/3.0 * fabs(sin3_phi*rj.err);
  return GSL_ERROR_SELECT_2(rfstatus, rjstatus);
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.4)] */
int
gsl_sf_ellint_D_impl(double phi, double k, double n, gsl_mode_t mode, gsl_sf_result * result)
{
  double sin_phi  = sin(phi);
  double sin2_phi = sin_phi  * sin_phi;
  double sin3_phi = sin2_phi * sin_phi;
  double x = 1.0 - sin2_phi;
  double y = 1.0 - k*k*sin2_phi;
  gsl_sf_result rd;
  int status = gsl_sf_ellint_RD_impl(x, y, 1.0, mode, &rd);
  result->val = sin3_phi/3.0 * rd.val;
  result->err = GSL_DBL_EPSILON * fabs(result->val) + fabs(sin3_phi/3.0 * rd.err);
  return status;
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.5)] */
int
gsl_sf_ellint_Kcomp_impl(double k, gsl_mode_t mode, gsl_sf_result * result)
{
  return gsl_sf_ellint_RF_impl(0.0, 1.0 - k*k, 1.0, mode, result);
}


/* [Carlson, Numer. Math. 33 (1979) 1, (4.6)] */
int
gsl_sf_ellint_Ecomp_impl(double k, gsl_mode_t mode, gsl_sf_result * result)
{
  double y = 1.0 - k*k;
  gsl_sf_result rf;
  gsl_sf_result rd;
  int rfstatus = gsl_sf_ellint_RF_impl(0.0, y, 1.0, mode, &rf);
  int rdstatus = gsl_sf_ellint_RD_impl(0.0, y, 1.0, mode, &rd);
  result->val = rf.val - k*k/3.0 * rd.val;
  result->err = rf.err + k*k/3.0 * rd.err;
  return GSL_ERROR_SELECT_2(rfstatus, rdstatus);
}


/*-*-*-*-*-*-*-*-*-*-*-* Functions w/ Error Handling *-*-*-*-*-*-*-*-*-*-*-*/

int gsl_sf_ellint_Kcomp_e(double k, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_Kcomp_impl(k, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_Kcomp_e", status);
  }
  return status;
}

int gsl_sf_ellint_Ecomp_e(double k, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_Ecomp_impl(k, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_Ecomp_e", status);
  }
  return status;
}

int gsl_sf_ellint_F_e(double phi, double k, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_F_impl(phi, k, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_F_e", status);
  }
  return status;
}

int gsl_sf_ellint_E_e(double phi, double k, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_E_impl(phi, k, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_E_e", status);
  }
  return status;
}

int gsl_sf_ellint_P_e(double phi, double k, double n, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_P_impl(phi, k, n, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_P_e", status);
  }
  return status;
}

int gsl_sf_ellint_D_e(double phi, double k, double n, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_D_impl(phi, k, n, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_D_e", status);
  }
  return status;
}

int gsl_sf_ellint_RC_e(double x, double y, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_RC_impl(x, y, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_RC_e", status);
  }
  return status;
}

int gsl_sf_ellint_RD_e(double x, double y, double z, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_RD_impl(x, y, z, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_RD_e", status);
  }
  return status;
}

int gsl_sf_ellint_RF_e(double x, double y, double z, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_RF_impl(x, y, z, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_RF_e", status);
  }
  return status;
}

int gsl_sf_ellint_RJ_e(double x, double y, double z, double p, gsl_mode_t mode, gsl_sf_result * result)
{
  int status = gsl_sf_ellint_RJ_impl(x, y, z, p, mode, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_ellint_RJ_e", status);
  }
  return status;
}
