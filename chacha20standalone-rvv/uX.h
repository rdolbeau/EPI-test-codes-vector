/*
uX.h version $Date: 2020/04/27 07:42:51 $
D. J. Bernstein
Romain Dolbeau
Public domain.

This does a variable number of blocks, depending on the V vector size.
*/

#define VEC4_ROT(a,imm) __builtin_epi_vxor_2xi32(__builtin_epi_vsll_2xi32(a, __builtin_epi_vmv_v_x_2xi32(imm, vc), vc),__builtin_epi_vsrl_2xi32(a, __builtin_epi_vmv_v_x_2xi32(32-imm, vc), vc), vc)

//#define VEC4_ROT16(a) svrevh_u32_z(svptrue_b32(), a)

#define VEC4_QUARTERROUND(a,b,c,d)                                \
   x_##a = __builtin_epi_vadd_2xi32(x_##a, x_##b, vc); t_##a = __builtin_epi_vxor_2xi32(x_##d, x_##a, vc); x_##d = VEC4_ROT(t_##a, 16); \
   x_##c = __builtin_epi_vadd_2xi32(x_##c, x_##d, vc); t_##c = __builtin_epi_vxor_2xi32(x_##b, x_##c, vc); x_##b = VEC4_ROT(t_##c, 12); \
   x_##a = __builtin_epi_vadd_2xi32(x_##a, x_##b, vc); t_##a = __builtin_epi_vxor_2xi32(x_##d, x_##a, vc); x_##d = VEC4_ROT(t_##a,  8); \
   x_##c = __builtin_epi_vadd_2xi32(x_##c, x_##d, vc); t_##c = __builtin_epi_vxor_2xi32(x_##b, x_##c, vc); x_##b = VEC4_ROT(t_##c,  7)

  if (!bytes) return;
if (bytes>=256) {
  unsigned long int vc = __builtin_epi_vsetvlmax(__epi_e32, __epi_m1);
  u32 in12, in13;
  __epi_2xi32 x_0 = __builtin_epi_vmv_v_x_2xi32(x[0], vc);
  __epi_2xi32 x_1 = __builtin_epi_vmv_v_x_2xi32(x[1], vc);
  __epi_2xi32 x_2 = __builtin_epi_vmv_v_x_2xi32(x[2], vc);
  __epi_2xi32 x_3 = __builtin_epi_vmv_v_x_2xi32(x[3], vc);
  __epi_2xi32 x_4 = __builtin_epi_vmv_v_x_2xi32(x[4], vc);
  __epi_2xi32 x_5 = __builtin_epi_vmv_v_x_2xi32(x[5], vc);
  __epi_2xi32 x_6 = __builtin_epi_vmv_v_x_2xi32(x[6], vc);
  __epi_2xi32 x_7 = __builtin_epi_vmv_v_x_2xi32(x[7], vc);
  __epi_2xi32 x_8 = __builtin_epi_vmv_v_x_2xi32(x[8], vc);
  __epi_2xi32 x_9 = __builtin_epi_vmv_v_x_2xi32(x[9], vc);
  __epi_2xi32 x_10 = __builtin_epi_vmv_v_x_2xi32(x[10], vc);
  __epi_2xi32 x_11 = __builtin_epi_vmv_v_x_2xi32(x[11], vc);
  __epi_2xi32 x_12;// = __builtin_epi_vmv_v_x_2xi32(x[12], vc); /* useless */
  __epi_2xi32 x_13;// = __builtin_epi_vmv_v_x_2xi32(x[13], vc); /* useless */
  __epi_2xi32 x_14 = __builtin_epi_vmv_v_x_2xi32(x[14], vc);
  __epi_2xi32 x_15 = __builtin_epi_vmv_v_x_2xi32(x[15], vc);
  __epi_2xi32 orig0 = x_0;
  __epi_2xi32 orig1 = x_1;
  __epi_2xi32 orig2 = x_2;
  __epi_2xi32 orig3 = x_3;
  __epi_2xi32 orig4 = x_4;
  __epi_2xi32 orig5 = x_5;
  __epi_2xi32 orig6 = x_6;
  __epi_2xi32 orig7 = x_7;
  __epi_2xi32 orig8 = x_8;
  __epi_2xi32 orig9 = x_9;
  __epi_2xi32 orig10 = x_10;
  __epi_2xi32 orig11 = x_11;
  __epi_2xi32 orig12;// = x_12; /* useless */
  __epi_2xi32 orig13;// = x_13; /* useless */
  __epi_2xi32 orig14 = x_14;
  __epi_2xi32 orig15 = x_15;
  __epi_2xi32 t_0;
  __epi_2xi32 t_1;
  __epi_2xi32 t_2;
  __epi_2xi32 t_3;
  __epi_2xi32 t_4;
  __epi_2xi32 t_5;
  __epi_2xi32 t_6;
  __epi_2xi32 t_7;
  __epi_2xi32 t_8;
  __epi_2xi32 t_9;
  __epi_2xi32 t_10;
  __epi_2xi32 t_11;
  __epi_2xi32 t_12;
  __epi_2xi32 t_13;
  __epi_2xi32 t_14;
  __epi_2xi32 t_15;

  while (bytes >= 256) {
{
  vc = bytes / 64;
  vc = vc | (vc >> 1);
  vc = vc | (vc >> 2);
  vc = vc | (vc >> 4);
  vc = vc | (vc >> 8);
  vc = vc | (vc >> 16);
  vc = vc | (vc >> 32);
  vc = vc - (vc >> 1); // rounded to power-of-2
}
  vc = 2 * __builtin_epi_vsetvl(vc/2, __epi_e64, __epi_m1);
    x_0 = orig0;
    x_1 = orig1;
    x_2 = orig2;
    x_3 = orig3;
    x_4 = orig4;
    x_5 = orig5;
    x_6 = orig6;
    x_7 = orig7;
    x_8 = orig8;
    x_9 = orig9;
    x_10 = orig10;
    x_11 = orig11;
    //x_12 = orig12; /* useless */
    //x_13 = orig13; /* useless */
    x_14 = orig14;
    x_15 = orig15;

    in12 = x[12];
    in13 = x[13];
    u64 in1213 = ((u64)in12) | (((u64)in13) << 32);
//#define CHACHA20_OLDCOUNTER
#ifdef CHACHA20_OLDCOUNTER
    /* vid makes it easy to build the input counter */
    /* this likely should be rewritten in an easier(& faster) way using vector add-with-carry */
    const __epi_1xi64 addv13 = __builtin_epi_vid_1xi64(vc/2);
    const __epi_1xi64 vvcover2 = __builtin_epi_vmv_v_x_1xi64(vc/2, vc/2);
    const __epi_1xi64 addv12 = __builtin_epi_vadd_1xi64(addv13, vvcover2, vc/2);
    __epi_1xi64 t12, t13;
    t12 = __builtin_epi_vmv_v_x_1xi64(in1213, vc/2);
    t13 = __builtin_epi_vmv_v_x_1xi64(in1213, vc/2);

    x_12 = (__epi_2xi32)__builtin_epi_vadd_1xi64(addv12, t12, vc/2);
    x_13 = (__epi_2xi32)__builtin_epi_vadd_1xi64(addv13, t13, vc/2);

    __epi_2xi32 t = x_12;
    x_12 = __builtin_epi_uzp1_2xi32(x_13, x_12, vc); // fixme: how ?
    x_13 = __builtin_epi_uzp2_2xi32(x_13, t, vc);
#else
    const __epi_2xi32 addv = __builtin_epi_vid_2xi32(vc);
    __epi_2xi32 oldx_12;
    __epi_2xi1 carry;
    carry = __builtin_epi_vmxor_2xi1(carry, carry, vc); //
    x_12 = __builtin_epi_vmv_v_x_2xi32(in12, vc);
    x_13 = __builtin_epi_vmv_v_x_2xi32(in13, vc);
    oldx_12 = x_12;
    x_12 = __builtin_epi_vadc_2xi32(x_12, addv, carry, vc);
    carry = __builtin_epi_vmadc_carry_in_2xi32(oldx_12, addv, carry, vc);
    x_13 = __builtin_epi_vadc_2xi32(x_13, __builtin_epi_vmv_v_x_2xi32(0, vc), carry, vc);
#endif

    orig12 = x_12;
    orig13 = x_13;

    in1213 += vc;
    
    x[12] = in1213 & 0xFFFFFFFF;
    x[13] = (in1213>>32)&0xFFFFFFFF;

    for (i = 0 ; i < ROUNDS ; i+=2) {
      VEC4_QUARTERROUND( 0, 4, 8,12);
      VEC4_QUARTERROUND( 1, 5, 9,13);
      VEC4_QUARTERROUND( 2, 6,10,14);
      VEC4_QUARTERROUND( 3, 7,11,15);
      VEC4_QUARTERROUND( 0, 5,10,15);
      VEC4_QUARTERROUND( 1, 6,11,12);
      VEC4_QUARTERROUND( 2, 7, 8,13);
      VEC4_QUARTERROUND( 3, 4, 9,14);
    }

#define ONEQUAD_TRANSPOSE(a,b,c,d)                                      \
    {                                                                   \
      __epi_2xi32 t00, t01, t10, t11;                                    \
      __epi_1xi64 t0, t1, t2, t3;                                        \
      x_##a = __builtin_epi_vadd_2xi32(x_##a, orig##a, vc);                                \
      x_##b = __builtin_epi_vadd_2xi32(x_##b, orig##b, vc);                                \
      x_##c = __builtin_epi_vadd_2xi32(x_##c, orig##c, vc);                                \
      x_##d = __builtin_epi_vadd_2xi32(x_##d, orig##d, vc);                                \
      t00 = __builtin_epi_trn1_2xi32(x_##a,x_##b,vc);\
      t01 = __builtin_epi_trn2_2xi32(x_##a,x_##b,vc);\
      t10 = __builtin_epi_trn1_2xi32(x_##c,x_##d,vc);\
      t11 = __builtin_epi_trn2_2xi32(x_##c,x_##d,vc);\
      x_##a = (__epi_2xi32)__builtin_epi_trn1_1xi64((__epi_1xi64)t00, (__epi_1xi64)t10, vc/2);\
      x_##b = (__epi_2xi32)__builtin_epi_trn1_1xi64((__epi_1xi64)t01, (__epi_1xi64)t11, vc/2);\
      x_##c = (__epi_2xi32)__builtin_epi_trn2_1xi64((__epi_1xi64)t00, (__epi_1xi64)t10, vc/2);\
      x_##d = (__epi_2xi32)__builtin_epi_trn2_1xi64((__epi_1xi64)t01, (__epi_1xi64)t11, vc/2);\
      t0 = __builtin_epi_vxor_1xi64((__epi_1xi64)x_##a, __builtin_epi_vload_indexed_1xi64(( long int*)(m+0), gvv, vc/2), vc/2);\
      __builtin_epi_vstore_indexed_1xi64(( long int*)(out+0), t0, gvv, vc/2); \
      t1 = __builtin_epi_vxor_1xi64((__epi_1xi64)x_##b, __builtin_epi_vload_indexed_1xi64(( long int*)(m+64), gvv, vc/2), vc/2);\
      __builtin_epi_vstore_indexed_1xi64(( long int*)(out+64), t1, gvv, vc/2); \
      t2 = __builtin_epi_vxor_1xi64((__epi_1xi64)x_##c, __builtin_epi_vload_indexed_1xi64(( long int*)(m+128), gvv, vc/2), vc/2);\
      __builtin_epi_vstore_indexed_1xi64(( long int*)(out+128), t2, gvv, vc/2); \
      t3 = __builtin_epi_vxor_1xi64((__epi_1xi64)x_##d, __builtin_epi_vload_indexed_1xi64(( long int*)(m+192), gvv, vc/2), vc/2);\
      __builtin_epi_vstore_indexed_1xi64(( long int*)(out+192), t3, gvv, vc/2); \
    }

#define ONE_STRIDED(z, o)						\
    {									\
	    __epi_2xi32 t00;						\
	    x_##z = __builtin_epi_vadd_2xi32(x_##z, orig##z, vc);	\
	    t00 = __builtin_epi_vload_strided_2xi32((long int*)(m+o), 64, vc); \
	    t00 = __builtin_epi_vxor_2xi32(x_##z, t00, vc); \
	    __builtin_epi_vstore_strided_2xi32((long int*)(out+o), t00, 64, vc); \
    }
#define ONEQUAD_STRIDED(a,b,c,d) \
    ONE_STRIDED(a,0) ONE_STRIDED(b,4) ONE_STRIDED(c,8) ONE_STRIDED(d,12) 
    
#define ONEQUAD(a,b,c,d) ONEQUAD_TRANSPOSE(a,b,c,d)

#if 1 // for ONEQUAD_TRANSPOSE only
    __epi_1xi64 gvv, gvvl, gvvh;
    gvvl = __builtin_epi_vid_1xi64(vc/2); // (0, 1, 2, 3, ...)
    gvvl = __builtin_epi_vsll_1xi64(gvvl,  __builtin_epi_vmv_v_x_1xi64(8, vc/2), vc/2); // (0, 256, 512, 768, ... )
    /* emulate zip1(a,a) */
    __epi_1xi64 idx = __builtin_epi_vid_1xi64(vc/2); // (0, 1, 2, 3, ...)
    __epi_1xi64 hidx = __builtin_epi_vsrl_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, vc/2), vc/2); // (0, 0, 1, 1, 2, 2, ...)
    gvvl = __builtin_epi_vrgather_1xi64(gvvl, hidx, vc/2); // (0, 0, 256, 256, 512, 512, ... )
    /* emulate vadd + vdupq_n_s64 with vmerge ...*/
    __epi_1xi64 gvvlp8 = __builtin_epi_vadd_1xi64(gvvl, __builtin_epi_vmv_v_x_1xi64(8, vc/2), vc/2); // (8, 8, 264, 264, 520, 520)
    __epi_1xi64 mask = __builtin_epi_vand_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(1, vc/2), vc/2); // (0, 1, 0, 1, 0, ...)
    __epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64(mask);
    gvv = __builtin_epi_vmerge_1xi64(gvvl, gvvlp8, mmask, vc/2); // should give us what we want ...
#endif

    ONEQUAD(0,1,2,3);
    m+=16;
    out+=16;
    ONEQUAD(4,5,6,7);
    m+=16;
    out+=16;
    ONEQUAD(8,9,10,11);
    m+=16;
    out+=16;
    ONEQUAD(12,13,14,15);
    m-=48;
    out-=48;
    
#undef ONEQUAD
#undef ONEQUAD_TRANSPOSE
 
    bytes -= 64*vc;
    out += 64*vc;
    m += 64*vc;
  }
 }
#undef VEC4_ROT
#undef VEC4_QUARTERROUND
