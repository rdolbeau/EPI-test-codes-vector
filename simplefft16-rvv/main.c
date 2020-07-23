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

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "n1_16.h"
#include "n1_16_simplified.h"
#include "test_fft16.h"
#include "test_fft16_zgemv.h"

const char *const black  = "\033[0;39;30m";
const char *const red    = "\033[0;39;31m";
const char *const green  = "\033[0;39;32m";
const char *const orange = "\033[0;39;33m";
const char *const blue   = "\033[0;39;34m";
const char *const purple = "\033[0;39;35m";
const char *const cyan   = "\033[0;39;36m";
const char *const grey   = "\033[0;39;37m";

const char *const normal = "\033[0m";


#if defined(__x86_64__)
static inline uint64_t
getitc_(void) {
        uint32_t lo, hi;
        /*
         * cpuid will serialize the following rdtsc with respect to all other
         * instructions the processor may be handling.
         */
        __asm__ __volatile__ (
                              "xorl %%eax, %%eax\n"
                              "cpuid\n"
                              "rdtsc\n"
                              : "=a" (lo), "=d" (hi)
                              :
                              : "%ebx", "%ecx");
        return (uint64_t)hi << 32 | lo;
}
#endif

#if defined(__aarch64__)
static inline uint64_t
getitc_(void)
{
        uint64_t r = 0;
        //  asm volatile("mrs %0, PMCCNTR_EL0" : "=r"(r));
        asm volatile("mrs %0,  CNTVCT_EL0" : "=r" (r));
        return r;
}
#endif

#if defined(__riscv) // rv64, really
static inline uint64_t
getitc_(void)
{
        uint64_t r = 0;
        asm volatile("rdcycle %0" : "=r" (r));
        return r;
}
#endif

#define TIME(X) \
do { \
uint64_t t0, t1;\
t0 = getitc_();\
X;\
t1 = getitc_();\
printf("Time for % 60s is % 8llu cycles\n", #X, (unsigned long long)(t1-t0));\
} while (0)

#define ITER 4

#define MAXTESTS 40

int main(int argc, char **argv) {
	double *in = calloc(sizeof(double), 64);
	double **out = calloc(sizeof(double*), MAXTESTS);
	int i, j, k, idx;

	srand48(0xDEADBEEF); // fixed randomness
	for (i = 0 ; i < MAXTESTS ; i++)
		out[i] = calloc(sizeof(double), 64);

	for (i = 0 ; i < 64 ; i++) {
		in[i] = drand48();
		//in[i] = 1. + ((double)i)/1000000.;
	}
	
	for (k = 0; k < ITER ; k++) {
	idx = 0;
#if 1
	TIME(n1_16(in, in+1, out[idx], out[idx]+1, 2, 2, 2, 32, 32)); idx++;
	TIME(n1_16_simplified(in, in+1, out[idx], out[idx]+1, 2, 2, 2, 32, 32)); idx++;
#endif
//	TIME(generated_double_fft16(in, out[idx])); idx++;
#if defined (BLAS)
	TIME(gemv_double_fft16_blas(in, out[idx], 2, 32, 32)); idx++;
#endif
#if defined(__riscv)
	unsigned long int vc = __builtin_epi_vsetvl(16, __epi_e64, __epi_m1);
	if (vc < 16) { fprintf(stderr, "vector too short for register-based FFT16\n"); exit(-1); };

	/* emulated vlseg2/vsseg2 */
	TIME(gemv_double_fft16_1(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_2(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_2b(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_3(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_4(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_5(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_6(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_7(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_8(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_9(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_10(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_11(in, out[idx], 2, 32, 32)); idx++;

	/* true vlseg2/vsseg2 */
	TIME(gemv_double_fft16_1T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_2T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_2bT(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_3T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_4T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_5T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_6T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_7T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_8T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_9T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_10T(in, out[idx], 2, 32, 32)); idx++;
	TIME(gemv_double_fft16_11T(in, out[idx], 2, 32, 32)); idx++;
#endif
	}	

	for (i = 0 ; i < 64 ; i++) {
		printf("% 3d [%s]:", i, (i%2?"imag":"real"));
		for (j = 0 ; j<idx; j++) {
			const char *color = normal;
			if (fabs(out[j][i] - out[0][i]) > (fmax(fabs(out[j][i]), fabs(out[0][i]))/(1000000000.)))
				color = red;
			printf("\t%s% 10.8lf%s", color, out[j][i], normal);
		}
		printf("\n");
		if (i % 32 == 31) printf("\n");
	}

	return 0;
}

