/*
  core.c version $Date: 2020/03/23 13:45:12 $
  aes256encrypt
  Romain Dolbeau
  Public Domain
*/

#include "crypto_core.h"
#include "stdaes-common.h"

static inline void printiv32(const __epi_2xi32 v, const unsigned long int vl) {
	unsigned int data[vl];
	__builtin_epi_vstore_2xi32(data, v, vl);
	for (int i = 0 ; i < vl; i++) {
			printf("0x%08x  ", data[i]);
	}
	printf("\n");
}



//#define __builtin_epi_vrotr_2xi32(a,b,vc) __builtin_epi_vor_2xi32(__builtin_epi_vsll_2xi32(a, b, vc),__builtin_epi_vsrl_2xi32(a, __builtin_epi_vsub_2xi32(__builtin_epi_vmv_v_x_2xi32(32, vc), b, vc), vc), vc)

static inline __epi_2xi32 __builtin_epi_vrotr_2xi32(const __epi_2xi32 a, const __epi_2xi32 b, const unsigned long int vc) {
	const __epi_2xi32 asb = __builtin_epi_vsrl_2xi32(a, b, vc);
	const __epi_2xi32 as32mb = __builtin_epi_vsll_2xi32(a, __builtin_epi_vsub_2xi32(__builtin_epi_vmv_v_x_2xi32(32, vc), b, vc), vc);
	return __builtin_epi_vor_2xi32(asb, as32mb, vc);
}

static inline __epi_2xi32 __builtin_epi_vrotlv4_2xi32(const __epi_2xi32 a, const unsigned long int vc) {
	// vc must be 4
	static const unsigned int lpat[4] = { 1, 2, 3, 0};
	static const unsigned int rpat[4] = { 3, 0, 1, 2};
	const __epi_2xi32 vlpat = __builtin_epi_vload_2xi32(lpat, vc);
	return __builtin_epi_vrgather_2xi32(a, vlpat, vc);
}


static inline void aes256_4ft_encrypt(const unsigned int *aes_edrk, const unsigned int *input, unsigned int *output)
{
  __epi_2xi32 vinput;
  __epi_2xi32 vaes_edrk;
  __epi_2xi32 X, Y;
  __epi_2xi32 v0xFF, v0x8, v0x10, v0x18;
  __epi_2xi32 X8, Y8;
  __epi_2xi32 vFT0, vFT1, vFT2, vFT3;
  __epi_2xi32 vSHIFTP1, vSHIFTP2, vSHIFTP3, vSHIFTP4;
  __epi_2xi32 vROTP1, vROTP2, vROTP3, vROTP4;
  static const unsigned int rotp1[4] = { 0, 8, 16, 24 };
  static const unsigned int rotp2[4] = { 24, 0, 8, 16 };
  static const unsigned int rotp3[4] = { 16, 24, 0, 8 };
  static const unsigned int rotp4[4] = { 8, 16, 24, 0 };
  static const unsigned int shiftp1[4] = { 0, 24, 16, 8 };
  static const unsigned int shiftp2[4] = { 8, 0, 24, 16 };
  static const unsigned int shiftp3[4] = { 16, 8, 0, 24 };
  static const unsigned int shiftp4[4] = { 24, 16, 8, 0 };

  unsigned int i = 0, j = 0;
  const unsigned int l_aes_nr = 14; // AES256
  /* let's assume we have at least 8192 bits worth of vector ... I'm an optimist :-) */
  
  vinput    = __builtin_epi_vload_2xi32(input   , 4);
  vaes_edrk = __builtin_epi_vload_2xi32(aes_edrk, 4);

  X = __builtin_epi_vxor_2xi32(vinput, vaes_edrk, 4);
  
__builtin_epi_vstore_2xi32(output, X, 4);
  unsigned int X0, X1, X2, X3, Y0, Y1, Y2, Y3;
  X0 = output[0];
  X1 = output[1];
  X2 = output[2];
  X3 = output[3];

  v0xFF = __builtin_epi_vmv_v_x_2xi32(0xFF, 4);
  v0x8 = __builtin_epi_vmv_v_x_2xi32(0x8, 4);
  v0x10 = __builtin_epi_vmv_v_x_2xi32(0x10, 4);
  v0x18 = __builtin_epi_vmv_v_x_2xi32(0x18, 4);
  vSHIFTP1 = __builtin_epi_vload_2xi32(shiftp1, 4);
  vSHIFTP2 = __builtin_epi_vload_2xi32(shiftp2, 4);
  vSHIFTP3 = __builtin_epi_vload_2xi32(shiftp3, 4);
  vSHIFTP4 = __builtin_epi_vload_2xi32(shiftp4, 4);
  vROTP1 = __builtin_epi_vload_2xi32(rotp1, 4);
  vROTP2 = __builtin_epi_vload_2xi32(rotp2, 4);
  vROTP3 = __builtin_epi_vload_2xi32(rotp3, 4);
  vROTP4 = __builtin_epi_vload_2xi32(rotp4, 4);

  vFT0 = __builtin_epi_vload_2xi32(FT0, 256);
  vFT1 = __builtin_epi_vload_2xi32(FT1, 256);
  vFT2 = __builtin_epi_vload_2xi32(FT2, 256);
  vFT3 = __builtin_epi_vload_2xi32(FT3, 256);

  for (i = 4 ; i < (l_aes_nr<<2) ; i+= 4) {

    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y = __builtin_epi_vrgather_2xi32(vFT0, X8, 4);
#if defined(onetable)
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x18, 4); /* emulated */
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
    
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x10, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT0, X8, 4);
    Y8 = __builtin_epi_vrotr_2xi32(Y8, v0x8, 4); /* emulated */
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
#else
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT1, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
    
    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT2, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);

    X = __builtin_epi_vrotlv4_2xi32(X, 4); /* emulated */
    X = __builtin_epi_vsrl_2xi32(X, v0x8, 4);
    X8 = __builtin_epi_vand_2xi32(X, v0xFF, 4);
    Y8 = __builtin_epi_vrgather_2xi32(vFT3, X8, 4);
    Y = __builtin_epi_vxor_2xi32(Y8, Y, 4);
#endif
    
    vaes_edrk = __builtin_epi_vload_2xi32(aes_edrk + i, 4);

    X = __builtin_epi_vxor_2xi32(Y, vaes_edrk, 4);
  }

  /* try with non-vector last round */
  __builtin_epi_vstore_2xi32(output, X, 4);
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

int crypto_core(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
) {
  unsigned int rkeys[60];
  aes256_setkey_encrypt((const unsigned int*)k,rkeys);
  aes256_4ft_encrypt(rkeys, (const unsigned int*)in, (unsigned int*)out);
  return 0;
}
