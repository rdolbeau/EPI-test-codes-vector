#ifndef crypto_stream_aes256ctr_H
#define crypto_stream_aes256ctr_H

#define crypto_stream_aes256ctr_armsve_KEYBYTES 32
#define crypto_stream_aes256ctr_armsve_NONCEBYTES 16
 
#ifdef __cplusplus
extern "C" {
#endif
extern int crypto_stream_aes256ctr_armsve(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_aes256ctr_armsve_xor(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_aes256ctr_armsve_beforenm(unsigned char *,const unsigned char *);
extern int crypto_stream_aes256ctr_armsve_afternm(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_aes256ctr_armsve_xor_afternm(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_stream_aes256ctr crypto_stream_aes256ctr_armsve
#define crypto_stream_aes256ctr_xor crypto_stream_aes256ctr_armsve_xor
#define crypto_stream_aes256ctr_beforenm crypto_stream_aes256ctr_armsve_beforenm
#define crypto_stream_aes256ctr_afternm crypto_stream_aes256ctr_armsve_afternm
#define crypto_stream_aes256ctr_xor_afternm crypto_stream_aes256ctr_armsve_xor_afternm
#define crypto_stream_aes256ctr_KEYBYTES crypto_stream_aes256ctr_armsve_KEYBYTES
#define crypto_stream_aes256ctr_NONCEBYTES crypto_stream_aes256ctr_armsve_NONCEBYTES
#define crypto_stream_aes256ctr_BEFORENMBYTES crypto_stream_aes256ctr_armsve_BEFORENMBYTES
#define crypto_stream_aes256ctr_IMPLEMENTATION "crypto_stream/aes256ctr/armsve"
#ifndef crypto_stream_aes256ctr_armsve_VERSION
#define crypto_stream_aes256ctr_armsve_VERSION "-"
#endif
#define crypto_stream_aes256ctr_VERSION crypto_stream_aes256ctr_armsve_VERSION

#endif
