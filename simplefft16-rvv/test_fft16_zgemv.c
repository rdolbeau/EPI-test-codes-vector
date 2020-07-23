/*

RISC-V V DFT/FFT demo code.

Copyright ? 2020 Romain Dolbeau <Romain.Dolbeau@sipearl.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "test_fft16.h"
#include <malloc.h>
#include <math.h>
#include <complex.h>

#if !defined(__riscv) || defined(BLAS)
#if defined(__INTEL_COMPILER)
#include <mkl.h>
#else
#warning "Must define some sort of BLAS headers"
#define BLIS_ENABLE_CBLAS 1
#include "blis.h"
#endif

#define gemv_double_fft16(a,b,c,d,e) gemv_double_fft16_blas(a,b,c,d,e)

#else

#if !defined(RISCV_V_VARIANT1) && !defined(RISCV_V_VARIANT2) &&         \
        !defined(RISCV_V_VARIANT2B) && !defined(RISCV_V_VARIANT3) &&    \
        !defined(RISCV_V_VARIANT4)  && !defined(RISCV_V_VARIANT5) &&	\
	!defined(RISCV_V_VARIANT6)  && !defined(RISCV_V_VARIANT7) &&    \
	!defined(RISCV_V_VARIANT8)  && !defined(RISCV_V_VARIANT9) &&	\
	!defined(RISCV_V_VARIANT10) && !defined(RISCV_V_VARIANT11)
#error "should pick a variant"
#endif

#if defined(RISCV_V_TRUESEGLS)
#define T(X) X##T
#else
#define T(X) X
#endif

#if defined(RISCV_V_VARIANT1)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_1)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT2)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_2)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT2B)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_2b)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT3)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_3)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT4)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_4)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT5)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_5)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT6)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_6)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT7)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_7)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT8)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_8)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT9)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_9)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT10)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_10)(a,b,c,d,e)
#elif defined(RISCV_V_VARIANT11)
#define gemv_double_fft16(a,b,c,d,e) T(gemv_double_fft16_11)(a,b,c,d,e)
#endif


#define printiv64(s, v, vl)					\
	{							\
	        long int data[64];				\
	        __builtin_epi_vstore_1xi64(data, v, vl);	\
	        printf("%s "#v": ", s);				\
	        /*printarray(data, vl);*/			\
		for (int i = 0 ; i < vl ; i++)			\
			printf("(%d) %ld ", i, data[i]);	\
								\
	        printf("\n");					\
	}
#define printfv64(s, v, vl)					\
	{							\
		double data[64];				\
		__builtin_epi_vstore_1xf64(data, v, vl);	\
		printf("%s "#v": ", s);				\
		/*printarray(data, vl);*/			\
		for (int i = 0 ; i < vl ; i++)			\
			printf("(%d) %lf ", i, data[i]);	\
								\
		printf("\n");					\
	}

#include "vf64.h"

#endif // __riscv

static double *mst;

void
gemv_double_fft16 (
		    /* int n */
		    double *in, double *out, int v, int ivs, int ovs)
{
  //      double *mst = calloc(sizeof(double), 512);
#define SIZE 16
#define matrix(i,j) ((double complex*)mst)[i+j*SIZE]
  int k, n, i, j;

  if (mst == NULL)
    {
      mst = calloc (sizeof (double), SIZE * SIZE * 2);
      for (k = 0; k < SIZE; k++)
	{
	  for (n = 0; n < SIZE; n++)
	    {
	      matrix (k, n) = cexp (-2. * M_PI * I * k * n / ((double) SIZE));
	    }
	}
    }
#if 0
  for (k = 0; k < SIZE; k++)
    {
      for (n = 0; n < SIZE; n++)
	{
	  printf ("% 3.2f%+ 3.2fi ", creal (matrix (k, n)), cimag (matrix (k, n)));
	}
      printf ("\n");
    }


#endif

  for (j = 0; j < v; j++, in += ivs, out += ovs)
    {

#if !defined(__riscv) || defined(BLAS)
      // non-RISC-V platform: use BLAS for checking
      const double complex alpha = 1.;
      const double complex beta = 0.;
      cblas_zgemv (CblasRowMajor, CblasNoTrans, 16, 16, &alpha, mst, 16, in, 1, &beta, out, 1);
      //      cblas_zsymv(CblasRowMajor, CblasNoTrans, 16, &alpha, mst, 16, in, 1, &beta, out, 1);
#else

#if defined(RISCV_V_TRUESEGLS)
#define fake_vlseg2e(addr, o1, o2, vc)					\
      do {								\
	      __epi_1xf64x2 l = __builtin_epi_vlseg2_1xf64x2((addr), vc); \
	      o1 = l.v0;						\
	      o2 = l.v1;						\
      } while (0)

#define fake_vsseg2e(addr, o1, o2, vc)					\
      do {								\
	      __epi_1xf64x2 l;						\
	      l.v0 = o1;						\
	      l.v1 = o2;						\
	      __builtin_epi_vsseg2_1xf64x2((addr), l, vc);		\
      } while (0)

#else
      /* could also use two indexed load */
#define fake_vlseg2e(addr, o1, o2, vc)					\
	do {								\
		__epi_1xf64 l1 = __builtin_epi_vload_1xf64((addr), vc);	\
		__epi_1xf64 l2 = __builtin_epi_vload_1xf64((addr) + vc, vc); \
		o1 = __builtin_epi_uzp1_1xf64(l1, l2, vc);		\
		o2 = __builtin_epi_uzp2_1xf64(l1, l2, vc);		\
	} while (0)


      /* could also use two indexed store */
#define fake_vsseg2e(addr, o1, o2, vc)					\
	do {								\
		__epi_1xf64 l1 = __builtin_epi_zip1_1xf64(o1, o2, vc);	\
		__epi_1xf64 l2 = __builtin_epi_zip2_1xf64(o1, o2, vc);	\
		__builtin_epi_vstore_1xf64((addr), l1, vc);		\
		__builtin_epi_vstore_1xf64((addr) + vc, l2, vc);	\
	} while (0)
#endif

#define fake_vlseg4e(addr, o1, o2, o3, o4, vc) \
do { \
	__epi_1xf64 il1, il2, il3, il4;\
	fake_vlseg2e((addr), il1, il2, 16);\
	fake_vlseg2e((addr) + 2*vc, il3, il4, 16);\
                o1 = __builtin_epi_uzp1_1xf64(il1, il3, vc);              \
                o2 = __builtin_epi_uzp2_1xf64(il1, il3, vc);              \
                o3 = __builtin_epi_uzp1_1xf64(il2, il4, vc);              \
                o4 = __builtin_epi_uzp2_1xf64(il2, il4, vc);              \
} while (0)

#define fake_vsseg4e(addr, o1, o2, o3, o4, vc)                          \
        do {                                                            \
                __epi_1xf64 il1 = __builtin_epi_zip1_1xf64(o1, o3, vc);  \
                __epi_1xf64 il2 = __builtin_epi_zip2_1xf64(o1, o3, vc);  \
                __epi_1xf64 il3 = __builtin_epi_zip1_1xf64(o2, o4, vc);  \
                __epi_1xf64 il4 = __builtin_epi_zip2_1xf64(o2, o4, vc);  \
		fake_vsseg2e((addr), il1, il3, vc);\
		fake_vsseg2e((addr) + 2*vc, il2, il4, vc);\
        } while (0)



#if defined(RISCV_V_VARIANT1)
      // (1) straightforward ZGEMV, essentially N * ZDOT, store 1 value at a time
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
      fake_vlseg2e (in, vinr, vini, 16);
      for (i = 0; i < 16; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __builtin_epi_vstore_1xf64 (out + 2 * i, vr, 1);
	  __builtin_epi_vstore_1xf64 (out + 2 * i + 1, vi, 1);
	}
#elif defined(RISCV_V_VARIANT2)
      // (2) similar to (1), but accumulates 16 values in a vector register (using slide1down) and store a full vector at once
      __epi_1xf64 vall = __builtin_epi_vfmv_v_f_1xf64 (1234., 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
      fake_vlseg2e (in, vinr, vini, 16);
      union
      {
	double d;
	long l;
      } dl;
      for (i = 0; i < 8; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  dl.d = __builtin_epi_vfmv_f_s_1xf64 (vr);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, dl.l, 16);
	  dl.d = __builtin_epi_vfmv_f_s_1xf64 (vi);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, dl.l, 16);
	}
      __builtin_epi_vstore_1xf64 (out, vall, 16);
      for (i = 8; i < 16; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  dl.d = __builtin_epi_vfmv_f_s_1xf64 (vr);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, dl.l, 16);
	  dl.d = __builtin_epi_vfmv_f_s_1xf64 (vi);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, dl.l, 16);
	}
      __builtin_epi_vstore_1xf64 (out + 16, vall, 16);
#elif defined(RISCV_V_VARIANT2B)
      // (2b) similar to (2), but use slideup+vmerge after slide1down+0 instead of gslide1down+getfirst
      __epi_1xf64 vall = __builtin_epi_vfmv_v_f_1xf64 (1234., 16);
      __epi_1xi64 vizero = __builtin_epi_vmv_v_x_1xi64 (0, 16);
      __epi_1xi64 vhighone = __builtin_epi_vslide1down_1xi64 (vizero, 1, 16);
      __epi_1xi1 vmm = __builtin_epi_cast_1xi1_1xi64 (vhighone);	// mask to keep only the last lane
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
      fake_vlseg2e (in, vinr, vini, 16);
      for (i = 0; i < 8; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, 0, 16);
	  vr = __builtin_epi_vslideup_1xf64 (vr, 15, 16);
	  vall = __builtin_epi_vfmerge_1xf64 (vall, vr, vmm, 16);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, 0, 16);
	  vi = __builtin_epi_vslideup_1xf64 (vi, 15, 16);
	  vall = __builtin_epi_vfmerge_1xf64 (vall, vi, vmm, 16);
	}
      __builtin_epi_vstore_1xf64 (out, vall, 16);
      for (i = 8; i < 16; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, 0, 16);
	  vr = __builtin_epi_vslideup_1xf64 (vr, 15, 16);
	  vall = __builtin_epi_vfmerge_1xf64 (vall, vr, vmm, 16);
	  vall = __builtin_epi_vslide1down_1xf64 (vall, 0, 16);
	  vi = __builtin_epi_vslideup_1xf64 (vi, 15, 16);
	  vall = __builtin_epi_vfmerge_1xf64 (vall, vi, vmm, 16);
	}
      __builtin_epi_vstore_1xf64 (out + 16, vall, 16);
#elif defined(RISCV_V_VARIANT3)
      // (3) similar to 1, but only use a single line of constants, using indices computation + indexed loads to get the constants
      // could also be combined with slide1up
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
      fake_vlseg2e (in, vinr, vini, 16);
      for (i = 0; i < 16; i++)
	{
	  __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
	  __epi_1xi64 v240 = __builtin_epi_vmv_v_x_1xi64 (240, 16);	// 240, 240, 240, ...
	  __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v240, 16);	// vtr % 240
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vini, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vinr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vinr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vini, vmr, 16);
	  __epi_1xf64 vr = __builtin_epi_vfredsum_1xf64 (veemoo, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __epi_1xf64 vi = __builtin_epi_vfredsum_1xf64 (veomoe, __builtin_epi_vfmv_v_f_1xf64 (0., 16), 16);
	  __builtin_epi_vstore_1xf64 (out + 2 * i, vr, 1);
	  __builtin_epi_vstore_1xf64 (out + 2 * i + 1, vi, 1);
	}
#elif defined(RISCV_V_VARIANT4)
      // (4) transposed version of (1)
      __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
#if 0
      fake_vlseg2e (in, vinr, vini, 16);
#endif
      for (i = 0; i < 16; i++)
	{
	  fake_vlseg2e (mst + 32 * i, vmr, vmi, 16);
#if 0
	  __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vibi, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vibr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);

	  voutr = __builtin_epi_vfadd_1xf64 (voutr, veemoo, 16);
	  vouti = __builtin_epi_vfadd_1xf64 (vouti, veomoe, 16);
	}
      fake_vsseg2e (out, voutr, vouti, 16);
#elif defined(RISCV_V_VARIANT5)
      // (5) same as (4) but with the single-line matrix from (3)
      __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
#if 0
      fake_vlseg2e (in, vinr, vini, 16);
#endif
      for (i = 0; i < 16; i++)
	{
	  __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
	  __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64 (255, 16);	// 255, 255, 255, ...
	  __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

#if 0
	  __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  __epi_1xf64 voo = __builtin_epi_vfmul_1xf64 (vibi, vmi, 16);
	  __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  __epi_1xf64 veo = __builtin_epi_vfmul_1xf64 (vibr, vmi, 16);
	  __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);

	  voutr = __builtin_epi_vfadd_1xf64 (voutr, veemoo, 16);
	  vouti = __builtin_epi_vfadd_1xf64 (vouti, veomoe, 16);
	}
      fake_vsseg2e (out, voutr, vouti, 16);
#elif defined(RISCV_V_VARIANT6)
      // (6) same as (5) but with more fma
      __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64 (0., 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
#if 0
      fake_vlseg2e (in, vinr, vini, 16);
#endif
      for (i = 0; i < 16; i++)
	{
	  __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
	  __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64 (255, 16);	// 255, 255, 255, ...
	  __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

#if 0
	  __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  __epi_1xf64 voo = __builtin_epi_vfmsac_1xf64 (voutr, vibi, vmi, 16);	// -*- -> + for voutr
	  __epi_1xf64 veo = __builtin_epi_vfmacc_1xf64 (vouti, vibr, vmi, 16);
	  voutr = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  vouti = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);
	}
      fake_vsseg2e (out, voutr, vouti, 16);
#elif defined(RISCV_V_VARIANT7)
      // (7) same as (6) but with no iteration 0
      __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64 (in[0], 16);
      __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64 (in[1], 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
#if 0
      fake_vlseg2e (in, vinr, vini, 16);
#endif
      for (i = 1; i < 16; i++)
	{
	  __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
	  __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64 (255, 16);	// 255, 255, 255, ...
	  __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

#if 0
	  __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  __epi_1xf64 voo = __builtin_epi_vfmsac_1xf64 (voutr, vibi, vmi, 16);	// -*- -> + for voutr
	  __epi_1xf64 veo = __builtin_epi_vfmacc_1xf64 (vouti, vibr, vmi, 16);
	  voutr = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  vouti = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);
	}
      fake_vsseg2e (out, voutr, vouti, 16);
#elif defined(RISCV_V_VARIANT8)
      // (8) same as (7) but with no iteration 0, 1 or 8
      __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64 (in[0], 16);
      __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64 (in[1], 16);
      const __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
      const __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64 (255, 16);	// 255, 255, 255, ...
      const __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 vmr, vmi;
#if 0
      fake_vlseg2e (in, vinr, vini, 16);
#endif
      fake_vlseg2e ((mst + 32), vmr, vmi, 16);
#if 0
      __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64 (vinr, 1, 16);	// not supported in vehave
      __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64 (vini, 1, 16);
#else
      __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2], 16);	// is it costlier than dense load + splat ?
      __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64 (in[3], 16);	// splat & broadcast are both vrgather anyway ...
#endif
      __epi_1xf64 voo = __builtin_epi_vfmsac_1xf64 (voutr, vibi, vmi, 16);	// -*- -> + for voutr
      __epi_1xf64 veo = __builtin_epi_vfmacc_1xf64 (vouti, vibr, vmi, 16);
      voutr = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
      vouti = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);

      for (i = 2; i < 8; i++)
	{
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

#if 0
	  vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  voo = __builtin_epi_vfmsac_1xf64 (voutr, vibi, vmi, 16);	// -*- -> + for voutr
	  veo = __builtin_epi_vfmacc_1xf64 (vouti, vibr, vmi, 16);
	  voutr = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  vouti = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);
	}
      {				// i = 8 // still need to optimize awway the indices & gather -> it's just 1/-1/1/-1 ...
#if 0
	__epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (8, 16);	// i, i, i, ...
	__epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	__epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
#else
	__epi_1xi64 vmask = __builtin_epi_vand_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 1, 0
	__epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64 (vmask);
	vmr = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask, 16);
#endif
	// vmi is 0
#if 0
	vibr = __builtin_epi_vsplat_1xf64 (vinr, 8, 16);	// not supported in vehave
	vibi = __builtin_epi_vsplat_1xf64 (vini, 8, 16);
#else
	vibr = __builtin_epi_vfmv_v_f_1xf64 (in[16], 16);	// is it costlier than dense load + splat ?
	vibi = __builtin_epi_vfmv_v_f_1xf64 (in[17], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	voutr = __builtin_epi_vfmacc_1xf64 (voutr, vibr, vmr, 16);
	vouti = __builtin_epi_vfmacc_1xf64 (vouti, vibi, vmr, 16);
      }
      for (i = 9; i < 16; i++)
	{
	  __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64 (i, 16);	// i, i, i, ...
	  __epi_1xi64 vtr = __builtin_epi_vmul_1xi64 (idx16, voi, 16);	// 0, 16*i, 32*i, 48*i, ...
	  __epi_1xi64 vidxr = __builtin_epi_vand_1xi64 (vtr, v255, 16);	// vtr % 255
	  vmr = __builtin_epi_vload_indexed_1xf64 ((mst + 32), vidxr, 16);
	  vmi = __builtin_epi_vload_indexed_1xf64 ((mst + 33), vidxr, 16);

#if 0
	  vibr = __builtin_epi_vsplat_1xf64 (vinr, i, 16);	// not supported in vehave
	  vibi = __builtin_epi_vsplat_1xf64 (vini, i, 16);
#else
	  vibr = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i], 16);	// is it costlier than dense load + splat ?
	  vibi = __builtin_epi_vfmv_v_f_1xf64 (in[2 * i + 1], 16);	// splat & broadcast are both vrgather anyway ...
#endif
	  voo = __builtin_epi_vfmsac_1xf64 (voutr, vibi, vmi, 16);	// -*- -> + for voutr
	  veo = __builtin_epi_vfmacc_1xf64 (vouti, vibr, vmi, 16);
	  voutr = __builtin_epi_vfmsac_1xf64 (voo, vibr, vmr, 16);
	  vouti = __builtin_epi_vfmacc_1xf64 (veo, vibi, vmr, 16);
	}

      fake_vsseg2e (out, voutr, vouti, 16);
#elif defined(RISCV_V_VARIANT9)
      // have a go at a radix-4 FFT
#define DK(a,b) static const double a = b;
#define R double
      DK (KP382683432, +0.382683432365089771728459984030398866761344562);
      DK (KP923879532, +0.923879532511286756128183189396788286822416626);
      DK (KP707106781, +0.707106781186547524400844362104849039284835938);

      const __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
      const __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 rsum, isum;

      fake_vlseg2e (in, vinr, vini, 16);	// splitted input

      // first, 4 DFT-4 - only additions, fmacc are used to time by -1/1 only (could be done with e.g. XOR + ADD)
      // but needs a LOT of vrgather at this time
      // currently dense, fixme: transpose 4x4 ?
      {
	const __epi_1xi64 idxo4 = __builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16);	// 0, 0, 0, 0, 1, 1, ...
	__epi_1xf64 in0_8_16_24 = __builtin_epi_vrgather_1xf64 (vinr, idxo4, 16);
	__epi_1xf64 in1_9_17_25 = __builtin_epi_vrgather_1xf64 (vini, idxo4, 16);

	const __epi_1xi64 idxo4p2 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (8, 16), 16);
	__epi_1xf64 in4_12_20_28 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p2, 16);	// will need +1/-1
	__epi_1xf64 in5_13_21_29 = __builtin_epi_vrgather_1xf64 (vini, idxo4p2, 16);

	const __epi_1xi64 idxo4p1 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (4, 16), 16);
	__epi_1xf64 in2_10_18_26 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p1, 16);	// will need merging & +1/+1/-1/-1
	__epi_1xf64 in3_11_19_27 = __builtin_epi_vrgather_1xf64 (vini, idxo4p1, 16);

	const __epi_1xi64 idxo4p3 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (12, 16), 16);
	__epi_1xf64 in6_14_22_30 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p3, 16);	// will need merging & +1/-1/-1/+1
	__epi_1xf64 in7_15_23_31 = __builtin_epi_vrgather_1xf64 (vini, idxo4p3, 16);

	const __epi_1xi64 vmask = __builtin_epi_vand_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 1, 0
	const __epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64 (vmask);
	__epi_1xf64 in2_3_etc = __builtin_epi_vfmerge_1xf64 (in2_10_18_26, in3_11_19_27, mmask, 16);
	__epi_1xf64 in3_2_etc = __builtin_epi_vfmerge_1xf64 (in3_11_19_27, in2_10_18_26, mmask, 16);
	__epi_1xf64 in6_7_etc = __builtin_epi_vfmerge_1xf64 (in6_14_22_30, in7_15_23_31, mmask, 16);
	__epi_1xf64 in7_6_etc = __builtin_epi_vfmerge_1xf64 (in7_15_23_31, in6_14_22_30, mmask, 16);

	__epi_1xf64 cplusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask, 16);
	const __epi_1xi64 vmask2 = __builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 0, 1, 1, 2, 2, ...
	const __epi_1xi64 vmask2b = __builtin_epi_vand_1xi64 (vmask2, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 0, 1, 1, 0, 0
	const __epi_1xi1 mmask2 = __builtin_epi_cast_1xi1_1xi64 (vmask2b);
	__epi_1xf64 cplusplusminusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask2, 16);

	__epi_1xf64 rsum1 = __builtin_epi_vfmacc_1xf64 (in0_8_16_24, cplusminus, in4_12_20_28, 16);
	__epi_1xf64 rsum2 = __builtin_epi_vfmacc_1xf64 (in2_3_etc, cplusminus, in6_7_etc, 16);
	rsum = __builtin_epi_vfmacc_1xf64 (rsum1, cplusplusminusminus, rsum2, 16);

	__epi_1xf64 isum1 = __builtin_epi_vfmacc_1xf64 (in1_9_17_25, cplusminus, in5_13_21_29, 16);
	__epi_1xf64 isum2 = __builtin_epi_vfmacc_1xf64 (in7_6_etc, cplusminus, in3_2_etc, 16);
	isum = __builtin_epi_vfmacc_1xf64 (isum1, cplusplusminusminus, isum2, 16);
      }

      // at this point we should have 4 DFT-4 done ... still need to do the twiddle factors, and 4 more DFT-4 ...
      // problem: horizontal stuff ... transpose ?
      // or use vsseg4e to do the transpose in memory ?
      double storage_r[64];
      double storage_i[64];
      __epi_1xf64 vm1r, vm1i;
      __epi_1xf64 vm2r, vm2i;
      __epi_1xf64 vm3r, vm3i;
      {
	fake_vsseg4e (storage_r, rsum, rsum, rsum, rsum, 16);
	rsum = __builtin_epi_vload_1xf64 (storage_r + 0, 16);
	vm1r = __builtin_epi_vload_1xf64 (storage_r + 16, 16);
	vm2r = __builtin_epi_vload_1xf64 (storage_r + 32, 16);
	vm3r = __builtin_epi_vload_1xf64 (storage_r + 48, 16);
      }
      {
	fake_vsseg4e (storage_i, isum, isum, isum, isum, 16);
	isum = __builtin_epi_vload_1xf64 (storage_i + 0, 16);
	vm1i = __builtin_epi_vload_1xf64 (storage_i + 16, 16);
	vm2i = __builtin_epi_vload_1xf64 (storage_i + 32, 16);
	vm3i = __builtin_epi_vload_1xf64 (storage_i + 48, 16);
      }


      __epi_1xf64 vmr, vmi;
      __epi_1xf64 vtr, vti;
      {
	static const R twid1[32] = {
	  1., -0.,
	  -0., -1.,
	  -1., +0.,
	  0., +1.,
	  KP923879532, -KP382683432,
	  -KP382683432, -KP923879532,
	  -KP923879532, +KP382683432,
	  KP382683432, +KP923879532,
	  KP707106781, -KP707106781,
	  -KP707106781, -KP707106781,
	  -KP707106781, +KP707106781,
	  KP707106781, +KP707106781,
	  KP382683432, -KP923879532,
	  -KP923879532, -KP382683432,
	  -KP382683432, +KP923879532,
	  KP923879532, +KP382683432
	};

	fake_vlseg2e (twid1, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm1i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm1r, vmi, 16);
	vm1r = __builtin_epi_vfmsac_1xf64 (vtr, vm1r, vmr, 16);
	vm1i = __builtin_epi_vfmacc_1xf64 (vti, vm1i, vmr, 16);

	static const R twid2[32] = {
	  1., 0.,
	  -1., 0.,
	  1., 0.,
	  -1., 0.,
	  KP707106781, -KP707106781,
	  -KP707106781, +KP707106781,
	  KP707106781, -KP707106781,
	  -KP707106781, +KP707106781,
	  0., -1.,
	  0., 1.,
	  0., -1.,
	  0., 1.,
	  -KP707106781, -KP707106781,
	  KP707106781, +KP707106781,
	  -KP707106781, -KP707106781,
	  KP707106781, +KP707106781,
	};

	fake_vlseg2e (twid2, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm2i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm2r, vmi, 16);
	vm2r = __builtin_epi_vfmsac_1xf64 (vtr, vm2r, vmr, 16);
	vm2i = __builtin_epi_vfmacc_1xf64 (vti, vm2i, vmr, 16);


	static const R twid3[32] = {
	  1., 0.,
	  0., 1.,
	  -1., 0.,
	  0., -1.,
	  KP382683432, -KP923879532,
	  KP923879532, +KP382683432,
	  -KP382683432, +KP923879532,
	  -KP923879532, -KP382683432,
	  -KP707106781, -KP707106781,
	  KP707106781, -KP707106781,
	  KP707106781, +KP707106781,
	  -KP707106781, +KP707106781,
	  -KP923879532, +KP382683432,
	  -KP382683432, -KP923879532,
	  KP923879532, -KP382683432,
	  KP382683432, +KP923879532,
	};

	fake_vlseg2e (twid3, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm3i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm3r, vmi, 16);
	vm3r = __builtin_epi_vfmsac_1xf64 (vtr, vm3r, vmr, 16);
	vm3i = __builtin_epi_vfmacc_1xf64 (vti, vm3i, vmr, 16);


	rsum = __builtin_epi_vfadd_1xf64 (rsum, vm1r, 16);
	vm2r = __builtin_epi_vfadd_1xf64 (vm2r, vm3r, 16);

	isum = __builtin_epi_vfadd_1xf64 (isum, vm1i, 16);
	vm2i = __builtin_epi_vfadd_1xf64 (vm2i, vm3i, 16);

	vinr = __builtin_epi_vfadd_1xf64 (rsum, vm2r, 16);
	vini = __builtin_epi_vfadd_1xf64 (isum, vm2i, 16);
      }

      unsigned long data[16] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };
      const __epi_1xi64 pattern = __builtin_epi_vload_1xi64 (data, 16);
      rsum = __builtin_epi_vrgather_1xf64 (vinr, pattern, 16);
      isum = __builtin_epi_vrgather_1xf64 (vini, pattern, 16);
      fake_vsseg2e (out, rsum, isum, 16);

#elif defined(RISCV_V_VARIANT10)
      // have a go at a radix-4 FFT
      // similar to 9, but with a different order in-register
#define DK(a,b) static const double a = b;
#define R double
      DK (KP382683432, +0.382683432365089771728459984030398866761344562);
      DK (KP923879532, +0.923879532511286756128183189396788286822416626);
      DK (KP707106781, +0.707106781186547524400844362104849039284835938);

      const __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
      const __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 rsum, isum;

      fake_vlseg2e (in, vinr, vini, 16);	// splitted input

      // first, 4 DFT-4 - only additions, fmacc are used to time by -1/1 only (could be done with e.g. XOR + ADD)
      // but needs a LOT of vrgather at this time
      // currently dense, fixme: transpose 4x4 ?
      //const __epi_1xi64 idxo4 =  __builtin_epi_vsrl_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(2, 16), 16); // 0, 0, 0, 0, 1, 1, 1, ...
      const __epi_1xi64 idxo4 = __builtin_epi_vand_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);	// 0, 1, 2, 3, 0, 1, ...
      __epi_1xf64 in0_8_16_24 = __builtin_epi_vrgather_1xf64 (vinr, idxo4, 16);	// wrong name, from when idxo4 == 0, 0, 0, 0, 1, 1, 1, ...
      __epi_1xf64 in1_9_17_25 = __builtin_epi_vrgather_1xf64 (vini, idxo4, 16);

      const __epi_1xi64 idxo4p2 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (8, 16), 16);
      __epi_1xf64 in4_12_20_28 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p2, 16);	// will need +1/-1
      __epi_1xf64 in5_13_21_29 = __builtin_epi_vrgather_1xf64 (vini, idxo4p2, 16);

      const __epi_1xi64 idxo4p1 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (4, 16), 16);
      __epi_1xf64 in2_10_18_26 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p1, 16);	// will need merging & +1/+1/-1/-1
      __epi_1xf64 in3_11_19_27 = __builtin_epi_vrgather_1xf64 (vini, idxo4p1, 16);

      const __epi_1xi64 idxo4p3 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (12, 16), 16);
      __epi_1xf64 in6_14_22_30 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p3, 16);	// will need merging & +1/-1/-1/+1
      __epi_1xf64 in7_15_23_31 = __builtin_epi_vrgather_1xf64 (vini, idxo4p3, 16);

      //const __epi_1xi64 vmask = __builtin_epi_vand_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 1, 0
      const __epi_1xi64 vmask = __builtin_epi_vand_1xi64 (__builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16), __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0...
      const __epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64 (vmask);
      __epi_1xf64 in2_3_etc = __builtin_epi_vfmerge_1xf64 (in2_10_18_26, in3_11_19_27, mmask, 16);
      __epi_1xf64 in3_2_etc = __builtin_epi_vfmerge_1xf64 (in3_11_19_27, in2_10_18_26, mmask, 16);
      __epi_1xf64 in6_7_etc = __builtin_epi_vfmerge_1xf64 (in6_14_22_30, in7_15_23_31, mmask, 16);
      __epi_1xf64 in7_6_etc = __builtin_epi_vfmerge_1xf64 (in7_15_23_31, in6_14_22_30, mmask, 16);

      __epi_1xf64 cplusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask, 16);
      //const __epi_1xi64 vmask2 = __builtin_epi_vsrl_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 0, 1, 1, 2, 2, ...
      //const __epi_1xi64 vmask2b =  __builtin_epi_vand_1xi64(vmask2, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 0, 1, 1, 0, 0
      const __epi_1xi64 vmask2b = __builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);	// 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ...
      const __epi_1xi1 mmask2 = __builtin_epi_cast_1xi1_1xi64 (vmask2b);
      __epi_1xf64 cplusplusminusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask2, 16);

      __epi_1xf64 rsum1 = __builtin_epi_vfmacc_1xf64 (in0_8_16_24, cplusminus, in4_12_20_28, 16);
      __epi_1xf64 rsum2 = __builtin_epi_vfmacc_1xf64 (in2_3_etc, cplusminus, in6_7_etc, 16);
      rsum = __builtin_epi_vfmacc_1xf64 (rsum1, cplusplusminusminus, rsum2, 16);

      __epi_1xf64 isum1 = __builtin_epi_vfmacc_1xf64 (in1_9_17_25, cplusminus, in5_13_21_29, 16);
      __epi_1xf64 isum2 = __builtin_epi_vfmacc_1xf64 (in7_6_etc, cplusminus, in3_2_etc, 16);
      isum = __builtin_epi_vfmacc_1xf64 (isum1, cplusplusminusminus, isum2, 16);

      // at this point we should have 4 DFT-4 done ... still need to do the twiddle factors, and 4 more DFT-4 ...
      // problem: horizontal stuff ... transpose ?
      // or use vsseg4e to do the transpose in memory ?
      __epi_1xf64 vm1r, vm1i;
      __epi_1xf64 vm2r, vm2i;
      __epi_1xf64 vm3r, vm3i;

/* 0, 4, 8, 12, 0, 4, 8, 12, ...
 * 1, 5, 9, 13, 1, 5, 6, 13, ...
 */
      {
	      const __epi_1xi64 idxo4t4 = __builtin_epi_vsll_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16); // 0,0,0,0,4,4,4,...
	const __epi_1xi64 idxo4t4p3 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);
	const __epi_1xi64 idxo4t4p2 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16);
	const __epi_1xi64 idxo4t4p1 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);
	vm3r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p3, 16);
	vm2r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p2, 16);
	vm1r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p1, 16);
	rsum = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4, 16);
	vm3i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p3, 16);
	vm2i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p2, 16);
	vm1i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p1, 16);
	isum = __builtin_epi_vrgather_1xf64 (isum, idxo4t4, 16);
      }

      __epi_1xf64 vmr, vmi;
      __epi_1xf64 vtr, vti;
      {
	static const R twid1[32] = {
	  1., 0.,
	  KP923879532, -KP382683432,
	  KP707106781, -KP707106781,
	  KP382683432, -KP923879532,
	  0., -1.,
	  -KP382683432, -KP923879532,
	  -KP707106781, -KP707106781,
	  -KP923879532, -KP382683432,
	  -1., 0.,
	  -KP923879532, KP382683432,
	  -KP707106781, KP707106781,
	  -KP382683432, KP923879532,
	  0., 1.,
	  KP382683432, KP923879532,
	  KP707106781, KP707106781,
	  KP923879532, KP382683432
	};

	fake_vlseg2e (twid1, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm1i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm1r, vmi, 16);
	vm1r = __builtin_epi_vfmsac_1xf64 (vtr, vm1r, vmr, 16);
	vm1i = __builtin_epi_vfmacc_1xf64 (vti, vm1i, vmr, 16);

	static const R twid2[32] = {
	  1., 0.,
	  KP707106781, -KP707106781,
	  0., -1.,
	  -KP707106781, -KP707106781,
	  -1., 0.,
	  -KP707106781, KP707106781,
	  -0., 1.,
	  KP707106781, KP707106781,
	  1., 0.,
	  KP707106781, -KP707106781,
	  0., -1.,
	  -KP707106781, -KP707106781,
	  -1., 0.,
	  -KP707106781, KP707106781,
	  0., 1.,
	  KP707106781, KP707106781,
	};

	fake_vlseg2e (twid2, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm2i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm2r, vmi, 16);
	vm2r = __builtin_epi_vfmsac_1xf64 (vtr, vm2r, vmr, 16);
	vm2i = __builtin_epi_vfmacc_1xf64 (vti, vm2i, vmr, 16);


	static const R twid3[32] = {
	  1., -0.,
	  KP382683432, -KP923879532,
	  -KP707106781, -KP707106781,
	  -KP923879532, KP382683432,
	  -0., 1.,
	  KP923879532, KP382683432,
	  KP707106781, -KP707106781,
	  -KP382683432, -KP923879532,
	  -1., -0.,
	  -KP382683432, KP923879532,
	  KP707106781, KP707106781,
	  KP923879532, -KP382683432,
	  0., -1.,
	  -KP923879532, -KP382683432,
	  -KP707106781, KP707106781,
	  KP382683432, KP923879532
	};

	fake_vlseg2e (twid3, vmr, vmi, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm3i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm3r, vmi, 16);
	vm3r = __builtin_epi_vfmsac_1xf64 (vtr, vm3r, vmr, 16);
	vm3i = __builtin_epi_vfmacc_1xf64 (vti, vm3i, vmr, 16);

	rsum = __builtin_epi_vfadd_1xf64 (rsum, vm1r, 16);
	vm2r = __builtin_epi_vfadd_1xf64 (vm2r, vm3r, 16);

	isum = __builtin_epi_vfadd_1xf64 (isum, vm1i, 16);
	vm2i = __builtin_epi_vfadd_1xf64 (vm2i, vm3i, 16);

	vinr = __builtin_epi_vfadd_1xf64 (rsum, vm2r, 16);
	vini = __builtin_epi_vfadd_1xf64 (isum, vm2i, 16);
      }
      rsum = vinr;
      isum = vini;
      fake_vsseg2e (out, rsum, isum, 16);
#elif defined(RISCV_V_VARIANT11)
      // have a go at a radix-4 FFT
      // similar to 10, but with non-interleaved twiddle
#define DK(a,b) static const double a = b;
#define R double
      DK (KP382683432, +0.382683432365089771728459984030398866761344562);
      DK (KP923879532, +0.923879532511286756128183189396788286822416626);
      DK (KP707106781, +0.707106781186547524400844362104849039284835938);

      const __epi_1xi64 idx = __builtin_epi_vid_1xi64 (16);	// 0, 1, 2, ...
      const __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2 * sizeof (double), 16), 16);
      __epi_1xf64 vinr, vini;
      __epi_1xf64 rsum, isum;

      fake_vlseg2e (in, vinr, vini, 16);	// splitted input

      // first, 4 DFT-4 - only additions, fmacc are used to time by -1/1 only (could be done with e.g. XOR + ADD)
      // but needs a LOT of vrgather at this time
      // currently dense, fixme: transpose 4x4 ?
      //const __epi_1xi64 idxo4 =  __builtin_epi_vsrl_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(2, 16), 16); // 0, 0, 0, 0, 1, 1, 1, ...
      const __epi_1xi64 idxo4 = __builtin_epi_vand_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);	// 0, 1, 2, 3, 0, 1, ...
      __epi_1xf64 in0_8_16_24 = __builtin_epi_vrgather_1xf64 (vinr, idxo4, 16);	// wrong name, from when idxo4 == 0, 0, 0, 0, 1, 1, 1, ...
      __epi_1xf64 in1_9_17_25 = __builtin_epi_vrgather_1xf64 (vini, idxo4, 16);

      const __epi_1xi64 idxo4p2 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (8, 16), 16);
      __epi_1xf64 in4_12_20_28 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p2, 16);	// will need +1/-1
      __epi_1xf64 in5_13_21_29 = __builtin_epi_vrgather_1xf64 (vini, idxo4p2, 16);

      const __epi_1xi64 idxo4p1 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (4, 16), 16);
      __epi_1xf64 in2_10_18_26 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p1, 16);	// will need merging & +1/+1/-1/-1
      __epi_1xf64 in3_11_19_27 = __builtin_epi_vrgather_1xf64 (vini, idxo4p1, 16);

      const __epi_1xi64 idxo4p3 = __builtin_epi_vadd_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (12, 16), 16);
      __epi_1xf64 in6_14_22_30 = __builtin_epi_vrgather_1xf64 (vinr, idxo4p3, 16);	// will need merging & +1/-1/-1/+1
      __epi_1xf64 in7_15_23_31 = __builtin_epi_vrgather_1xf64 (vini, idxo4p3, 16);

      //const __epi_1xi64 vmask = __builtin_epi_vand_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 1, 0
      const __epi_1xi64 vmask = __builtin_epi_vand_1xi64 (__builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16), __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);	// 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0...
      const __epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64 (vmask);
      __epi_1xf64 in2_3_etc = __builtin_epi_vfmerge_1xf64 (in2_10_18_26, in3_11_19_27, mmask, 16);
      __epi_1xf64 in3_2_etc = __builtin_epi_vfmerge_1xf64 (in3_11_19_27, in2_10_18_26, mmask, 16);
      __epi_1xf64 in6_7_etc = __builtin_epi_vfmerge_1xf64 (in6_14_22_30, in7_15_23_31, mmask, 16);
      __epi_1xf64 in7_6_etc = __builtin_epi_vfmerge_1xf64 (in7_15_23_31, in6_14_22_30, mmask, 16);

      __epi_1xf64 cplusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask, 16);
      //const __epi_1xi64 vmask2 = __builtin_epi_vsrl_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 0, 1, 1, 2, 2, ...
      //const __epi_1xi64 vmask2b =  __builtin_epi_vand_1xi64(vmask2, __builtin_epi_vmv_v_x_1xi64(1, 16), 16); // 0, 0, 1, 1, 0, 0
      const __epi_1xi64 vmask2b = __builtin_epi_vsrl_1xi64 (idx, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);	// 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ...
      const __epi_1xi1 mmask2 = __builtin_epi_cast_1xi1_1xi64 (vmask2b);
      __epi_1xf64 cplusplusminusminus = __builtin_epi_vfmerge_1xf64 (__builtin_epi_vfmv_v_f_1xf64 (1., 16), __builtin_epi_vfmv_v_f_1xf64 (-1., 16), mmask2, 16);

      __epi_1xf64 rsum1 = __builtin_epi_vfmacc_1xf64 (in0_8_16_24, cplusminus, in4_12_20_28, 16);
      __epi_1xf64 rsum2 = __builtin_epi_vfmacc_1xf64 (in2_3_etc, cplusminus, in6_7_etc, 16);
      rsum = __builtin_epi_vfmacc_1xf64 (rsum1, cplusplusminusminus, rsum2, 16);

      __epi_1xf64 isum1 = __builtin_epi_vfmacc_1xf64 (in1_9_17_25, cplusminus, in5_13_21_29, 16);
      __epi_1xf64 isum2 = __builtin_epi_vfmacc_1xf64 (in7_6_etc, cplusminus, in3_2_etc, 16);
      isum = __builtin_epi_vfmacc_1xf64 (isum1, cplusplusminusminus, isum2, 16);

      // at this point we should have 4 DFT-4 done ... still need to do the twiddle factors, and 4 more DFT-4 ...
      // problem: horizontal stuff ... transpose ?
      // or use vsseg4e to do the transpose in memory ?
      __epi_1xf64 vm1r, vm1i;
      __epi_1xf64 vm2r, vm2i;
      __epi_1xf64 vm3r, vm3i;

/* 0, 4, 8, 12, 0, 4, 8, 12, ...
 * 1, 5, 9, 13, 1, 5, 6, 13, ...
 */
      {
	      const __epi_1xi64 idxo4t4 = __builtin_epi_vsll_1xi64 (idxo4, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16); // 0,0,0,0,4,4,4,...
	const __epi_1xi64 idxo4t4p3 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (3, 16), 16);
	const __epi_1xi64 idxo4t4p2 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (2, 16), 16);
	const __epi_1xi64 idxo4t4p1 = __builtin_epi_vadd_1xi64 (idxo4t4, __builtin_epi_vmv_v_x_1xi64 (1, 16), 16);
	vm3r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p3, 16);
	vm2r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p2, 16);
	vm1r = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4p1, 16);
	rsum = __builtin_epi_vrgather_1xf64 (rsum, idxo4t4, 16);
	vm3i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p3, 16);
	vm2i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p2, 16);
	vm1i = __builtin_epi_vrgather_1xf64 (isum, idxo4t4p1, 16);
	isum = __builtin_epi_vrgather_1xf64 (isum, idxo4t4, 16);
      }

      __epi_1xf64 vmr, vmi;
      __epi_1xf64 vtr, vti;
      {
	      static const R twid1[32] = {
		      1.,
		      KP923879532,
		      KP707106781,
		      KP382683432,
		      0.,
		      -KP382683432,
		      -KP707106781,
		      -KP923879532,
		      -1.,
		      -KP923879532,
		      -KP707106781,
		      -KP382683432,
		      0.,
		      KP382683432,
		      KP707106781,
		      KP923879532,

		      0.,
		      -KP382683432,
		      -KP707106781,
		      -KP923879532,
		      -1.,
		      -KP923879532,
		      -KP707106781,
		      -KP382683432,
		      0.,
		      KP382683432,
		      KP707106781,
		      KP923879532,
		      1.,
		      KP923879532,
		      KP707106781,
		      KP382683432
	      };

	vmr =  __builtin_epi_vload_1xf64(twid1, 16);
	vmi =  __builtin_epi_vload_1xf64(twid1+16, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm1i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm1r, vmi, 16);
	vm1r = __builtin_epi_vfmsac_1xf64 (vtr, vm1r, vmr, 16);
	vm1i = __builtin_epi_vfmacc_1xf64 (vti, vm1i, vmr, 16);

	static const R twid2[32] = {1.,
				    KP707106781,
				    0.,
				    -KP707106781,
				    -1.,
				    -KP707106781,
				    0.,
				    KP707106781,
				    1.,
				    KP707106781,
				    0.,
				    -KP707106781,
				    -1.,
				    -KP707106781,
				    0.,
				    KP707106781,
				    0.,
				    -KP707106781,
				    -1.,
				    -KP707106781,
				    0.,
				    KP707106781,
				    1.,
				    KP707106781,
				    0.,
				    -KP707106781,
				    -1.,
				    -KP707106781,
				    0.,
				    KP707106781,
				    1.,
				    KP707106781
	};

	vmr =  __builtin_epi_vload_1xf64(twid2, 16);
	vmi =  __builtin_epi_vload_1xf64(twid2+16, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm2i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm2r, vmi, 16);
	vm2r = __builtin_epi_vfmsac_1xf64 (vtr, vm2r, vmr, 16);
	vm2i = __builtin_epi_vfmacc_1xf64 (vti, vm2i, vmr, 16);


	static const R twid3[32] = {1.,
				    KP382683432,
				    -KP707106781,
				    -KP923879532,
				    0.,
				    KP923879532,
				    KP707106781,
				    -KP382683432,
				    -1.,
				    -KP382683432,
				    KP707106781,
				    KP923879532,
				    0.,
				    -KP923879532,
				    -KP707106781,
				    KP382683432,
				    0.,
				    -KP923879532,
				    -KP707106781,
				    KP382683432,
				    1.,
				    KP382683432,
				    -KP707106781,
				    -KP923879532,
				    0.,
				    KP923879532,
				    KP707106781,
				    -KP382683432,
				    -1.,
				    -KP382683432,
				    KP707106781,
				    KP923879532
	};

	vmr =  __builtin_epi_vload_1xf64(twid3, 16);
	vmi =  __builtin_epi_vload_1xf64(twid3+16, 16);
	vtr = __builtin_epi_vfmul_1xf64 (vm3i, vmi, 16);
	vti = __builtin_epi_vfmul_1xf64 (vm3r, vmi, 16);
	vm3r = __builtin_epi_vfmsac_1xf64 (vtr, vm3r, vmr, 16);
	vm3i = __builtin_epi_vfmacc_1xf64 (vti, vm3i, vmr, 16);

	rsum = __builtin_epi_vfadd_1xf64 (rsum, vm1r, 16);
	vm2r = __builtin_epi_vfadd_1xf64 (vm2r, vm3r, 16);

	isum = __builtin_epi_vfadd_1xf64 (isum, vm1i, 16);
	vm2i = __builtin_epi_vfadd_1xf64 (vm2i, vm3i, 16);

	vinr = __builtin_epi_vfadd_1xf64 (rsum, vm2r, 16);
	vini = __builtin_epi_vfadd_1xf64 (isum, vm2i, 16);
      }
      rsum = vinr;
      isum = vini;
      fake_vsseg2e (out, rsum, isum, 16);

#endif // variants
#endif // riscv && !BLAS

    }				// loop on #transform 'v'
}
