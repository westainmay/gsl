/* Author:  G. Jungman
 * RCS:     $Id$
 */
#include <config.h>
#include <stdlib.h>
#include <gsl_errno.h>
#include "gsl_odeiv.h"


const char *
gsl_odeiv_step_name(const gsl_odeiv_step * s)
{
  return s->_name;
}


int
gsl_odeiv_step_impl(
  gsl_odeiv_step * step,
  double t,
  double h,
  double y[],
  double yerr[],
  const double dydt_in[],
  double dydt_out[],
  const gsl_odeiv_system * dydt)
{
  return step->_step(step, t, h, y, yerr, dydt_in, dydt_out, dydt);
}


int
gsl_odeiv_step_reset(gsl_odeiv_step * s)
{
  if(s->_reset != 0) {
    return s->_reset(s->_state);
  }
  else {
    return GSL_SUCCESS;
  }
}


void
gsl_odeiv_step_free(gsl_odeiv_step * s)
{
  if(s != 0) {
    free(s->_name);
    if(s->_free != 0)  s->_free(s->_state, s->_work);
    if(s->_state != 0) free(s->_state);
    if(s->_work != 0)  free(s->_work);
    free(s);
  }
}
