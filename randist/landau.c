/* randist/landau.c
 *
 * Copyright (C) 2001, 2004 David Morrison
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Adapted from the CERN library routines DENLAN, RANLAN, and DISLAN
 * as described in http://consult.cern.ch/shortwrups/g110/top.html.
 * Original author: K.S. K\"olbig.
 *
 * The distribution is given by the complex path integral,
 *
 *  p(x) = (1/(2 pi i)) \int_{c-i\inf}^{c+i\inf} ds exp(s log(s) + x s) 
 *
 * which can be converted into a real integral over [0,+\inf]
 *
 *  p(x) = (1/pi) \int_0^\inf dt \exp(-t log(t) - x t) sin(pi t)
 *
 */

#include <config.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

double
gsl_ran_landau_pdf(const double x)
{
  static double P1[5] =
    {
      0.4259894875E0, -0.1249762550E0, 0.3984243700E-1,
      -0.6298287635E-2, 0.1511162253E-2
    };
  static double P2[5] =
    {
      0.1788541609E0, 0.1173957403E0, 0.1488850518E-1,
      -0.1394989411E-2, 0.1283617211E-3
    };
  static double P3[5] =
    {
      0.1788544503E0, 0.9359161662E-1, 0.6325387654E-2,
      0.6611667319E-4, -0.2031049101E-5
    };
  static double P4[5] =
    {
      0.9874054407E0, 0.1186723273E3, 0.8492794360E3,
      -0.7437792444E3, 0.4270262186E3
    };
  static double P5[5] =
    {
      0.1003675074E1, 0.1675702434E3, 0.4789711289E4,
      0.2121786767E5, -0.2232494910E5
    };
  static double P6[5] =
    {
      0.1000827619E1, 0.6649143136E3, 0.6297292665E5,
      0.4755546998E6, -0.5743609109E7
    };

  static double Q1[5] =
    {
      1.0, -0.3388260629E0, 0.9594393323E-1,
      -0.1608042283E-1, 0.3778942063E-2
    };
  static double Q2[5] =
    {
      1.0, 0.7428795082E0, 0.3153932961E0,
      0.6694219548E-1, 0.8790609714E-2
    };
  static double Q3[5] =
    {
      1.0, 0.6097809921E0, 0.2560616665E0,
      0.4746722384E-1, 0.6957301675E-2
    };
  static double Q4[5] =
    {
      1.0, 0.1068615961E3, 0.3376496214E3,
      0.2016712389E4, 0.1597063511E4
    };
  static double Q5[5] =
    {
      1.0, 0.1569424537E3, 0.3745310488E4,
      0.9834698876E4, 0.6692428357E5
    };
  static double Q6[5] =
    {
      1.0, 0.6514101098E3, 0.5697473333E5,
      0.1659174725E6, -0.2815759939E7
    };

  static double A1[3] =
    {
      0.4166666667E-1, -0.1996527778E-1, 0.2709538966E-1
    };
  static double A2[2] =
    {
      -0.1845568670E1, -0.4284640743E1
    };

  double U, V, DENLAN;

  V = x;
  if (V < -5.5)
    {
      U = exp(V + 1.0);
      DENLAN = 0.3989422803 * (exp( -1 / U) / sqrt(U)) *
        (1 + (A1[0] + (A1[1] + A1[2] * U) * U) * U);
    }
  else if (V < -1)
    {
      U = exp( -V - 1);
      DENLAN = exp( -U) * sqrt(U) *
        (P1[0] + (P1[1] + (P1[2] + (P1[3] + P1[4] * V) * V) * V) * V) /
        (Q1[0] + (Q1[1] + (Q1[2] + (Q1[3] + Q1[4] * V) * V) * V) * V);
    }
  else if (V < 1)
    {
      DENLAN = (P2[0] + (P2[1] + (P2[2] + (P2[3] + P2[4] * V) * V) * V) * V) /
        (Q2[0] + (Q2[1] + (Q2[2] + (Q2[3] + Q2[4] * V) * V) * V) * V);
    }
  else if (V < 5)
    {
      DENLAN = (P3[0] + (P3[1] + (P3[2] + (P3[3] + P3[4] * V) * V) * V) * V) /
        (Q3[0] + (Q3[1] + (Q3[2] + (Q3[3] + Q3[4] * V) * V) * V) * V);
    }
  else if (V < 12)
    {
      U = 1 / V;
      DENLAN = U * U *
        (P4[0] + (P4[1] + (P4[2] + (P4[3] + P4[4] * U) * U) * U) * U) /
        (Q4[0] + (Q4[1] + (Q4[2] + (Q4[3] + Q4[4] * U) * U) * U) * U);
    }
  else if (V < 50)
    {
      U = 1 / V;
      DENLAN = U * U *
        (P5[0] + (P5[1] + (P5[2] + (P5[3] + P5[4] * U) * U) * U) * U) /
        (Q5[0] + (Q5[1] + (Q5[2] + (Q5[3] + Q5[4] * U) * U) * U) * U);
    }
  else if (V < 300)
    {
      U = 1 / V;
      DENLAN = U * U *
        (P6[0] + (P6[1] + (P6[2] + (P6[3] + P6[4] * U) * U) * U) * U) /
        (Q6[0] + (Q6[1] + (Q6[2] + (Q6[3] + Q6[4] * U) * U) * U) * U);
    }
  else
    {
      U = 1 / (V - V * log(V) / (V + 1));
      DENLAN = U * U * (1 + (A2[0] + A2[1] * U) * U);
    }

  return DENLAN;
}

#if 0 /* Not needed yet */
/* This function is a translation from the original Fortran of the
 * CERN library routine DISLAN, the integral from -inf to x of the
 * Landau p.d.f.
 */
static
double
gsl_ran_landau_dislan(const double x)
{
  static double P1[5] =
    {
      0.2514091491E0, -0.6250580444E-1,
      0.1458381230E-1, -0.2108817737E-2,
      0.7411247290E-3
    };

  static double P2[4] =
    {
      0.2868328584E0, 0.3564363231E0,
      0.1523518695E0, 0.2251304883E-1
    };

  static double P3[4] =
    {
      0.2868329066E0, 0.3003828436E0,
      0.9950951941E-1, 0.8733827185E-2
    };

  static double P4[4] =
    {
      0.1000351630E1, 0.4503592498E1,
      0.1085883880E2, 0.7536052269E1
    };

  static double P5[4] =
    {
      0.1000006517E1, 0.4909414111E2,
      0.8505544753E2, 0.1532153455E3
    };

  static double P6[4] =
    {
      0.1000000983E1, 0.1329868456E3,
      0.9162149244E3, -0.9605054274E3
    };

  static double Q1[5] =
    {
      1.0, -0.5571175625E-2,
      0.6225310236E-1, -0.3137378427E-2,
      0.1931496439E-2
    };

  static double Q2[4] =
    {
      1.0, 0.6191136137E0,
      0.1720721448E0, 0.2278594771E-1
    };

  static double Q3[4] =
    {
      1.0, 0.4237190502E0,
      0.1095631512E0, 0.8693851567E-2
    };

  static double Q4[4] =
    {
      1.0, 0.5539969678E1,
      0.1933581111E2, 0.2721321508E2
    };

  static double Q5[4] =
    {
      1.0, 0.5009928881E2,
      0.1399819104E3, 0.4200002909E3
    };

  static double Q6[4] =
    {
      1.0, 0.1339887843E3,
      0.1055990413E4, 0.5532224619E3
    };

  static double A1[3] =
    {
      -0.4583333333E0, 0.6675347222E0, -0.1641741416E1
    };

  static double A2[3] =
    {
      1.0, -0.4227843351E0, -0.2043403138E1
    };

  double U, V, DISLAN;

  V = x;
  if (V < -5.5)
    {
      U = exp(V + 1);
      DISLAN = 0.3989422803 * exp( -1 / U) * sqrt(U) *
               (1 + (A1[0] + (A1[1] + A1[2] * U) * U) * U);
    }
  else if (V < -1)
    {
      U = exp( -V - 1);
      DISLAN = (exp( -U) / sqrt(U)) *
               (P1[0] + (P1[1] + (P1[2] + (P1[3] + P1[4] * V) * V) * V) * V) /
               (Q1[0] + (Q1[1] + (Q1[2] + (Q1[3] + Q1[4] * V) * V) * V) * V);
    }
  else if (V < 1)
    {
      DISLAN = (P2[0] + (P2[1] + (P2[2] + P2[3] * V) * V) * V) /
               (Q2[0] + (Q2[1] + (Q2[2] + Q2[3] * V) * V) * V);
    }
  else if (V < 4)
    {
      DISLAN = (P3[0] + (P3[1] + (P3[2] + P3[3] * V) * V) * V) /
               (Q3[0] + (Q3[1] + (Q3[2] + Q3[3] * V) * V) * V);
    }
  else if (V < 12)
    {
      U = 1 / V;
      DISLAN = (P4[0] + (P4[1] + (P4[2] + P4[3] * U) * U) * U) /
               (Q4[0] + (Q4[1] + (Q4[2] + Q4[3] * U) * U) * U);
    }
  else if (V < 50)
    {
      U = 1 / V;
      DISLAN = (P5[0] + (P5[1] + (P5[2] + P5[3] * U) * U) * U) /
               (Q5[0] + (Q5[1] + (Q5[2] + Q5[3] * U) * U) * U);
    }
  else if (V < 300)
    {
      U = 1 / V;
      DISLAN = (P6[0] + (P6[1] + (P6[2] + P6[3] * U) * U) * U) /
               (Q6[0] + (Q6[1] + (Q6[2] + Q6[3] * U) * U) * U);
    }
  else
    {
      U = 1 / (V - V * log(V) / (V + 1));
      DISLAN = 1 - (A2[0] + (A2[1] + A2[2] * U) * U) * U;
    }

  return DISLAN;
}
#endif

double
gsl_ran_landau(const gsl_rng * r)
{
  static double F[983] =
    {
      0.0000000,   /* Add empty element [0] to account for difference 
                      between C and Fortran convention for lower bound. */
      00.000000, 00.000000, 00.000000, 00.000000, 00.000000,
      -2.244733, -2.204365, -2.168163, -2.135219, -2.104898,
      -2.076740, -2.050397, -2.025605, -2.002150, -1.979866,
      -1.958612, -1.938275, -1.918760, -1.899984, -1.881879,
      -1.864385, -1.847451, -1.831030, -1.815083, -1.799574,
      -1.784473, -1.769751, -1.755383, -1.741346, -1.727620,
      -1.714187, -1.701029, -1.688130, -1.675477, -1.663057,
      -1.650858, -1.638868, -1.627078, -1.615477, -1.604058,
      -1.592811, -1.581729, -1.570806, -1.560034, -1.549407,
      -1.538919, -1.528565, -1.518339, -1.508237, -1.498254,
      -1.488386, -1.478628, -1.468976, -1.459428, -1.449979,
      -1.440626, -1.431365, -1.422195, -1.413111, -1.404112,
      -1.395194, -1.386356, -1.377594, -1.368906, -1.360291,
      -1.351746, -1.343269, -1.334859, -1.326512, -1.318229,
      -1.310006, -1.301843, -1.293737, -1.285688, -1.277693,
      -1.269752, -1.261863, -1.254024, -1.246235, -1.238494,
      -1.230800, -1.223153, -1.215550, -1.207990, -1.200474,
      -1.192999, -1.185566, -1.178172, -1.170817, -1.163500,
      -1.156220, -1.148977, -1.141770, -1.134598, -1.127459,
      -1.120354, -1.113282, -1.106242, -1.099233, -1.092255,
      -1.085306, -1.078388, -1.071498, -1.064636, -1.057802,
      -1.050996, -1.044215, -1.037461, -1.030733, -1.024029,
      -1.017350, -1.010695, -1.004064, -0.997456, -0.990871,
      -0.984308, -0.977767, -0.971247, -0.964749, -0.958271,
      -0.951813, -0.945375, -0.938957, -0.932558, -0.926178,
      -0.919816, -0.913472, -0.907146, -0.900838, -0.894547,
      -0.888272, -0.882014, -0.875773, -0.869547, -0.863337,
      -0.857142, -0.850963, -0.844798, -0.838648, -0.832512,
      -0.826390, -0.820282, -0.814187, -0.808106, -0.802038,
      -0.795982, -0.789940, -0.783909, -0.777891, -0.771884,
      -0.765889, -0.759906, -0.753934, -0.747973, -0.742023,
      -0.736084, -0.730155, -0.724237, -0.718328, -0.712429,
      -0.706541, -0.700661, -0.694791, -0.688931, -0.683079,
      -0.677236, -0.671402, -0.665576, -0.659759, -0.653950,
      -0.648149, -0.642356, -0.636570, -0.630793, -0.625022,
      -0.619259, -0.613503, -0.607754, -0.602012, -0.596276,
      -0.590548, -0.584825, -0.579109, -0.573399, -0.567695,
      -0.561997, -0.556305, -0.550618, -0.544937, -0.539262,
      -0.533592, -0.527926, -0.522266, -0.516611, -0.510961,
      -0.505315, -0.499674, -0.494037, -0.488405, -0.482777,
      -0.477153, -0.471533, -0.465917, -0.460305, -0.454697,
      -0.449092, -0.443491, -0.437893, -0.432299, -0.426707,
      -0.421119, -0.415534, -0.409951, -0.404372, -0.398795,
      -0.393221, -0.387649, -0.382080, -0.376513, -0.370949,
      -0.365387, -0.359826, -0.354268, -0.348712, -0.343157,
      -0.337604, -0.332053, -0.326503, -0.320955, -0.315408,
      -0.309863, -0.304318, -0.298775, -0.293233, -0.287692,
      -0.282152, -0.276613, -0.271074, -0.265536, -0.259999,
      -0.254462, -0.248926, -0.243389, -0.237854, -0.232318,
      -0.226783, -0.221247, -0.215712, -0.210176, -0.204641,
      -0.199105, -0.193568, -0.188032, -0.182495, -0.176957,
      -0.171419, -0.165880, -0.160341, -0.154800, -0.149259,
      -0.143717, -0.138173, -0.132629, -0.127083, -0.121537,
      -0.115989, -0.110439, -0.104889, -0.099336, -0.093782,
      -0.088227, -0.082670, -0.077111, -0.071550, -0.065987,
      -0.060423, -0.054856, -0.049288, -0.043717, -0.038144,
      -0.032569, -0.026991, -0.021411, -0.015828, -0.010243,
      -0.004656, 00.000934, 00.006527, 00.012123, 00.017722,
      00.023323, 00.028928, 00.034535, 00.040146, 00.045759,
      00.051376, 00.056997, 00.062620, 00.068247, 00.073877,
      00.079511, 00.085149, 00.090790, 00.096435, 00.102083,
      00.107736, 00.113392, 00.119052, 00.124716, 00.130385,
      00.136057, 00.141734, 00.147414, 00.153100, 00.158789,
      00.164483, 00.170181, 00.175884, 00.181592, 00.187304,
      00.193021, 00.198743, 00.204469, 00.210201, 00.215937,
      00.221678, 00.227425, 00.233177, 00.238933, 00.244696,
      00.250463, 00.256236, 00.262014, 00.267798, 00.273587,
      00.279382, 00.285183, 00.290989, 00.296801, 00.302619,
      00.308443, 00.314273, 00.320109, 00.325951, 00.331799,
      00.337654, 00.343515, 00.349382, 00.355255, 00.361135,
      00.367022, 00.372915, 00.378815, 00.384721, 00.390634,
      00.396554, 00.402481, 00.408415, 00.414356, 00.420304,
      00.426260, 00.432222, 00.438192, 00.444169, 00.450153,
      00.456145, 00.462144, 00.468151, 00.474166, 00.480188,
      00.486218, 00.492256, 00.498302, 00.504356, 00.510418,
      00.516488, 00.522566, 00.528653, 00.534747, 00.540850,
      00.546962, 00.553082, 00.559210, 00.565347, 00.571493,
      00.577648, 00.583811, 00.589983, 00.596164, 00.602355,
      00.608554, 00.614762, 00.620980, 00.627207, 00.633444,
      00.639689, 00.645945, 00.652210, 00.658484, 00.664768,
      00.671062, 00.677366, 00.683680, 00.690004, 00.696338,
      00.702682, 00.709036, 00.715400, 00.721775, 00.728160,
      00.734556, 00.740963, 00.747379, 00.753807, 00.760246,
      00.766695, 00.773155, 00.779627, 00.786109, 00.792603,
      00.799107, 00.805624, 00.812151, 00.818690, 00.825241,
      00.831803, 00.838377, 00.844962, 00.851560, 00.858170,
      00.864791, 00.871425, 00.878071, 00.884729, 00.891399,
      00.898082, 00.904778, 00.911486, 00.918206, 00.924940,
      00.931686, 00.938446, 00.945218, 00.952003, 00.958802,
      00.965614, 00.972439, 00.979278, 00.986130, 00.992996,
      00.999875, 01.006769, 01.013676, 01.020597, 01.027533,
      01.034482, 01.041446, 01.048424, 01.055417, 01.062424,
      01.069446, 01.076482, 01.083534, 01.090600, 01.097681,
      01.104778, 01.111889, 01.119016, 01.126159, 01.133316,
      01.140490, 01.147679, 01.154884, 01.162105, 01.169342,
      01.176595, 01.183864, 01.191149, 01.198451, 01.205770,
      01.213105, 01.220457, 01.227826, 01.235211, 01.242614,
      01.250034, 01.257471, 01.264926, 01.272398, 01.279888,
      01.287395, 01.294921, 01.302464, 01.310026, 01.317605,
      01.325203, 01.332819, 01.340454, 01.348108, 01.355780,
      01.363472, 01.371182, 01.378912, 01.386660, 01.394429,
      01.402216, 01.410024, 01.417851, 01.425698, 01.433565,
      01.441453, 01.449360, 01.457288, 01.465237, 01.473206,
      01.481196, 01.489208, 01.497240, 01.505293, 01.513368,
      01.521465, 01.529583, 01.537723, 01.545885, 01.554068,
      01.562275, 01.570503, 01.578754, 01.587028, 01.595325,
      01.603644, 01.611987, 01.620353, 01.628743, 01.637156,
      01.645593, 01.654053, 01.662538, 01.671047, 01.679581,
      01.688139, 01.696721, 01.705329, 01.713961, 01.722619,
      01.731303, 01.740011, 01.748746, 01.757506, 01.766293,
      01.775106, 01.783945, 01.792810, 01.801703, 01.810623,
      01.819569, 01.828543, 01.837545, 01.846574, 01.855631,
      01.864717, 01.873830, 01.882972, 01.892143, 01.901343,
      01.910572, 01.919830, 01.929117, 01.938434, 01.947781,
      01.957158, 01.966566, 01.976004, 01.985473, 01.994972,
      02.004503, 02.014065, 02.023659, 02.033285, 02.042943,
      02.052633, 02.062355, 02.072110, 02.081899, 02.091720,
      02.101575, 02.111464, 02.121386, 02.131343, 02.141334,
      02.151360, 02.161421, 02.171517, 02.181648, 02.191815,
      02.202018, 02.212257, 02.222533, 02.232845, 02.243195,
      02.253582, 02.264006, 02.274468, 02.284968, 02.295507,
      02.306084, 02.316701, 02.327356, 02.338051, 02.348786,
      02.359562, 02.370377, 02.381234, 02.392131, 02.403070,
      02.414051, 02.425073, 02.436138, 02.447246, 02.458397,
      02.469591, 02.480828, 02.492110, 02.503436, 02.514807,
      02.526222, 02.537684, 02.549190, 02.560743, 02.572343,
      02.583989, 02.595682, 02.607423, 02.619212, 02.631050,
      02.642936, 02.654871, 02.666855, 02.678890, 02.690975,
      02.703110, 02.715297, 02.727535, 02.739825, 02.752168,
      02.764563, 02.777012, 02.789514, 02.802070, 02.814681,
      02.827347, 02.840069, 02.852846, 02.865680, 02.878570,
      02.891518, 02.904524, 02.917588, 02.930712, 02.943894,
      02.957136, 02.970439, 02.983802, 02.997227, 03.010714,
      03.024263, 03.037875, 03.051551, 03.065290, 03.079095,
      03.092965, 03.106900, 03.120902, 03.134971, 03.149107,
      03.163312, 03.177585, 03.191928, 03.206340, 03.220824,
      03.235378, 03.250005, 03.264704, 03.279477, 03.294323,
      03.309244, 03.324240, 03.339312, 03.354461, 03.369687,
      03.384992, 03.400375, 03.415838, 03.431381, 03.447005,
      03.462711, 03.478500, 03.494372, 03.510328, 03.526370,
      03.542497, 03.558711, 03.575012, 03.591402, 03.607881,
      03.624450, 03.641111, 03.657863, 03.674708, 03.691646,
      03.708680, 03.725809, 03.743034, 03.760357, 03.777779,
      03.795300, 03.812921, 03.830645, 03.848470, 03.866400,
      03.884434, 03.902574, 03.920821, 03.939176, 03.957640,
      03.976215, 03.994901, 04.013699, 04.032612, 04.051639,
      04.070783, 04.090045, 04.109425, 04.128925, 04.148547,
      04.168292, 04.188160, 04.208154, 04.228275, 04.248524,
      04.268903, 04.289413, 04.310056, 04.330832, 04.351745,
      04.372794, 04.393982, 04.415310, 04.436781, 04.458395,
      04.480154, 04.502060, 04.524114, 04.546319, 04.568676,
      04.591187, 04.613854, 04.636678, 04.659662, 04.682807,
      04.706116, 04.729590, 04.753231, 04.777041, 04.801024,
      04.825179, 04.849511, 04.874020, 04.898710, 04.923582,
      04.948639, 04.973883, 04.999316, 05.024942, 05.050761,
      05.076778, 05.102993, 05.129411, 05.156034, 05.182864,
      05.209903, 05.237156, 05.264625, 05.292312, 05.320220,
      05.348354, 05.376714, 05.405306, 05.434131, 05.463193,
      05.492496, 05.522042, 05.551836, 05.581880, 05.612178,
      05.642734, 05.673552, 05.704634, 05.735986, 05.767610,
      05.799512, 05.831694, 05.864161, 05.896918, 05.929968,
      05.963316, 05.996967, 06.030925, 06.065194, 06.099780,
      06.134687, 06.169921, 06.205486, 06.241387, 06.277630,
      06.314220, 06.351163, 06.388465, 06.426130, 06.464166,
      06.502578, 06.541371, 06.580553, 06.620130, 06.660109,
      06.700495, 06.741297, 06.782520, 06.824173, 06.866262,
      06.908795, 06.951780, 06.995225, 07.039137, 07.083525,
      07.128398, 07.173764, 07.219632, 07.266011, 07.312910,
      07.360339, 07.408308, 07.456827, 07.505905, 07.555554,
      07.605785, 07.656608, 07.708035, 07.760077, 07.812747,
      07.866057, 07.920019, 07.974647, 08.029953, 08.085952,
      08.142657, 08.200083, 08.258245, 08.317158, 08.376837,
      08.437300, 08.498562, 08.560641, 08.623554, 08.687319,
      08.751955, 08.817481, 08.883916, 08.951282, 09.019600,
      09.088889, 09.159174, 09.230477, 09.302822, 09.376233,
      09.450735, 09.526355, 09.603118, 09.681054, 09.760191,
      09.840558, 09.922186, 10.005107, 10.089353, 10.174959,
      10.261958, 10.350389, 10.440287, 10.531693, 10.624646,
      10.719188, 10.815362, 10.913214, 11.012789, 11.114137,
      11.217307, 11.322352, 11.429325, 11.538283, 11.649285,
      11.762390, 11.877664, 11.995170, 12.114979, 12.237161,
      12.361791, 12.488946, 12.618708, 12.751161, 12.886394,
      13.024498, 13.165570, 13.309711, 13.457026, 13.607625,
      13.761625, 13.919145, 14.080314, 14.245263, 14.414134,
      14.587072, 14.764233, 14.945778, 15.131877, 15.322712,
      15.518470, 15.719353, 15.925570, 16.137345, 16.354912,
      16.578520, 16.808433, 17.044929, 17.288305, 17.538873,
      17.796967, 18.062943, 18.337176, 18.620068, 18.912049,
      19.213574, 19.525133, 19.847249, 20.180480, 20.525429,
      20.882738, 21.253102, 21.637266, 22.036036, 22.450278,
      22.880933, 23.329017, 23.795634, 24.281981, 24.789364,
      25.319207, 25.873062, 26.452634, 27.059789, 27.696581,
      28.365274, 29.068370, 29.808638, 30.589157, 31.413354,
      32.285060, 33.208568, 34.188705, 35.230920, 36.341388,
      37.527131, 38.796172, 40.157721, 41.622399, 43.202525,
      44.912465, 46.769077, 48.792279, 51.005773, 53.437996,
      56.123356, 59.103894
    };
  double X, U, V, RANLAN;
  int I;

  X = gsl_rng_uniform_pos(r);
  U = 1000.0 * X;
  I = U;
  U = U - I;

  if (I >= 70 && I <= 800)
    {
      RANLAN = F[I] + U * (F[I + 1] - F[I]);
    }
  else if (I >= 7 && I <= 980)
    {
      RANLAN = F[I] 
        + U * (F[I + 1] - F[I] 
               - 0.25 * (1 - U) * (F[I + 2] - F[I + 1] - F[I] + F[I - 1]));
    }
  else if (I < 7)
    {
      V = log(X);
      U = 1 / V;
      RANLAN = ((0.99858950 + (3.45213058E1 + 1.70854528E1 * U) * U) /
                (1 + (3.41760202E1 + 4.01244582 * U) * U)) *
               ( -log( -0.91893853 - V) - 1);
    }
  else
    {
      U = 1 - X;
      V = U * U;
      if (X <= 0.999)
        {
          RANLAN = (1.00060006 + 2.63991156E2 * U + 4.37320068E3 * V) /
                   ((1 + 2.57368075E2 * U + 3.41448018E3 * V) * U);
        }
      else
        {
          RANLAN = (1.00001538 + 6.07514119E3 * U + 7.34266409E5 * V) /
                   ((1 + 6.06511919E3 * U + 6.94021044E5 * V) * U);
        }
    }

  return RANLAN;
}

