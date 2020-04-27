#ifndef crypto_stream_chacha20_H
#define crypto_stream_chacha20_H

#define crypto_stream_chacha20_dolbeau_riscv_v_KEYBYTES 32
#define crypto_stream_chacha20_dolbeau_riscv_v_NONCEBYTES 8
 
#ifdef __cplusplus
extern "C" {
#endif
extern int crypto_stream_chacha20_dolbeau_riscv_v(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_riscv_v_xor(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_riscv_v_beforenm(unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_riscv_v_afternm(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_chacha20_dolbeau_riscv_v_xor_afternm(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_stream_chacha20 crypto_stream_chacha20_dolbeau_riscv_v
#define crypto_stream_chacha20_xor crypto_stream_chacha20_dolbeau_riscv_v_xor
#define crypto_stream_chacha20_beforenm crypto_stream_chacha20_dolbeau_riscv_v_beforenm
#define crypto_stream_chacha20_afternm crypto_stream_chacha20_dolbeau_riscv_v_afternm
#define crypto_stream_chacha20_xor_afternm crypto_stream_chacha20_dolbeau_riscv_v_xor_afternm
#define crypto_stream_chacha20_KEYBYTES crypto_stream_chacha20_dolbeau_riscv_v_KEYBYTES
#define crypto_stream_chacha20_NONCEBYTES crypto_stream_chacha20_dolbeau_riscv_v_NONCEBYTES
#define crypto_stream_chacha20_BEFORENMBYTES crypto_stream_chacha20_dolbeau_riscv_v_BEFORENMBYTES
#define crypto_stream_chacha20_IMPLEMENTATION "crypto_stream/chacha20/dolbeau/riscv-v"
#ifndef crypto_stream_chacha20_dolbeau_riscv_v_VERSION
#define crypto_stream_chacha20_dolbeau_riscv_v_VERSION "-"
#endif
#define crypto_stream_chacha20_VERSION crypto_stream_chacha20_dolbeau_riscv_v_VERSION

#endif
