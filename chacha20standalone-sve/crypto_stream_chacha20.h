#ifndef crypto_stream_chacha20_H
#define crypto_stream_chacha20_H

#define crypto_stream_chacha20_dolbeau_arm_sve_KEYBYTES 32
#define crypto_stream_chacha20_dolbeau_arm_sve_NONCEBYTES 8
 
#ifdef __cplusplus
extern "C" {
#endif
extern int crypto_stream_chacha20_dolbeau_arm_sve(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_arm_sve_xor(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_arm_sve_beforenm(unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_arm_sve_afternm(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_arm_sve_xor_afternm(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_stream_chacha20 crypto_stream_chacha20_dolbeau_arm_sve
#define crypto_stream_chacha20_xor crypto_stream_chacha20_dolbeau_arm_sve_xor
#define crypto_stream_chacha20_beforenm crypto_stream_chacha20_dolbeau_arm_sve_beforenm
#define crypto_stream_chacha20_afternm crypto_stream_chacha20_dolbeau_arm_sve_afternm
#define crypto_stream_chacha20_xor_afternm crypto_stream_chacha20_dolbeau_arm_sve_xor_afternm
#define crypto_stream_chacha20_KEYBYTES crypto_stream_chacha20_dolbeau_arm_sve_KEYBYTES
#define crypto_stream_chacha20_NONCEBYTES crypto_stream_chacha20_dolbeau_arm_sve_NONCEBYTES
#define crypto_stream_chacha20_BEFORENMBYTES crypto_stream_chacha20_dolbeau_arm_sve_BEFORENMBYTES
#define crypto_stream_chacha20_IMPLEMENTATION "crypto_stream/chacha20/dolbeau/arm-sve"
#ifndef crypto_stream_chacha20_dolbeau_arm_sve_VERSION
#define crypto_stream_chacha20_dolbeau_arm_sve_VERSION "-"
#endif
#define crypto_stream_chacha20_VERSION crypto_stream_chacha20_dolbeau_arm_sve_VERSION

#endif
