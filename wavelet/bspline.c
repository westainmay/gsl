/* wavelet/bspline.c
 * 
 * Copyright (C) 2004 Ivo Alxneit
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

/* Coefficients are from A. Cohen, I. Daubechies, and J.-C. Feauveau;
 * "Biorthogonal Bases of Compactly Supported Wavelets", Communications
 * on Pure and Apllied Mathematics, 45 (1992) 485--560 (table 6.1).
 *
 * Note the following errors in table 1:
 *
 * N = 2, N~ = 4, m0~
 * the second term in z^-1 (45/64 z^-1) should be left out.
 *
 * N = 3, N~ = 7, m0~
 * the term 336z^-3 should read 363z^-3.
 */

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_wavelet.h>

static const double h1_103[6] = { -0.0883883476483184405501055452631,
  0.0883883476483184405501055452631,
  M_SQRT1_2,
  M_SQRT1_2,
  0.0883883476483184405501055452631,
  -0.0883883476483184405501055452631
};

static const double g2_103[6] = { -0.0883883476483184405501055452631,
  -0.0883883476483184405501055452631,
  M_SQRT1_2,
  -(M_SQRT1_2),
  0.0883883476483184405501055452631,
  0.0883883476483184405501055452631
};

static const double h1_105[10] = { 0.0165728151840597076031447897368,
  -0.0165728151840597076031447897368,
  -0.1215339780164378557563951247368,
  0.1215339780164378557563951247368,
  M_SQRT1_2,
  M_SQRT1_2,
  0.1215339780164378557563951247368,
  -0.1215339780164378557563951247368,
  -0.0165728151840597076031447897368,
  0.0165728151840597076031447897368
};

static const double g2_105[10] = { 0.0165728151840597076031447897368,
  0.0165728151840597076031447897368,
  -0.1215339780164378557563951247368,
  -0.1215339780164378557563951247368,
  M_SQRT1_2,
  -(M_SQRT1_2),
  0.1215339780164378557563951247368,
  0.1215339780164378557563951247368,
  -0.0165728151840597076031447897368,
  -0.0165728151840597076031447897368
};

static const double g1_1[10] = { 0.0, 0.0, 0.0, 0.0,
  M_SQRT1_2,
  -(M_SQRT1_2),
  0.0, 0.0, 0.0, 0.0
};

static const double h2_1[10] = { 0.0, 0.0, 0.0, 0.0,
  M_SQRT1_2,
  M_SQRT1_2,
  0.0, 0.0, 0.0, 0.0
};

static const double h1_202[6] = { -0.1767766952966368811002110905262,
  0.3535533905932737622004221810524,
  1.0606601717798212866012665431573,
  0.3535533905932737622004221810524,
  -0.1767766952966368811002110905262,
  0.0
};

static const double g2_202[6] = { 0.0,
  -0.1767766952966368811002110905262,
  -0.3535533905932737622004221810524,
  1.0606601717798212866012665431573,
  -0.3535533905932737622004221810524,
  -0.1767766952966368811002110905262
};

static const double h1_204[10] = { 0.0331456303681194152062895794737,
  -0.0662912607362388304125791589473,
  -0.1767766952966368811002110905262,
  0.4198446513295125926130013399998,
  0.9943689110435824561886873842099,
  0.4198446513295125926130013399998,
  -0.1767766952966368811002110905262,
  -0.0662912607362388304125791589473,
  0.0331456303681194152062895794737,
  0.0
};

static const double g2_204[10] = { 0.0,
  0.0331456303681194152062895794737,
  0.0662912607362388304125791589473,
  -0.1767766952966368811002110905262,
  -0.4198446513295125926130013399998,
  0.9943689110435824561886873842099,
  -0.4198446513295125926130013399998,
  -0.1767766952966368811002110905262,
  0.0662912607362388304125791589473,
  0.0331456303681194152062895794737
};

static const double h1_206[14] = { -0.0069053396600248781679769957237,
  0.0138106793200497563359539914474,
  0.0469563096881691715422435709210,
  -0.1077232986963880994204411332894,
  -0.1698713556366120029322340948025,
  0.4474660099696121052849093228945,
  0.9667475524034829435167794013152,
  0.4474660099696121052849093228945,
  -0.1698713556366120029322340948025,
  -0.1077232986963880994204411332894,
  0.0469563096881691715422435709210,
  0.0138106793200497563359539914474,
  -0.0069053396600248781679769957237,
  0.0
};

static const double g2_206[14] = { 0.0,
  -0.0069053396600248781679769957237,
  -0.0138106793200497563359539914474,
  0.0469563096881691715422435709210,
  0.1077232986963880994204411332894,
  -0.1698713556366120029322340948025,
  -0.4474660099696121052849093228945,
  0.9667475524034829435167794013152,
  -0.4474660099696121052849093228945,
  -0.1698713556366120029322340948025,
  0.1077232986963880994204411332894,
  0.0469563096881691715422435709210,
  -0.0138106793200497563359539914474,
  -0.0069053396600248781679769957237,
};

static const double h1_208[18] = { 0.0015105430506304420992449678146,
  -0.0030210861012608841984899356291,
  -0.0129475118625466465649568669819,
  0.0289161098263541773284036695929,
  0.0529984818906909399392234421792,
  -0.1349130736077360572068505539514,
  -0.1638291834340902345352542235443,
  0.4625714404759165262773590010400,
  0.9516421218971785225243297231697,
  0.4625714404759165262773590010400,
  -0.1638291834340902345352542235443,
  -0.1349130736077360572068505539514,
  0.0529984818906909399392234421792,
  0.0289161098263541773284036695929,
  -0.0129475118625466465649568669819,
  -0.0030210861012608841984899356291,
  0.0015105430506304420992449678146,
  0.0
};

static const double g2_208[18] = { 0.0,
  0.0015105430506304420992449678146,
  0.0030210861012608841984899356291,
  -0.0129475118625466465649568669819,
  -0.0289161098263541773284036695929,
  0.0529984818906909399392234421792,
  0.1349130736077360572068505539514,
  -0.1638291834340902345352542235443,
  -0.4625714404759165262773590010400,
  0.9516421218971785225243297231697,
  -0.4625714404759165262773590010400,
  -0.1638291834340902345352542235443,
  0.1349130736077360572068505539514,
  0.0529984818906909399392234421792,
  -0.0289161098263541773284036695929,
  -0.0129475118625466465649568669819,
  0.0030210861012608841984899356291,
  0.0015105430506304420992449678146,
};

static const double h2_2[18] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.3535533905932737622004221810524,
  0.7071067811865475244008443621048,
  0.3535533905932737622004221810524,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static const double g1_2[18] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  -0.3535533905932737622004221810524,
  0.7071067811865475244008443621048,
  -0.3535533905932737622004221810524,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static const double h1_301[4] = { -0.3535533905932737622004221810524,
  1.0606601717798212866012665431573,
  1.0606601717798212866012665431573,
  -0.3535533905932737622004221810524
};

static const double g2_301[4] = { 0.3535533905932737622004221810524,
  1.0606601717798212866012665431573,
  -1.0606601717798212866012665431573,
  -0.3535533905932737622004221810524
};

static const double h1_303[8] = { 0.0662912607362388304125791589473,
  -0.1988737822087164912377374768420,
  -0.1546796083845572709626847042104,
  0.9943689110435824561886873842099,
  0.9943689110435824561886873842099,
  -0.1546796083845572709626847042104,
  -0.1988737822087164912377374768420,
  0.0662912607362388304125791589473
};

static const double g2_303[8] = { -0.0662912607362388304125791589473,
  -0.1988737822087164912377374768420,
  0.1546796083845572709626847042104,
  0.9943689110435824561886873842099,
  -0.9943689110435824561886873842099,
  -0.1546796083845572709626847042104,
  0.1988737822087164912377374768420,
  0.0662912607362388304125791589473
};

static const double h1_305[12] = { -0.0138106793200497563359539914474,
  0.0414320379601492690078619743421,
  0.0524805814161890740766251675000,
  -0.2679271788089652729175074340788,
  -0.0718155324642587329469607555263,
  0.9667475524034829435167794013152,
  0.9667475524034829435167794013152,
  -0.0718155324642587329469607555263,
  -0.2679271788089652729175074340788,
  0.0524805814161890740766251675000,
  0.0414320379601492690078619743421,
  -0.0138106793200497563359539914474
};

static const double g2_305[12] = { 0.0138106793200497563359539914474,
  0.0414320379601492690078619743421,
  -0.0524805814161890740766251675000,
  -0.2679271788089652729175074340788,
  0.0718155324642587329469607555263,
  0.9667475524034829435167794013152,
  -0.9667475524034829435167794013152,
  -0.0718155324642587329469607555263,
  0.2679271788089652729175074340788,
  0.0524805814161890740766251675000,
  -0.0414320379601492690078619743421,
  -0.0138106793200497563359539914474
};

static const double h1_307[16] = { 0.0030210861012608841984899356291,
  -0.0090632583037826525954698068873,
  -0.0168317654213106405344439270765,
  0.0746639850740189951912512662623,
  0.0313329787073628846871956180962,
  -0.3011591259228349991008967259990,
  -0.0264992409453454699696117210896,
  0.9516421218971785225243297231697,
  0.9516421218971785225243297231697,
  -0.0264992409453454699696117210896,
  -0.3011591259228349991008967259990,
  0.0313329787073628846871956180962,
  0.0746639850740189951912512662623,
  -0.0168317654213106405344439270765,
  -0.0090632583037826525954698068873,
  0.0030210861012608841984899356291
};

static const double g2_307[16] = { -0.0030210861012608841984899356291,
  -0.0090632583037826525954698068873,
  0.0168317654213106405344439270765,
  0.0746639850740189951912512662623,
  -0.0313329787073628846871956180962,
  -0.3011591259228349991008967259990,
  0.0264992409453454699696117210896,
  0.9516421218971785225243297231697,
  -0.9516421218971785225243297231697,
  -0.0264992409453454699696117210896,
  0.3011591259228349991008967259990,
  0.0313329787073628846871956180962,
  -0.0746639850740189951912512662623,
  -0.0168317654213106405344439270765,
  0.0090632583037826525954698068873,
  0.0030210861012608841984899356291
};

static const double h1_309[20] = { -0.0006797443727836989446602355165,
  0.0020392331183510968339807065496,
  0.0050603192196119810324706421788,
  -0.0206189126411055346546938106687,
  -0.0141127879301758447558029850103,
  0.0991347824942321571990197448581,
  0.0123001362694193142367090236328,
  -0.3201919683607785695513833204624,
  0.0020500227115698857061181706055,
  0.9421257006782067372990864259380,
  0.9421257006782067372990864259380,
  0.0020500227115698857061181706055,
  -0.3201919683607785695513833204624,
  0.0123001362694193142367090236328,
  0.0991347824942321571990197448581,
  -0.0141127879301758447558029850103,
  -0.0206189126411055346546938106687,
  0.0050603192196119810324706421788,
  0.0020392331183510968339807065496,
  -0.0006797443727836989446602355165
};

static const double g2_309[20] = { 0.0006797443727836989446602355165,
  0.0020392331183510968339807065496,
  -0.0050603192196119810324706421788,
  -0.0206189126411055346546938106687,
  0.0141127879301758447558029850103,
  0.0991347824942321571990197448581,
  -0.0123001362694193142367090236328,
  -0.3201919683607785695513833204624,
  -0.0020500227115698857061181706055,
  0.9421257006782067372990864259380,
  -0.9421257006782067372990864259380,
  0.0020500227115698857061181706055,
  0.3201919683607785695513833204624,
  0.0123001362694193142367090236328,
  -0.0991347824942321571990197448581,
  -0.0141127879301758447558029850103,
  0.0206189126411055346546938106687,
  0.0050603192196119810324706421788,
  -0.0020392331183510968339807065496,
  -0.0006797443727836989446602355165
};

static const double h2_3[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.1767766952966368811002110905262,
  0.5303300858899106433006332715786,
  0.5303300858899106433006332715786,
  0.1767766952966368811002110905262,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static const double g1_3[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  -0.1767766952966368811002110905262,
  0.5303300858899106433006332715786,
  -0.5303300858899106433006332715786,
  0.1767766952966368811002110905262,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static int
bspline_init (const double **h1, const double **g1,
              const double **h2, const double **g2, size_t * nc,
              size_t * offset, size_t member)
{
  switch (member)
    {
    case 103:
      *nc = 6;
      *h1 = h1_103;
      *g1 = &g1_1[2];
      *h2 = &h2_1[2];
      *g2 = g2_103;
      break;

    case 105:
      *nc = 10;
      *h1 = h1_105;
      *g1 = g1_1;
      *h2 = h2_1;
      *g2 = g2_105;
      break;

    case 202:
      *nc = 6;
      *h1 = h1_202;
      *g1 = &g1_2[6];
      *h2 = &h2_2[6];
      *g2 = g2_202;
      break;

    case 204:
      *nc = 10;
      *h1 = h1_204;
      *g1 = &g1_2[4];
      *h2 = &h2_2[4];
      *g2 = g2_204;
      break;

    case 206:
      *nc = 14;
      *h1 = h1_206;
      *g1 = &g1_2[2];
      *h2 = &h2_2[2];
      *g2 = g2_206;
      break;

    case 208:
      *nc = 18;
      *h1 = h1_208;
      *g1 = g1_2;
      *h2 = h2_2;
      *g2 = g2_208;
      break;

    case 301:
      *nc = 4;
      *h1 = h1_301;
      *g1 = &g1_3[8];
      *h2 = &h2_3[8];
      *g2 = g2_301;
      break;

    case 303:
      *nc = 8;
      *h1 = h1_303;
      *g1 = &g1_3[6];
      *h2 = &h2_3[6];
      *g2 = g2_303;
      break;

    case 305:
      *nc = 12;
      *h1 = h1_305;
      *g1 = &g1_3[4];
      *h2 = &h2_3[4];
      *g2 = g2_305;
      break;

    case 307:
      *nc = 16;
      *h1 = h1_307;
      *g1 = &g1_3[2];
      *h2 = &h2_3[2];
      *g2 = g2_307;
      break;

    case 309:
      *nc = 20;
      *h1 = h1_309;
      *g1 = g1_3;
      *h2 = h2_3;
      *g2 = g2_309;
      break;

    default:
      return GSL_FAILURE;
    }

  *offset = 0;

  return GSL_SUCCESS;
}

static int
bspline_centered_init (const double **h1, const double **g1,
                       const double **h2, const double **g2, size_t * nc,
                       size_t * offset, size_t member)
{
  switch (member)
    {
    case 103:
      *nc = 6;
      *h1 = h1_103;
      *g1 = &g1_1[2];
      *h2 = &h2_1[2];
      *g2 = g2_103;
      break;

    case 105:
      *nc = 10;
      *h1 = h1_105;
      *g1 = g1_1;
      *h2 = h2_1;
      *g2 = g2_105;
      break;

    case 202:
      *nc = 6;
      *h1 = h1_202;
      *g1 = &g1_2[6];
      *h2 = &h2_2[6];
      *g2 = g2_202;
      break;

    case 204:
      *nc = 10;
      *h1 = h1_204;
      *g1 = &g1_2[4];
      *h2 = &h2_2[4];
      *g2 = g2_204;
      break;

    case 206:
      *nc = 14;
      *h1 = h1_206;
      *g1 = &g1_2[2];
      *h2 = &h2_2[2];
      *g2 = g2_206;
      break;

    case 208:
      *nc = 18;
      *h1 = h1_208;
      *g1 = g1_2;
      *h2 = h2_2;
      *g2 = g2_208;
      break;

    case 301:
      *nc = 4;
      *h1 = h1_301;
      *g1 = &g1_3[8];
      *h2 = &h2_3[8];
      *g2 = g2_301;
      break;

    case 303:
      *nc = 8;
      *h1 = h1_303;
      *g1 = &g1_3[6];
      *h2 = &h2_3[6];
      *g2 = g2_303;
      break;

    case 305:
      *nc = 12;
      *h1 = h1_305;
      *g1 = &g1_3[4];
      *h2 = &h2_3[4];
      *g2 = g2_305;
      break;

    case 307:
      *nc = 16;
      *h1 = h1_307;
      *g1 = &g1_3[2];
      *h2 = &h2_3[2];
      *g2 = g2_307;
      break;

    case 309:
      *nc = 20;
      *h1 = h1_309;
      *g1 = g1_3;
      *h2 = h2_3;
      *g2 = g2_309;
      break;

    default:
      return GSL_FAILURE;
    }

  *offset = ((*nc) >> 1);

  return GSL_SUCCESS;
}

const gsl_wavelet_type bspline_type = {
  "bspline",
  &bspline_init
};

const gsl_wavelet_type bspline_centered_type = { 
  "bspline-centered",
  &bspline_centered_init
};

const gsl_wavelet_type *gsl_wavelet_bspline = &bspline_type;
const gsl_wavelet_type *gsl_wavelet_bspline_centered = &bspline_centered_type;
