#include <config.h>
#include <math.h>
#include <stdlib.h>
#include <gsl_rng.h>

/* This is the old BSD rand() generator. The sequence is

   x_{n+1} = (a x_n + c) mod m 

   with a = 1103515245, c = 12345 and m = 2^31 = 2147483648. The seed
   specifies the initial value, x_1.

   The theoretical value of x_{10001} is 1910041713.

   The period of this generator is 2^31.

   The rand() generator is not very good -- the low bits of successive
   numbers are correlated. */

inline unsigned long int rand_get (void *vstate);
double rand_get_double (void *vstate);
void rand_set (void *state, unsigned long int s);

typedef struct
  {
    unsigned long int x;
  }
rand_state_t;

inline unsigned long int
rand_get (void *vstate)
{
  rand_state_t *state = (rand_state_t *) vstate;

  /* The following line relies on unsigned 32-bit arithmetic */

  state->x = (1103515245 * state->x + 12345) & 0x7fffffffUL;

  return state->x;
}

double
rand_get_double (void *vstate)
{
  return rand_get (vstate) / 2147483648.0 ;
}

void
rand_set (void *vstate, unsigned long int s)
{
  rand_state_t *state = (rand_state_t *) vstate;

  state->x = s;

  return;
}

static const gsl_rng_type rand_type =
{"rand",			/* name */
 0x7fffffffUL,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (rand_state_t),
 &rand_set,
 &rand_get,
 &rand_get_double};

const gsl_rng_type *gsl_rng_rand = &rand_type;
