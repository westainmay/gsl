#include <math.h>
#include <gsl_rng.h>
#include <gsl_randist.h>
#include <gsl_sf.h>

/* The hypergeometric distribution has the form,

   prob(k) =  choose(n1,t) choose(n2, t-k) / choose(n1+n2,t)

   where choose(a,b) = a!/(b!(a-b)!) 

   n1 + n2 is the total population (tagged plus untagged)
   n1 is the tagged population
   t is the number of samples taken (without replacement)
   k is the number of tagged samples found

*/

unsigned int
gsl_ran_hypergeometric (const gsl_rng * r, unsigned int n1, unsigned int n2, 
			unsigned int t)
{
  unsigned int i = 0;
  unsigned int f = n1;
  unsigned int n = n1 + n2;
  unsigned int k = 0;

  if (t > n)
    {
      t = n ;
    }

  if (t < n / 2) 
    {
      for (i = 0 ; i < t ; i++)
	{
	  double u = gsl_rng_uniform(r) ;
	  
	  if (n * u < f)
	    {
	      k++ ;
	      if (k == n1)
		return k ;
	      f-- ;
	    }
	  n-- ;
	}
      return k;
    }
  else
    {
      for (i = 0 ; i < n - t ; i++)
	{
	  double u = gsl_rng_uniform(r) ;
	  
	  if (n * u < f)
	    {
	      k++ ;
	      if (k == n1)
		return n1 - k ;
	      f-- ;
	    }
	  n-- ;
	}
      return n1 - k;
    }


}

double
gsl_ran_hypergeometric_pdf (const unsigned int k, 
			    const unsigned int n1, 
			    const unsigned int n2, 
			    unsigned int t)
{
  if (t > n1 + n2)
    {
      t = n1 + n2 ;
    }

  if (k > n1 || k > t)
    {
      return 0 ;
    }
  else if (t > n2 && k + n2 < t )
    {
      return 0 ;
    }
  else 
    {
      double f = (gsl_sf_lnchoose (n1, k)  + gsl_sf_lnchoose (n2, t - k)
		  - gsl_sf_lnchoose (n1 + n2, t)) ;
      
      return exp(f);
    }
}
