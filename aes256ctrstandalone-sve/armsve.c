/*
  armsve.c
  AES-CTR
  Romain Dolbeau
  Public Domain
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arm_sve.h>
#include "crypto_stream.h"
#include "stdaes-common.h"

#ifdef __INTEL_COMPILER
#define ALIGN16 __declspec(align(16))
#define ALIGN32 __declspec(align(32))
#define ALIGN64 __declspec(align(64))
#else // assume GCC
#define ALIGN16  __attribute__((aligned(16)))
#define ALIGN32  __attribute__((aligned(32)))
#define ALIGN64  __attribute__((aligned(64)))
#define _bswap64(a) __builtin_bswap64(a)
#define _bswap(a) __builtin_bswap(a)
#endif

#define sverotr_u32_z(a,imm) sveor_u32_z(svptrue_b32(), svlsl_n_u32_z(svptrue_b32(), a, imm),svlsr_n_u32_z(svptrue_b32(), a, 32-imm))

static inline void printiv32(const svuint32_t v) {
	unsigned long long vc = svcntb();
        unsigned int data[vc/4];
        svst1_u32(svptrue_b32(), data, v);
        for (int i = 0 ; i < vc/4; i++) {
                printf("0x%08x  ", data[i]);
        }
        printf("\n");
}

static inline void printiv64(const svuint64_t v) {
        unsigned long long vc = svcntb();
        unsigned long long int data[vc/8];
        svst1_u64(svptrue_b64(), data, v);
        for (int i = 0 ; i < vc/8; i++) {
                printf("0x%016llx  ", data[i]);
        }
        printf("\n");
}



static inline svuint32_t sverot_q_u32(const svuint32_t a) {
	static const unsigned int lpat[64] = { 1,2,3,0,5,6,7,4,9,10,11,8,13,14,15,12,17,18,19,16,21,22,23,20,25,26,27,24,29,30,31,28,33,34,35,32,37,38,39,36,41,42,43,40,45,46,47,44,49,50,51,48,53,54,55,52,57,58,59,56,61,62,63,60 };
	const svuint32_t vlpat = svld1_u32(svptrue_b32(), lpat);
	const svuint32_t b = svtbl_u32(a, vlpat);
        return b;
}


static inline void aes256_4ft_encryptX(unsigned int *output, unsigned long long int *input, const unsigned int *aes_edrk, const int block)
{
  svuint32_t vinput;
  svuint32_t vaes_edrk;
  svuint32_t X, Y;
  svuint32_t X8, Y8;

  unsigned int i = 0, j = 0;
  const unsigned int l_aes_nr = 14; // AES256
  /* let's assume we have at least 8192 bits worth of vector ... I'm an optimist :-) */
  unsigned int X0, X1, X2, X3, Y0, Y1, Y2, Y3;

//  vinput = svld1_u32(svptrue_b32(), input);
//  vinput = svreinterpret_u32_u64(svrevb_u64_z(svptrue_b64(), svreinterpret_u64_u32(vinput)));
  long long nl = *(long long*)&input[1];

  if ((nl < 0) && (nl + block) >= 0) {
	//wrap
        long long int wrapnumber = (block - (nl+block));
        svuint64_t vinc = svindex_u64(0, 1);
        svuint64_t vzero = svdup_n_u64(0);
	svuint64_t vone = svdup_n_u64(1);
	svbool_t carries = svcmpge_n_u64(svptrue_b64(), vinc, wrapnumber);
        vinc = svzip1_u64(svsel_u64(carries, vone, vzero), vinc);
        vinput = svreinterpret_u32_u64(svadd_u64_z(svptrue_b64(), svld1rq_u64(svptrue_b64(), input), vinc));
	input[1] += block;
	input[0] ++;
	vinput = svreinterpret_u32_u64(svrevb_u64_z(svptrue_b64(), svreinterpret_u64_u32(vinput)));
  } else {
        //no wrap, maybe unecessary, using only wrap code
 	svuint64_t vinc = svindex_u64(0, 1);
	svuint64_t vzero = svdup_n_u64(0);
	vinc = svzip1_u64(vzero, vinc);
	vinput = svreinterpret_u32_u64(svadd_u64_z(svptrue_b64(), svld1rq_u64(svptrue_b64(), input), vinc));
	input[1] += block;
	vinput = svreinterpret_u32_u64(svrevb_u64_z(svptrue_b64(), svreinterpret_u64_u32(vinput)));
  }

  vaes_edrk = svld1rq_u32(svptrue_b32(), aes_edrk);

  X = sveor_u32_z(svptrue_b32(), vinput, vaes_edrk);

  for (i = 4 ; i < (l_aes_nr<<2) ; i+= 4) {

    X8 = svand_n_u32_z(svptrue_b32(), X, 0x0FF);
    Y = svld1_gather_u32index_u32(svptrue_b32(), FT0, X8);

    X = sverot_q_u32(X); /* emulated */
    X = svlsr_n_u32_z(svptrue_b32(), X, 8);
    X8 = svand_n_u32_z(svptrue_b32(), X, 0x0FF);
    Y8 = svld1_gather_u32index_u32(svptrue_b32(), FT1, X8);
    Y = sveor_u32_z(svptrue_b32(), Y8, Y);

    X = sverot_q_u32(X); /* emulated */
    X = svlsr_n_u32_z(svptrue_b32(), X, 8);
    X8 = svand_n_u32_z(svptrue_b32(), X, 0x0FF);
    Y8 = svld1_gather_u32index_u32(svptrue_b32(), FT2, X8);
    Y = sveor_u32_z(svptrue_b32(), Y8, Y);

    X = sverot_q_u32(X); /* emulated */
    X = svlsr_n_u32_z(svptrue_b32(), X, 8);
    X8 = svand_n_u32_z(svptrue_b32(), X, 0x0FF);
    Y8 = svld1_gather_u32index_u32(svptrue_b32(), FT3, X8);
    Y = sveor_u32_z(svptrue_b32(), Y8, Y);

    vaes_edrk = svld1rq_u32(svptrue_b32(), aes_edrk + i);

    X = sveor_u32_z(svptrue_b32(), Y, vaes_edrk);
  }

  /* try with non-vector last round */
  svst1_u32(svptrue_b32(), output, X);
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


#if 0
static inline void aes256_4ft_encrypt4(unsigned int *output, const unsigned int *input, const unsigned int *aes_edrk)
{
  __m512i vinput;
  __m512i vaes_edrk;
  __m512i X, Y;
  __m512i X8, Y8;

  unsigned int i = 0, j = 0;
  const unsigned int l_aes_nr = 14; // AES256
  /* let's assume we have at least 8192 bits worth of vector ... I'm an optimist :-) */

  vinput    = _mm512_load_si512((const __m512i*)input);
  vinput = _mm512_shuffle_epi8(vinput, _mm512_set_epi8(56,57,58,59,60,61,62,63,48,49,50,51,52,53,54,55,40,41,42,43,44,45,46,47,32,33,34,35,36,37,38,39,24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7));
  __m128i vaes_edrk_s = _mm_load_si128((const __m128i*)aes_edrk);
  vaes_edrk =  _mm512_broadcast_i32x4(vaes_edrk_s);

  X = _mm512_xor_si512(vinput, vaes_edrk);

  unsigned int X0, X1, X2, X3, Y0, Y1, Y2, Y3;

  for (i = 4 ; i < (l_aes_nr<<2) ; i+= 4) {

    X8 = _mm512_and_si512(X, _mm512_set1_epi32(0x0FF));
    Y = _mm512_i32gather_epi32(X8, FT0, 4);

    X = _mm512_brotri_epi32(X, 4); /* emulated */
    X = _mm512_srli_epi32(X, 8);
    X8 = _mm512_and_si512(X, _mm512_set1_epi32(0x0FF));
    Y8 = _mm512_i32gather_epi32(X8, FT1, 4);
    Y = _mm512_xor_si512(Y8, Y);

    X = _mm512_brotri_epi32(X, 4); /* emulated */
    X = _mm512_srli_epi32(X, 8);
    X8 = _mm512_and_si512(X, _mm512_set1_epi32(0x0FF));
    Y8 = _mm512_i32gather_epi32(X8, FT2, 4);
    Y = _mm512_xor_si512(Y8, Y);

    X = _mm512_brotri_epi32(X, 4); /* emulated */
    X = _mm512_srli_epi32(X, 8);
    X8 = _mm512_and_si512(X, _mm512_set1_epi32(0x0FF));
    Y8 = _mm512_i32gather_epi32(X8, FT3, 4);
    Y = _mm512_xor_si512(Y8, Y);

    vaes_edrk_s = _mm_load_si128((const __m128i*)(aes_edrk + i));
    vaes_edrk =  _mm512_broadcast_i32x4(vaes_edrk_s);

    X = _mm512_xor_si512(Y, vaes_edrk);
  }

  /* try with non-vector last round */
  _mm512_store_si512((__m512i*)output, X);
  for (j = 0 ; j < 16 ; j+= 4) {
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
#endif


int crypto_stream(
unsigned char *out,
unsigned long long outlen,
const unsigned char *n,
const unsigned char *k
)
{
  unsigned long long int vc = svcntb();
  ALIGN16 unsigned long long n2[2];
  unsigned long long i, j, k2;
  unsigned int rkeys[60];
  aes256_setkey_encrypt((const unsigned int*)k,rkeys);

  /* n2 is in byte-reversed (i.e., native little endian)
     order to make increment/testing easier */
  n2[1] = _bswap64((*(unsigned long long*)&n[8]));
  n2[0] = _bswap64((*(unsigned long long*)&n[0]));
  i = 0;
  while (i < outlen) {
	const unsigned long long todo = outlen - i;
	const int iter = vc/16;
	const unsigned long long lb = iter * 16; // a.k.a. vc ...
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
  unsigned long long int vc = svcntb();
  ALIGN16 unsigned long long n2[2];
  unsigned long long i, j, k2;
  unsigned int rkeys[60];
  aes256_setkey_encrypt((const unsigned int*)k,rkeys);

  /* n2 is in byte-reversed (i.e., native little endian)
     order to make increment/testing easier */
  n2[1] = _bswap64((*(unsigned long long*)&n[8]));
  n2[0] = _bswap64((*(unsigned long long*)&n[0]));
  
  i = 0;
  while (i < inlen) {
        const unsigned long long todo = inlen - i;
        const int iter = vc/16;
        const unsigned long long lb = iter * 16; // a.k.a. vc ...
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
