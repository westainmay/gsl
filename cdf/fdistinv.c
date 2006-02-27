/* cdf/fdistinv.c
 *
 * Copyright (C) 2002 Przemyslaw Sliwa and Jason H. Stover.
 * Copyright (C) 2006 Brian Gough
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 */

#include <config.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>

#include "error.h"

double 
gsl_cdf_fdist_Pinv (const double p, const double nu1, const double nu2)
{
  double result;
  double y;

  if (p < 0.0)
    {
      CDF_ERROR ("p < 0.0", GSL_EDOM);
    }
  if (p > 1.0 )
    {
      CDF_ERROR ("p > 1.0", GSL_EDOM);
    }
  if (nu1 < 1.0 )
    {
      CDF_ERROR ("nu1 < 1", GSL_EDOM);
    }
  if (nu2 < 1.0)
    {
      CDF_ERROR ("nu2 < 1", GSL_EDOM);
    }

  if (p < 0.5)
    {
      y = gsl_cdf_beta_Pinv(p, nu1 / 2.0, nu2 / 2.0);

      result = nu2 * y / (nu1 * (1.0 - y));
    }
  else 
    {
      y = gsl_cdf_beta_Qinv(p, nu2 / 2.0, nu1 / 2.0);

      result = nu2 * (1-y) / (nu1 * y);
    }
  
  return result;
}

double 
gsl_cdf_fdist_Qinv (const double Q, const double nu1, const double nu2)
{
  double result;
  double y;
  
  if (Q < 0.0)
    {
      CDF_ERROR ("Q < 0.0", GSL_EDOM);
    }
  if (Q > 1.0 )
    {
      CDF_ERROR ("Q > 1.0", GSL_EDOM);
    }
  if (nu1 < 1.0 )
    {
      CDF_ERROR ("nu1 < 1", GSL_EDOM);
    }
  if (nu2 < 1.0)
    {
      CDF_ERROR ("nu2 < 1", GSL_EDOM);
    }

  if (Q > 0.5)
    {
      y = gsl_cdf_beta_Qinv( Q, nu1 / 2.0, nu2 / 2.0 );
      
      result = nu2 * y / (nu1 * (1.0 - y));
    }
  else
    {
      y = gsl_cdf_beta_Pinv(Q, nu2 / 2.0, nu1 / 2.0 );
      
      result = nu2 * (1-y) / (nu1 * y);
    }

  return result;
}

