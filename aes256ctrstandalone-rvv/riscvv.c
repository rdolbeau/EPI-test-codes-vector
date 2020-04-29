/*
  riscvv.c
  AES-CTR
  Romain Dolbeau
  Public Domain
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_stream.h"
#include "stdaes-common.h"

#include "vi64.h"

#define ALIGN16  __attribute__((aligned(16)))
#define ALIGN32  __attribute__((aligned(32)))
#define ALIGN64  __attribute__((aligned(64)))
#define _bswap64(a) __builtin_bswap64(a)
#define _bswap(a) __builtin_bswap(a)

//#define REGISTER_SBOX

#if 0
static inline void printiv32(const __epi_2xi32 v, const unsigned long int vl) {
        unsigned int data[vl];
        __builtin_epi_vstore_2xi32(data, v, vl);
        for (int i = 0 ; i < vl; i++) {
		printf("0x%08x  ", data[i]);
        }
        printf("\n");
}
static inline void printiv64(const __epi_1xi64 v, const unsigned long int vl) {
        unsigned long long int data[vl];
        __builtin_epi_vstore_1xi64(data, v, vl);
        for (int i = 0 ; i < vl; i++) {
                printf("0x%016llx  ", data[i]);
        }
        printf("\n");
}

#else
#define printiv32(v, vl) \
	do {\
		unsigned int data[vl]; \
		__builtin_epi_vstore_2xi32((int*)data, (const __epi_2xi32)v, vl); \
		printf("%s: ", #v);\
		for (int i = 0 ; i < vl; i++) {\
			printf("0x%08x  ", data[i]);\
		}\
	printf("\n");\
	} while (0)


#define printiv64(v, vl) \
        do {\
                unsigned long long int data[vl]; \
                __builtin_epi_vstore_1xi64((long long int*)data, (const __epi_1xi64)v, vl); \
                printf("%s: ", #v);\
                for (int i = 0 ; i < vl; i++) {\
                        printf("0x%016llx  ", data[i]);\
                }\
        printf("\n");\
        } while (0)
#endif

static inline __epi_2xi32 __builtin_epi_vrotr_2xi32(const __epi_2xi32 a, const __epi_2xi32 b, const unsigned long int vc) {
        const __epi_2xi32 asb = __builtin_epi_vsrl_2xi32(a, b, vc);
        const __epi_2xi32 as32mb = __builtin_epi_vsll_2xi32(a, __builtin_epi_vsub_2xi32(__builtin_epi_vmv_v_x_2xi32(32, vc), b, vc), vc);
        return __builtin_epi_vor_2xi32(asb, as32mb, vc);
}

static inline __epi_2xi32 __builtin_epi_brev64_2xi32(const __epi_2xi32 a, const unsigned long int vc32) {
	const unsigned long int vc = vc32/2;
	const __epi_1xi64 a64 = (const __epi_1xi64)a;
	const __epi_1xi64 c8 = __builtin_epi_vmv_v_x_1xi64(8, vc);
	const __epi_1xi64 c16 = __builtin_epi_vmv_v_x_1xi64(16, vc);

	const __epi_1xi64 a64m1a = __builtin_epi_vmv_v_x_1xi64(0x00FF00FF00FF00FFull, vc);
	const __epi_1xi64 a64m1b = __builtin_epi_vmv_v_x_1xi64(0xFF00FF00FF00FF00ull, vc);
	const __epi_1xi64 a64m2a = __builtin_epi_vmv_v_x_1xi64(0x0000FFFF0000FFFFull, vc);
	const __epi_1xi64 a64m2b = __builtin_epi_vmv_v_x_1xi64(0xFFFF0000FFFF0000ull, vc);
	const __epi_1xi64 a64m3a = __builtin_epi_vmv_v_x_1xi64(0x00000000FFFFFFFFull, vc);
	const __epi_1xi64 a64m3b = __builtin_epi_vmv_v_x_1xi64(0xFFFFFFFF00000000ull, vc);

	const __epi_1xi64 a64_1a = __builtin_epi_vand_1xi64(a64, a64m1a, vc);
	const __epi_1xi64 a64_1b = __builtin_epi_vand_1xi64(a64, a64m1b, vc);
	const __epi_1xi64 a64l1a = __builtin_epi_vsll_1xi64(a64_1a, c8, vc);
	const __epi_1xi64 a64r1b = __builtin_epi_vsrl_1xi64(a64_1b, c8, vc);
	const __epi_1xi64 a64_1  = __builtin_epi_vor_1xi64(a64l1a, a64r1b, vc);

	const __epi_1xi64 a64_2a = __builtin_epi_vand_1xi64(a64_1, a64m2a, vc);
	const __epi_1xi64 a64_2b = __builtin_epi_vand_1xi64(a64_1, a64m2b, vc); // next is twice 8 as 16 is currently buggy
	//const __epi_1xi64 a64l2a = __builtin_epi_vsll_1xi64(__builtin_epi_vsll_1xi64(a64_2a, c8, vc), c8, vc);
	//const __epi_1xi64 a64r2b = __builtin_epi_vsrl_1xi64(__builtin_epi_vsrl_1xi64(a64_2b, c8, vc), c8, vc);
	const __epi_1xi64 a64l2a = __builtin_epi_vsll_1xi64(a64_2a, c16, vc);
	const __epi_1xi64 a64r2b = __builtin_epi_vsrl_1xi64(a64_2b, c16, vc);
	const __epi_1xi64 a64_2  = __builtin_epi_vor_1xi64(a64l2a, a64r2b, vc);

	const __epi_1xi64 a64_3a = __builtin_epi_vand_1xi64(a64_2, a64m3a, vc);
	const __epi_1xi64 a64_3b = __builtin_epi_vand_1xi64(a64_2, a64m3b, vc);
	const __epi_1xi64 a64l3a = __builtin_epi_vsll_1xi64(a64_3a, __builtin_epi_vmv_v_x_1xi64(32, vc), vc);
	const __epi_1xi64 a64r3b = __builtin_epi_vsrl_1xi64(a64_3b, __builtin_epi_vmv_v_x_1xi64(32, vc), vc);
	const __epi_1xi64 a64_3  = __builtin_epi_vor_1xi64(a64l3a, a64r3b, vc);

	return (__epi_2xi32)a64_3;
}

static inline __epi_2xi32 __builtin_epi_vrotlv4_2xi32(const __epi_2xi32 a, const unsigned long int vc) {
//        static const unsigned int lpat[4] = { 1, 2, 3, 0};
//        static const unsigned int rpat[4] = { 3, 0, 1, 2};
	static const unsigned int lpat[512] = { 1,2,3,0,5,6,7,4,9,10,11,8,13,14,15,12,17,18,19,16,21,22,23,20,25,26,27,24,29,30,31,28,33,34,35,32,37,38,39,36,41,42,43,40,45,46,47,44,49,50,51,48,53,54,55,52,57,58,59,56,61,62,63,60,65,66,67,64,69,70,71,68,73,74,75,72,77,78,79,76,81,82,83,80,85,86,87,84,89,90,91,88,93,94,95,92,97,98,99,96,101,102,103,100,105,106,107,104,109,110,111,108,113,114,115,112,117,118,119,116,121,122,123,120,125,126,127,124,129,130,131,128,133,134,135,132,137,138,139,136,141,142,143,140,145,146,147,144,149,150,151,148,153,154,155,152,157,158,159,156,161,162,163,160,165,166,167,164,169,170,171,168,173,174,175,172,177,178,179,176,181,182,183,180,185,186,187,184,189,190,191,188,193,194,195,192,197,198,199,196,201,202,203,200,205,206,207,204,209,210,211,208,213,214,215,212,217,218,219,216,221,222,223,220,225,226,227,224,229,230,231,228,233,234,235,232,237,238,239,236,241,242,243,240,245,246,247,244,249,250,251,248,253,254,255,252,257,258,259,256,261,262,263,260,265,266,267,264,269,270,271,268,273,274,275,272,277,278,279,276,281,282,283,280,285,286,287,284,289,290,291,288,293,294,295,292,297,298,299,296,301,302,303,300,305,306,307,304,309,310,311,308,313,314,315,312,317,318,319,316,321,322,323,320,325,326,327,324,329,330,331,328,333,334,335,332,337,338,339,336,341,342,343,340,345,346,347,344,349,350,351,348,353,354,355,352,357,358,359,356,361,362,363,360,365,366,367,364,369,370,371,368,373,374,375,372,377,378,379,376,381,382,383,380,385,386,387,384,389,390,391,388,393,394,395,392,397,398,399,396,401,402,403,400,405,406,407,404,409,410,411,408,413,414,415,412,417,418,419,416,421,422,423,420,425,426,427,424,429,430,431,428,433,434,435,432,437,438,439,436,441,442,443,440,445,446,447,444,449,450,451,448,453,454,455,452,457,458,459,456,461,462,463,460,465,466,467,464,469,470,471,468,473,474,475,472,477,478,479,476,481,482,483,480,485,486,487,484,489,490,491,488,493,494,495,492,497,498,499,496,501,502,503,500,505,506,507,504,509,510,511,508 };
        const __epi_2xi32 vlpat = __builtin_epi_vload_2xi32((const int*)lpat, vc);
        return __builtin_epi_vrgather_2xi32(a, vlpat, vc);
}

#if defined(REGISTER_SBOX)
#define do_gather(t,a,x) __builtin_epi_vrgather_2xi32(v##t, a, x)
#else
#define do_gather(t,a,x) __builtin_epi_vload_indexed_2xi32(t, a, x)
#endif

static inline void aes256_4ft_encrypt1(unsigned int *output, const unsigned int *input, const unsigned int *aes_edrk)
{
  __epi_2xi32 vinput;
  __epi_2xi32 vaes_edrk;
  __epi_2xi32 X, Y;
  __epi_2xi32 v0xFF, v0x8, v0x10, v0x18;
  __epi_2xi32 X8, Y8;
  __epi_2xi32 vFT0, vFT1, vFT2, vFT3;

  unsigned int i = 0, j = 0;
  const unsigned int l_aes_nr = 14; // AES256
  /* let's assume we have at least 8192 bits worth of vector ... I'm an optimist :-) */
  unsigned int X0, X1, X2, X3, Y0, Y1, Y2, Y3;

  vinput    = __builtin_epi_vload_2xi32((const int*)input   , 4);
  vaes_edrk = __builtin_epi_vload_2xi32((const int*)aes_edrk, 4);

  X = __builtin_epi_vxor_2xi32(vinput, vaes_edrk, 4);

  v0xFF = __builtin_epi_vmv_v_x_2xi32(0xFF, 4);
  v0x8 = __builtin_epi_vmv_v_x_2xi32(0x8, 4);
  v0x10 = __builtin_epi_vmv_v_x_2xi32(0x10, 4);
  v0x18 = __builtin_epi_vmv_v_x_2xi32(0x18, 4);

#if defined(REGISTER_SBOX)
  vFT0 = __builtin_epi_vload_2xi32((const int*)FT0, 256);
  vFT1 = __builtin_epi_vload_2xi32((const int*)FT1, 256);
  vFT2 = __builtin_epi_vload_2xi32((const int*)FT2, 256);
  vFT3 = __builtin_epi_vload_2xi32((const int*)FT3, 256);
#endif

  for (i = 4 ; i < (l_aes_nr<<2) ; i+= 4) {

    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y = do_gather(FT0, X8, 4);
#if defined(onetable)
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x18, 4); /* emulated */
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x10, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x8, 4); /* emulated */
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
#else
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT1, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT2, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = do_gather(FT3, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
#endif

    vaes_edrk = __builtin_epi_vload_2xi32((const int*)(aes_edrk + i), 4);

    X = __builtin_epi_vxor_2xi32(Y, vaes_edrk, 4);
  }

  /* try with non-vector last round */
  __builtin_epi_vstore_2xi32((int*)output, X, 4);
  X0 = output[0];
  X1 = output[1];
  X2 = output[2];
  X3 = output[3];

    Y0 = (aes_edrk[i]) ^
    ( FSb[( X0       ) &0xFF ]       ) ^
    ( FSb[( X1 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X2 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X3 >> 24 ) &0xFF ] << 24 );

  Y1 = (aes_edrk[1+i]) ^
    ( FSb[( X1       ) &0xFF ]       ) ^
    ( FSb[( X2 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X3 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X0 >> 24 ) &0xFF ] << 24 );

  Y2 = (aes_edrk[2+i]) ^
    ( FSb[( X2       ) &0xFF ]       ) ^
    ( FSb[( X3 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X0 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X1 >> 24 ) &0xFF ] << 24 );

  Y3 = (aes_edrk[3+i]) ^
    ( FSb[( X3       ) &0xFF ]       ) ^
    ( FSb[( X0 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X1 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X2 >> 24 ) &0xFF ] << 24 );

  output[0] = (Y0);
  output[1] = (Y1);
  output[2] = (Y2);
  output[3] = (Y3);
}

static inline __epi_2xi32 __builtin_epi_broadcast4xi32_2xi32(__epi_2xi32 a, unsigned long long vc) {
	const __epi_2xi32 vid = __builtin_epi_vid_2xi32(vc);
	const __epi_2xi32 idx = __builtin_epi_vand_2xi32(vid, __builtin_epi_vmv_v_x_2xi32(0x3, vc), vc);
	return __builtin_epi_vrgather_2xi32(a, idx, vc);
}

static inline void aes256_4ft_encryptX(unsigned int *output, unsigned long long int *input, const unsigned int *aes_edrk, const int block)
{
  __epi_2xi32 vinput;
  __epi_2xi32 vaes_edrk;
  __epi_2xi32 X, Y;
  __epi_2xi32 v0xFF, v0x8, v0x10, v0x18;
  __epi_2xi32 X8, Y8;
  __epi_2xi32 vFT0, vFT1, vFT2, vFT3;

  unsigned int i = 0, j = 0;
  const unsigned int l_aes_nr = 14; // AES256
  /* let's assume we have at least 8192 bits worth of vector ... I'm an optimist :-) */
  unsigned int X0, X1, X2, X3, Y0, Y1, Y2, Y3;

//  vinput = __builtin_epi_vload_2xi32((const int*)input, 4*block);
//  vinput = __builtin_epi_brev64_2xi32(vinput, 4*block);
  long long nl = *(long long*)&input[1];

  if ((nl < 0) && (nl + block) >= 0) {
        //wrap
        long long int wrapnumber = (block - (nl+block));
        __epi_1xi64 vinc = __builtin_epi_vid_1xi64(2*block);
        __epi_1xi64 vzero = __builtin_epi_vmv_v_x_1xi64(0,2*block);
        __epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1,2*block);
	__epi_1xi1 carries = __builtin_epi_vmsle_1xi64(__builtin_epi_vmv_v_x_1xi64(wrapnumber,2*block),vinc,2*block);
	__epi_1xi64 vcarries = __builtin_epi_vmerge_1xi64(vzero, vone, carries, 2*block);
	vinc = __builtin_epi_zip1_1xi64(vcarries, vinc, 2*block); // emulated
	vinput = __builtin_epi_vload_2xi32((const int*)input, 4);
	vinput = __builtin_epi_broadcast4xi32_2xi32(vinput, 4*block); // emulated
	vinput = (__epi_2xi32)__builtin_epi_vadd_1xi64((__epi_1xi64)vinput, vinc, 2*block);
	vinput = __builtin_epi_brev64_2xi32(vinput, 4*block); // emulated
        input[1] += block;
        input[0] ++;
  } else {
        //no wrap, maybe unecessary, using only wrap code
	__epi_1xi64 vinc = __builtin_epi_vid_1xi64(2*block);
	__epi_1xi64 vzero = __builtin_epi_vmv_v_x_1xi64(0,2*block);
	vinc = __builtin_epi_zip1_1xi64(vzero, vinc, 2*block); // emulated
	vinput = __builtin_epi_vload_2xi32((const int*)input, 4);
	vinput = __builtin_epi_broadcast4xi32_2xi32(vinput, 4*block); // emulated
	vinput = (__epi_2xi32)__builtin_epi_vadd_1xi64((__epi_1xi64)vinput, vinc, 2*block);
	vinput = __builtin_epi_brev64_2xi32(vinput, 4*block); // emulated
        input[1] += block;
  }

  vaes_edrk = __builtin_epi_vload_2xi32((const int*)aes_edrk, 4);
  vaes_edrk = __builtin_epi_broadcast4xi32_2xi32(vaes_edrk, 4*block); // emulated

  X = __builtin_epi_vxor_2xi32(vinput, vaes_edrk, 4*block);

  v0xFF = __builtin_epi_vmv_v_x_2xi32(0xFF, 4*block);
  v0x8 = __builtin_epi_vmv_v_x_2xi32(0x8, 4*block);
  v0x10 = __builtin_epi_vmv_v_x_2xi32(0x10, 4*block);
  v0x18 = __builtin_epi_vmv_v_x_2xi32(0x18, 4*block);

#if defined(REGISTER_SBOX)
  vFT0 = __builtin_epi_vload_2xi32((const int*)FT0, 256);
  vFT1 = __builtin_epi_vload_2xi32((const int*)FT1, 256);
  vFT2 = __builtin_epi_vload_2xi32((const int*)FT2, 256);
  vFT3 = __builtin_epi_vload_2xi32((const int*)FT3, 256);
#endif

  for (i = 4 ; i < (l_aes_nr<<2) ; i+= 4) {

    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4*block);
    Y = do_gather(FT0, X8, 4*block);

    X = __builtin_epi_vrotlv4_2xi32(X, 4*block); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4*block);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4*block);
    Y8 = do_gather(FT1, X8, 4*block);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4*block);

    X = __builtin_epi_vrotlv4_2xi32(X, 4*block); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4*block);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4*block);
    Y8 = do_gather(FT2, X8, 4*block);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4*block);

    X = __builtin_epi_vrotlv4_2xi32(X, 4*block); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4*block);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4*block);
    Y8 = do_gather(FT3, X8, 4*block);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4*block);

    vaes_edrk = __builtin_epi_vload_2xi32((const int*)(aes_edrk + i), 4);
    vaes_edrk = __builtin_epi_broadcast4xi32_2xi32(vaes_edrk, 4*block); /* emulated */

    X = __builtin_epi_vxor_2xi32(Y, vaes_edrk, 4*block);
  }

  /* try with non-vector last round */
  __builtin_epi_vstore_2xi32((int*)output, X, 4*block);
  for (j = 0 ; j < 4*block ; j+=4) {
  X0 = output[j+0];
  X1 = output[j+1];
  X2 = output[j+2];
  X3 = output[j+3];

    Y0 = (aes_edrk[i]) ^
    ( FSb[( X0       ) &0xFF ]       ) ^
    ( FSb[( X1 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X2 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X3 >> 24 ) &0xFF ] << 24 );

  Y1 = (aes_edrk[1+i]) ^
    ( FSb[( X1       ) &0xFF ]       ) ^
    ( FSb[( X2 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X3 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X0 >> 24 ) &0xFF ] << 24 );

  Y2 = (aes_edrk[2+i]) ^
    ( FSb[( X2       ) &0xFF ]       ) ^
    ( FSb[( X3 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X0 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X1 >> 24 ) &0xFF ] << 24 );

  Y3 = (aes_edrk[3+i]) ^
    ( FSb[( X3       ) &0xFF ]       ) ^
    ( FSb[( X0 >>  8 ) &0xFF ] <<  8 ) ^
    ( FSb[( X1 >> 16 ) &0xFF ] << 16 ) ^
    ( FSb[( X2 >> 24 ) &0xFF ] << 24 );

  output[j+0] = (Y0);
  output[j+1] = (Y1);
  output[j+2] = (Y2);
  output[j+3] = (Y3);
  }
}

int crypto_stream(
unsigned char *out,
unsigned long long outlen,
const unsigned char *n,
const unsigned char *k
)
{
  unsigned long int vc = __builtin_epi_vsetvl(512, __epi_e32, __epi_m1); // vc should be 256 or more, otherwise vrgather won't work for FT
  ALIGN16 unsigned long long n2[2];
  unsigned long long i, j, k2;
  unsigned int rkeys[60];
  aes256_setkey_encrypt((const unsigned int*)k,rkeys);
  vc &= ~0x03ul;

#if defined(REGISTER_SBOX)
  if (vc < 256) { fprintf(stderr, "vector too short for register-based s-boxes\n"); exit(-1); };
#endif

  /* n2 is in byte-reversed (i.e., native little endian)
     order to make increment/testing easier */
  n2[1] = _bswap64((*(unsigned long long*)&n[8]));
  n2[0] = _bswap64((*(unsigned long long*)&n[0]));

  i = 0;
  while (i < outlen) {
	const unsigned long long todo = outlen - i;
	const int iter = (todo+15/16)>(vc/4) ? vc/4 : (todo+15/16);
	const unsigned long long lb = iter * 16;
	ALIGN16 unsigned char outni[lb];
	aes256_4ft_encryptX((unsigned int*)outni, n2, rkeys, iter);
   	unsigned long long mj = lb;
	if ((i+mj)>=outlen)
		mj = outlen-i;
	for (j = 0 ; j < mj ; j++)
		out[i+j] = outni[j]; 
	i += lb;
  }

  return 0;
}

int crypto_stream_xor(
unsigned char *out,
const unsigned char *in,
unsigned long long inlen,
const unsigned char *n,
const unsigned char *k
)
{
  unsigned long int vc = __builtin_epi_vsetvl(512, __epi_e32, __epi_m1); // vc should be 256 or more, otherwise vrgather won't work for FT
  ALIGN16 unsigned long long n2[16];
  unsigned long long i, j, k2;
  unsigned int rkeys[60];
  aes256_setkey_encrypt((const unsigned int*)k,rkeys);
  vc &= ~0x03ull;

#if defined(REGISTER_SBOX)
  if (vc < 256) { fprintf(stderr, "vector too short for register-based s-boxes\n"); exit(-1); };
#endif

  /* n2 is in byte-reversed (i.e., native little endian)
     order to make increment/testing easier */
  n2[1] = _bswap64((*(unsigned long long*)&n[8]));
  n2[0] = _bswap64((*(unsigned long long*)&n[0]));

  i = 0;
  while (i < inlen) {
        const unsigned long long todo = inlen - i;
        const int iter = (todo+15/16)>(vc/4) ? vc/4 : (todo+15/16);
        const unsigned long long lb = iter * 16; 
        ALIGN16 unsigned char outni[lb];
        aes256_4ft_encryptX((unsigned int*)outni, n2, rkeys, iter);
        unsigned long long mj = lb;
        if ((i+mj)>=inlen)
                mj = inlen-i;
        for (j = 0 ; j < mj ; j++)
                out[i+j] = in[i+j] ^ outni[j];
        i += lb;
  }


  return 0;
}
