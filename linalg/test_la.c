/* Author:  G. Jungman
 * RCS:     $Id$
 */
#include <gsl/gsl_test.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_ieee_utils.h>
#include "gsl_linalg.h"

static int 
check (double x, double actual, double eps)
{
  if (actual == 0)
    return fabs(x) > eps;
  else
    return (fabs(x - actual)/fabs(actual)) > eps;
}

gsl_matrix *
create_hilbert_matrix(int size)
{
  int i, j;
  gsl_matrix * m = gsl_matrix_alloc(size, size);
  for(i=0; i<size; i++) {
    for(j=0; j<size; j++) {
      gsl_matrix_set(m, i, j, 1.0/(i+j+1.0));
    }
  }
  return m;
}

gsl_matrix *
create_vandermonde_matrix(int size)
{
  int i, j;
  gsl_matrix * m = gsl_matrix_alloc(size, size);
  for(i=0; i<size; i++) {
    for(j=0; j<size; j++) {
      gsl_matrix_set(m, i, j, pow(i + 1.0, size - j - 1));
    }
  }
  return m;
}

gsl_matrix * hilb2;
gsl_matrix * hilb3;
gsl_matrix * hilb4;
gsl_matrix * hilb12;

double hilb2_solution[] = {-8.0, 18.0} ;
double hilb3_solution[] = {27.0, -192.0, 210.0};
double hilb4_solution[] = {-64.0, 900.0, -2520.0, 1820.0};
double hilb12_solution[] = {-1728.0, 245388.0, -8528520.0, 
                            127026900.0, -1009008000.0, 4768571808.0, 
                            -14202796608.0, 27336497760.0, -33921201600.0,
                            26189163000.0, -11437874448.0, 2157916488.0 };


gsl_matrix * vander2;
gsl_matrix * vander3;
gsl_matrix * vander4;
gsl_matrix * vander12;

double vander2_solution[] = {1.0, 0.0}; 
double vander3_solution[] = {0.0, 1.0, 0.0}; 
double vander4_solution[] = {0.0, 0.0, 1.0, 0.0}; 
double vander12_solution[] = {0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 
                            0.0, 0.0, 1.0, 0.0}; 

int
test_matmult(void)
{
  int s = 0;

  gsl_matrix * A = gsl_matrix_calloc(2, 2);
  gsl_matrix * B = gsl_matrix_calloc(2, 3);
  gsl_matrix * C = gsl_matrix_calloc(2, 3);

  gsl_matrix_set(A, 0, 0, 10.0);
  gsl_matrix_set(A, 0, 1,  5.0);
  gsl_matrix_set(A, 1, 0,  1.0);
  gsl_matrix_set(A, 1, 1, 20.0);

  gsl_matrix_set(B, 0, 0, 10.0);
  gsl_matrix_set(B, 0, 1,  5.0);
  gsl_matrix_set(B, 0, 2,  2.0);
  gsl_matrix_set(B, 1, 0,  1.0);
  gsl_matrix_set(B, 1, 1,  3.0);
  gsl_matrix_set(B, 1, 2,  2.0);

  gsl_linalg_matmult(A, B, C);

  s += ( fabs(gsl_matrix_get(C, 0, 0) - 105.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  65.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  30.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) -  30.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  65.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  42.0) > GSL_DBL_EPSILON );

  gsl_matrix_free(A);
  gsl_matrix_free(B);
  gsl_matrix_free(C);

  return s;
}


int
test_matmult_mod(void)
{
  int s = 0;

  gsl_matrix * A = gsl_matrix_calloc(3, 3);
  gsl_matrix * B = gsl_matrix_calloc(3, 3);
  gsl_matrix * C = gsl_matrix_calloc(3, 3);
  gsl_matrix * D = gsl_matrix_calloc(2, 3);
  gsl_matrix * E = gsl_matrix_calloc(2, 3);
  gsl_matrix * F = gsl_matrix_calloc(2, 2);

  gsl_matrix_set(A, 0, 0, 10.0);
  gsl_matrix_set(A, 0, 1,  5.0);
  gsl_matrix_set(A, 0, 2,  1.0);
  gsl_matrix_set(A, 1, 0,  1.0);
  gsl_matrix_set(A, 1, 1, 20.0);
  gsl_matrix_set(A, 1, 2,  5.0);
  gsl_matrix_set(A, 2, 0,  1.0);
  gsl_matrix_set(A, 2, 1,  3.0);
  gsl_matrix_set(A, 2, 2,  7.0);

  gsl_matrix_set(B, 0, 0, 10.0);
  gsl_matrix_set(B, 0, 1,  5.0);
  gsl_matrix_set(B, 0, 2,  2.0);
  gsl_matrix_set(B, 1, 0,  1.0);
  gsl_matrix_set(B, 1, 1,  3.0);
  gsl_matrix_set(B, 1, 2,  2.0);
  gsl_matrix_set(B, 2, 0,  1.0);
  gsl_matrix_set(B, 2, 1,  3.0);
  gsl_matrix_set(B, 2, 2,  2.0);

  gsl_matrix_set(D, 0, 0, 10.0);
  gsl_matrix_set(D, 0, 1,  5.0);
  gsl_matrix_set(D, 0, 2,  1.0);
  gsl_matrix_set(D, 1, 0,  1.0);
  gsl_matrix_set(D, 1, 1, 20.0);
  gsl_matrix_set(D, 1, 2,  5.0);

  gsl_matrix_set(E, 0, 0, 10.0);
  gsl_matrix_set(E, 0, 1,  5.0);
  gsl_matrix_set(E, 0, 2,  2.0);
  gsl_matrix_set(E, 1, 0,  1.0);
  gsl_matrix_set(E, 1, 1,  3.0);
  gsl_matrix_set(E, 1, 2,  2.0);

  gsl_linalg_matmult_mod(A, GSL_LINALG_MOD_NONE, B, GSL_LINALG_MOD_NONE, C);
  s += ( fabs(gsl_matrix_get(C, 0, 0) - 106.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  68.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  32.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) -  35.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  80.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  52.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 0) -  20.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 1) -  35.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 2) -  22.0) > GSL_DBL_EPSILON );

  gsl_linalg_matmult_mod(A, GSL_LINALG_MOD_TRANSPOSE, B, GSL_LINALG_MOD_NONE, C);
  s += ( fabs(gsl_matrix_get(C, 0, 0) - 102.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  56.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  24.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) -  73.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  94.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  56.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 0) -  22.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 1) -  41.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 2) -  26.0) > GSL_DBL_EPSILON );

  gsl_linalg_matmult_mod(A, GSL_LINALG_MOD_NONE, B, GSL_LINALG_MOD_TRANSPOSE, C);
  s += ( fabs(gsl_matrix_get(C, 0, 0) - 127.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  27.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  27.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) - 120.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  71.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  71.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 0) -  39.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 1) -  24.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 2) -  24.0) > GSL_DBL_EPSILON );

  gsl_linalg_matmult_mod(A, GSL_LINALG_MOD_TRANSPOSE, B, GSL_LINALG_MOD_TRANSPOSE, C);
  s += ( fabs(gsl_matrix_get(C, 0, 0) - 107.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  15.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  15.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) - 156.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  71.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  71.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 0) -  49.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 1) -  30.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 2) -  30.0) > GSL_DBL_EPSILON );

  /* now try for non-symmetric matrices */
  gsl_linalg_matmult_mod(D, GSL_LINALG_MOD_TRANSPOSE, E, GSL_LINALG_MOD_NONE, C);
  s += ( fabs(gsl_matrix_get(C, 0, 0) - 101.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 1) -  53.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 0, 2) -  22.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 0) -  70.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 1) -  85.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 1, 2) -  50.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 0) -  15.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 1) -  20.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(C, 2, 2) -  12.0) > GSL_DBL_EPSILON );


  gsl_linalg_matmult_mod(D, GSL_LINALG_MOD_NONE, E, GSL_LINALG_MOD_TRANSPOSE, F);
  s += ( fabs(gsl_matrix_get(F, 0, 0) - 127.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(F, 0, 1) -  27.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(F, 1, 0) - 120.0) > GSL_DBL_EPSILON );
  s += ( fabs(gsl_matrix_get(F, 1, 1) -  71.0) > GSL_DBL_EPSILON );


  gsl_matrix_free(A);
  gsl_matrix_free(B);
  gsl_matrix_free(C);
  gsl_matrix_free(D);
  gsl_matrix_free(E);
  gsl_matrix_free(F);

  return s;
}


static int
test_LU_solve_dim(const gsl_matrix * m, const double * actual, double eps)
{
  int s = 0;
  int signum;
  size_t i, dim = m->size1;

  gsl_permutation * perm = gsl_permutation_alloc(dim);
  gsl_vector * rhs = gsl_vector_alloc(dim);
  gsl_matrix * lu  = gsl_matrix_alloc(dim,dim);
  gsl_vector * x = gsl_vector_alloc(dim);
  gsl_vector * residual = gsl_vector_alloc(dim);
  gsl_matrix_memcpy(lu,m);
  for(i=0; i<dim; i++) gsl_vector_set(rhs, i, i+1.0);
  s += gsl_linalg_LU_decomp(lu, perm, &signum);
  s += gsl_linalg_LU_solve(lu, perm, rhs, x);

  for(i=0; i<dim; i++) {
    int foo = check(gsl_vector_get(x, i),actual[i],eps);
    if(foo) {
      printf("%3d[%d]: %22.18g   %22.18g\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }

  s += gsl_linalg_LU_refine(m, lu, perm, rhs, x, residual);

  for(i=0; i<dim; i++) {
    int foo = check(gsl_vector_get(x, i),actual[i],eps);
    if(foo) {
      printf("%3d[%d]: %22.18g   %22.18g (improved)\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }

  gsl_vector_free(residual);
  gsl_vector_free(x);
  gsl_matrix_free(lu);
  gsl_vector_free(rhs);
  gsl_permutation_free(perm);

  return s;
}


int test_LU_solve(void)
{
  int f;
  int s = 0;

  f = test_LU_solve_dim(hilb2, hilb2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve hilbert(2)");
  s += f;

  f = test_LU_solve_dim(hilb3, hilb3_solution, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve hilbert(3)");
  s += f;

  f = test_LU_solve_dim(hilb4, hilb4_solution, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve hilbert(4)");
  s += f;

  f = test_LU_solve_dim(hilb12, hilb12_solution, 0.5);
  gsl_test(f, "  LU_solve hilbert(12)");
  s += f;

  f = test_LU_solve_dim(vander2, vander2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve vander(2)");
  s += f;

  f = test_LU_solve_dim(vander3, vander3_solution, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve vander(3)");
  s += f;

  f = test_LU_solve_dim(vander4, vander4_solution, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  LU_solve vander(4)");
  s += f;

  f = test_LU_solve_dim(vander12, vander12_solution, 0.05);
  gsl_test(f, "  LU_solve vander(12)");
  s += f;

  return s;
}

static int
test_QR_solve_dim(const gsl_matrix * m, const double * actual, double eps)
{
  int s = 0;
  size_t i, dim = m->size1;

  gsl_vector * rhs = gsl_vector_alloc(dim);
  gsl_matrix * qr  = gsl_matrix_alloc(dim,dim);
  gsl_vector * d = gsl_vector_alloc(dim);
  gsl_vector * x = gsl_vector_alloc(dim);
  gsl_matrix_memcpy(qr,m);
  for(i=0; i<dim; i++) gsl_vector_set(rhs, i, i+1.0);
  s += gsl_linalg_QR_decomp(qr, d);
  s += gsl_linalg_QR_solve(qr, d, rhs, x);
  for(i=0; i<dim; i++) {
    int foo = check(gsl_vector_get(x, i), actual[i], eps);
    if(foo) {
      printf("%3d[%d]: %22.18g   %22.18g\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }
  gsl_vector_free(x);
  gsl_vector_free(d);
  gsl_matrix_free(qr);
  gsl_vector_free(rhs);

  return s;
}

int test_QR_solve(void)
{
  int f;
  int s = 0;

  f = test_QR_solve_dim(hilb2, hilb2_solution, 2 * 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve hilbert(2)");
  s += f;

  f = test_QR_solve_dim(hilb3, hilb3_solution, 2 * 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve hilbert(3)");
  s += f;

  f = test_QR_solve_dim(hilb4, hilb4_solution, 2 * 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve hilbert(4)");
  s += f;

  f = test_QR_solve_dim(hilb12, hilb12_solution, 0.5);
  gsl_test(f, "  QR_solve hilbert(12)");
  s += f;

  f = test_QR_solve_dim(vander2, vander2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve vander(2)");
  s += f;

  f = test_QR_solve_dim(vander3, vander3_solution, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve vander(3)");
  s += f;

  f = test_QR_solve_dim(vander4, vander4_solution, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_solve vander(4)");
  s += f;

  f = test_QR_solve_dim(vander12, vander12_solution, 0.05);
  gsl_test(f, "  QR_solve vander(12)");
  s += f;

  return s;
}


static int
test_QRPT_solve_dim(const gsl_matrix * m, const double * actual, double eps)
{
  int s = 0;
  int signum;
  size_t i, dim = m->size1;

  gsl_permutation * perm = gsl_permutation_alloc(dim);
  gsl_vector * rhs = gsl_vector_alloc(dim);
  gsl_matrix * qr  = gsl_matrix_alloc(dim,dim);
  gsl_vector * d = gsl_vector_alloc(dim);
  gsl_vector * x = gsl_vector_alloc(dim);
  gsl_matrix_memcpy(qr,m);
  for(i=0; i<dim; i++) gsl_vector_set(rhs, i, i+1.0);
  s += gsl_linalg_QRPT_decomp(qr, d, perm, &signum);
  s += gsl_linalg_QRPT_solve(qr, d, perm, rhs, x);
  for(i=0; i<dim; i++) {
    int foo = check(gsl_vector_get(x, i), actual[i], eps);
    if(foo) {
      printf("%3d[%d]: %22.18g   %22.18g\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }
  gsl_vector_free(x);
  gsl_vector_free(d);
  gsl_matrix_free(qr);
  gsl_vector_free(rhs);
  gsl_permutation_free(perm);

  return s;
}

int test_QRPT_solve(void)
{
  int f;
  int s = 0;

  f = test_QRPT_solve_dim(hilb2, hilb2_solution, 2 * 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve hilbert(2)");
  s += f;

  f = test_QRPT_solve_dim(hilb3, hilb3_solution, 2 * 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve hilbert(3)");
  s += f;

  f = test_QRPT_solve_dim(hilb4, hilb4_solution, 2 * 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve hilbert(4)");
  s += f;

  f = test_QRPT_solve_dim(hilb12, hilb12_solution, 0.5);
  gsl_test(f, "  QRPT_solve hilbert(12)");
  s += f;

  f = test_QRPT_solve_dim(vander2, vander2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve vander(2)");
  s += f;

  f = test_QRPT_solve_dim(vander3, vander3_solution, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve vander(3)");
  s += f;

  f = test_QRPT_solve_dim(vander4, vander4_solution, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QRPT_solve vander(4)");
  s += f;

  f = test_QRPT_solve_dim(vander12, vander12_solution, 0.05);
  gsl_test(f, "  QRPT_solve vander(12)");
  s += f;

  return s;
}


static int
test_QR_update_dim(const gsl_matrix * m, double eps)
{
  int s = 0;
  size_t i,j,k, dim = m->size1;

  gsl_vector * rhs = gsl_vector_alloc(dim);
  gsl_matrix * qr1  = gsl_matrix_alloc(dim,dim);
  gsl_matrix * qr2  = gsl_matrix_alloc(dim,dim);
  gsl_matrix * q1  = gsl_matrix_alloc(dim,dim);
  gsl_matrix * r1  = gsl_matrix_alloc(dim,dim);
  gsl_matrix * q2  = gsl_matrix_alloc(dim,dim);
  gsl_matrix * r2  = gsl_matrix_alloc(dim,dim);
  gsl_vector * d = gsl_vector_alloc(dim);
  gsl_vector * solution1 = gsl_vector_alloc(dim);
  gsl_vector * solution2 = gsl_vector_alloc(dim);
  gsl_vector * u = gsl_vector_alloc(dim);
  gsl_vector * v = gsl_vector_alloc(dim);
  gsl_vector * w = gsl_vector_alloc(dim);
  gsl_matrix_memcpy(qr1,m);
  gsl_matrix_memcpy(qr2,m);
  for(i=0; i<dim; i++) gsl_vector_set(rhs, i, i+1.0);
  for(i=0; i<dim; i++) gsl_vector_set(u, i, sin(i+1.0));
  for(i=0; i<dim; i++) gsl_vector_set(v, i, cos(i+2.0) + sin(i*i+3.0));

  for(i=0; i<dim; i++) 
    {
      double ui = gsl_vector_get(u, i);
      for(j=0; j<dim; j++) 
        {
          double vj = gsl_vector_get(v, j);
          double qij = gsl_matrix_get(qr1, i, j);
          gsl_matrix_set(qr1, i, j, qij + ui * vj);
        }
    }

  s += gsl_linalg_QR_decomp(qr2, d);
  s += gsl_linalg_QR_unpack(qr2, d, q2, r2);

  /* compute w = Q^T u */
      
  for (j = 0; j < dim; j++)
    {
      double sum = 0;
      for (i = 0; i < dim; i++)
          sum += gsl_matrix_get (q2, i, j) * gsl_vector_get (u, i);
      gsl_vector_set (w, j, sum);
    }

  s += gsl_linalg_QR_update(q2, r2, w, v);

  /* compute qr2 = q2 * r2 */

  for (i = 0; i < dim; i++)
    {
      for (j = 0; j< dim; j++)
        {
          double sum = 0;
          for (k = 0; k <= j; k++)
            {
              double qik = gsl_matrix_get(q2, i, k);
              double rkj = gsl_matrix_get(r2, k, j);
              sum += qik * rkj ;
            }
          gsl_matrix_set (qr2, i, j, sum);
        }
    }

  for(i=0; i<dim; i++) {
    for(j=0; j<dim; j++) {
      double s1 = gsl_matrix_get(qr1, i, j);
      double s2 = gsl_matrix_get(qr2, i, j);
      
      int foo = check(s1, s2, eps);
      if(foo) {
        printf("%3d[%d,%d]: %22.18g   %22.18g\n", dim, i,j, s1, s2);
      }
      s += foo;
    }
  }

  gsl_vector_free(solution1);
  gsl_vector_free(solution2);
  gsl_vector_free(d);
  gsl_vector_free(u);
  gsl_vector_free(v);
  gsl_vector_free(w);
  gsl_matrix_free(qr1);
  gsl_matrix_free(qr2);
  gsl_matrix_free(q1);
  gsl_matrix_free(r1);
  gsl_matrix_free(q2);
  gsl_matrix_free(r2);
  gsl_vector_free(rhs);

  return s;
}

int test_QR_update(void)
{
  int f;
  int s = 0;

  f = test_QR_update_dim(hilb2,  2 * 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update hilbert(2)");
  s += f;

  f = test_QR_update_dim(hilb3,  2 * 128.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update hilbert(3)");
  s += f;

  f = test_QR_update_dim(hilb4, 2 * 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update hilbert(4)");
  s += f;

  f = test_QR_update_dim(hilb12, 0.5);
  gsl_test(f, "  QR_update hilbert(12)");
  s += f;

  f = test_QR_update_dim(vander2, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update vander(2)");
  s += f;

  f = test_QR_update_dim(vander3, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update vander(3)");
  s += f;

  f = test_QR_update_dim(vander4, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  QR_update vander(4)");
  s += f;

  f = test_QR_update_dim(vander12, 0.05);
  gsl_test(f, "  QR_update vander(12)");
  s += f;

  return s;
}


static int
test_HH_solve_dim(const gsl_matrix * m, const double * actual, double eps)
{
  int s = 0;
  size_t i, dim = m->size1;

  gsl_permutation * perm = gsl_permutation_alloc(dim);
  gsl_matrix * hh  = gsl_matrix_alloc(dim,dim);
  gsl_vector * d = gsl_vector_alloc(dim);
  gsl_vector * x = gsl_vector_alloc(dim);
  gsl_matrix_memcpy(hh,m);
  for(i=0; i<dim; i++) gsl_vector_set(x, i, i+1.0);
  s += gsl_linalg_HH_solve(hh, x);
  for(i=0; i<dim; i++) {
    int foo = check(gsl_vector_get(x, i),actual[i],eps);
    if( foo) {
      printf("%3d[%d]: %22.18g   %22.18g\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }
  gsl_vector_free(x);
  gsl_vector_free(d);
  gsl_matrix_free(hh);
  gsl_permutation_free(perm);

  return s;
}

int test_HH_solve(void)
{
  int f;
  int s = 0;

  f = test_HH_solve_dim(hilb2, hilb2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve hilbert(2)");
  s += f;

  f = test_HH_solve_dim(hilb3, hilb3_solution, 128.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve hilbert(3)");
  s += f;

  f = test_HH_solve_dim(hilb4, hilb4_solution, 2.0 * 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve hilbert(4)");
  s += f;

  f = test_HH_solve_dim(hilb12, hilb12_solution, 0.5);
  gsl_test(f, "  HH_solve hilbert(12)");
  s += f;

  f = test_HH_solve_dim(vander2, vander2_solution, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve vander(2)");
  s += f;

  f = test_HH_solve_dim(vander3, vander3_solution, 64.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve vander(3)");
  s += f;

  f = test_HH_solve_dim(vander4, vander4_solution, 1024.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  HH_solve vander(4)");
  s += f;

  f = test_HH_solve_dim(vander12, vander12_solution, 0.05);
  gsl_test(f, "  HH_solve vander(12)");
  s += f;

  return s;
}


static int
test_TD_solve_dim(size_t dim, double d, double od, const double * actual, double eps)
{
  int s = 0;
  size_t i;

  gsl_vector * offdiag = gsl_vector_alloc(dim-1);
  gsl_vector * diag = gsl_vector_alloc(dim);
  gsl_vector * rhs = gsl_vector_alloc(dim);
  gsl_vector * x = gsl_vector_alloc(dim);

  for(i=0; i<dim; i++) {
    gsl_vector_set(diag, i, d);
    gsl_vector_set(rhs,  i, i + 1.0);
  }
  for(i=0; i<dim-1; i++) {
    gsl_vector_set(offdiag, i, od);
  }

  s += gsl_linalg_solve_symm_tridiag(diag, offdiag, rhs, x);

  for(i=0; i<dim; i++) {
    double si = gsl_vector_get(x, i);
    double ai = actual[i];
    int foo = check(si, ai, eps);
    if(foo) {
      printf("%3d[%d]: %22.18g   %22.18g\n", dim, i, gsl_vector_get(x, i), actual[i]);
    }
    s += foo;
  }

  gsl_vector_free(x);
  gsl_vector_free(rhs);
  gsl_vector_free(diag);
  gsl_vector_free(offdiag);

  return s;
}


int test_TD_solve(void)
{
  int f;
  int s = 0;
  double actual[16];

  actual[0] =  0.0;
  actual[1] =  2.0;
  f = test_TD_solve_dim(2, 1.0, 0.5, actual, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  solve_TD dim=2 A");
  s += f;

  actual[0] =  3.0/8.0;
  actual[1] =  15.0/8.0;
  f = test_TD_solve_dim(2, 1.0, 1.0/3.0, actual, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  solve_TD dim=2 B");
  s += f;

  actual[0] =  5.0/8.0;
  actual[1] =  9.0/8.0;
  actual[2] =  2.0;
  actual[3] =  15.0/8.0;
  actual[4] =  35.0/8.0;
  f = test_TD_solve_dim(5, 1.0, 1.0/3.0, actual, 8.0 * GSL_DBL_EPSILON);
  gsl_test(f, "  solve_TD dim=5");
  s += f;

  return s;
}





int main()
{
  gsl_ieee_env_setup ();

  hilb2 = create_hilbert_matrix(2);
  hilb3 = create_hilbert_matrix(3);
  hilb4 = create_hilbert_matrix(4);
  hilb12 = create_hilbert_matrix(12);

  vander2 = create_vandermonde_matrix(2);
  vander3 = create_vandermonde_matrix(3);
  vander4 = create_vandermonde_matrix(4);
  vander12 = create_vandermonde_matrix(12);

  gsl_test(test_matmult(),        "Matrix Multiply");
  gsl_test(test_matmult_mod(),    "Matrix Multiply with Modification");
  gsl_test(test_LU_solve(),       "LU Decomposition and Solve");
  gsl_test(test_QR_solve(),       "QR Decomposition and Solve");
  gsl_test(test_QR_update(),      "QR Rank-1 Update");
  gsl_test(test_QRPT_solve(),     "QRPT Decomposition and Solve");
  gsl_test(test_HH_solve(),       "Householder solve");
  gsl_test(test_TD_solve(),       "Tridiagonal solve");

  gsl_matrix_free(hilb2);
  gsl_matrix_free(hilb3);
  gsl_matrix_free(hilb4);
  gsl_matrix_free(hilb12);

  gsl_matrix_free(vander2);
  gsl_matrix_free(vander3);
  gsl_matrix_free(vander4);
  gsl_matrix_free(vander12);

  return gsl_test_summary();
}
