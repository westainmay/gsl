/* Author:  G. Jungman
 * RCS:     $Id$
 */
#include <string.h>
#include "odeiv_util.h"


gsl_odeiv_step *
gsl_odeiv_step_new(
  const char * name,
  unsigned int dim,
  unsigned int ord,
  size_t state_size,
  size_t work_size)
{
  gsl_odeiv_step * s;

  if(dim == 0 || name == 0) return 0;

  s = (gsl_odeiv_step *) malloc(sizeof(gsl_odeiv_step));

  if(s != 0) {
    s->_name = (char *) malloc(strlen(name));

    s->dimension = dim;
    s->order = ord;

    s->_step  = 0;
    s->_reset = 0;
    s->_free  = 0;
    s->_state = 0;
    s->_work  = 0;

    if(state_size > 0) s->_state = malloc(state_size);
    if(work_size > 0) s->_work = malloc(work_size);

    if((s->_state == 0 && state_size > 0) || (s->_work == 0 && work_size > 0) || s->_name == 0) {
      if(s->_name != 0) free(s->_name);
      if(s->_state != 0) free(s->_state);
      if(s->_work != 0) free(s->_work);
      free(s);
      return 0;
    }

    strcpy(s->_name, name);
  }

  return s;
}
