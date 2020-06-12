/*
uX.h version $Date$
D. J. Bernstein
Romain Dolbeau
Public domain.

This does a variable number of blocks, depending on the SVE vector size.
*/

#define VEC4_ROT(a,imm) sveor_u32_z(svptrue_b32(), svlsl_n_u32_z(svptrue_b32(), a, imm),svlsr_n_u32_z(svptrue_b32(), a, 32-imm))

#define VEC4_ROT16(a) svrevh_u32_z(svptrue_b32(), a)

#define VEC4_QUARTERROUND_INT(a,b,c,d)                                \
   x_##a = svadd_u32_z(svptrue_b32(), x_##a, x_##b); t_##a = sveor_u32_z(svptrue_b32(), x_##d, x_##a); x_##d = VEC4_ROT16(t_##a); \
   x_##c = svadd_u32_z(svptrue_b32(), x_##c, x_##d); t_##c = sveor_u32_z(svptrue_b32(), x_##b, x_##c); x_##b = VEC4_ROT(t_##c, 12); \
   x_##a = svadd_u32_z(svptrue_b32(), x_##a, x_##b); t_##a = sveor_u32_z(svptrue_b32(), x_##d, x_##a); x_##d = VEC4_ROT(t_##a,  8); \
   x_##c = svadd_u32_z(svptrue_b32(), x_##c, x_##d); t_##c = sveor_u32_z(svptrue_b32(), x_##b, x_##c); x_##b = VEC4_ROT(t_##c,  7)

#define VEC4_QUARTERROUND_ASM(a,b,c,d)				\
	asm volatile("ptrue p0.s\n"				\
		     "add %0.s, %0.s, %1.s\n"			\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "revh %3.s, p0/m, %3.s\n"			\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "lsr %4.s, %1.s, #20\n"			\
		     "lsl %1.s, %1.s, #12\n"			\
		     "eor %1.d, %1.d, %4.d\n"			\
		     "\n"					\
		     "add %0.s, %0.s, %1.s\n"			\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "lsr %4.s, %3.s, #24\n"			\
		     "lsl %3.s, %3.s, #8\n"			\
		     "eor %3.d, %3.d, %4.d\n"			\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "lsr %4.s, %1.s, #25\n"			\
		     "lsl %1.s, %1.s, #7\n"			\
		     "eor %1.d, %1.d, %4.d\n"			\
		     "\n"					\
		     : "+&w" (x_##a), "+&w" (x_##b), "+&w" (x_##c), "+&w" (x_##d), "=w" (temp1) : : "p0")

#define VEC4_QUARTERROUND(a,b,c,d) VEC4_QUARTERROUND_ASM(a,b,c,d)


#define VEC4_DOUBLEQUARTERROUND_ASM(a,b,c,d,e,f,g,h)		\
	asm volatile("ptrue p0.s\n"				\
		     "add %0.s, %0.s, %1.s\n"			\
		     "add %4.s, %4.s, %5.s\n"			\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "eor %7.d, %7.d, %4.d\n"			\
		     "revh %3.s, p0/m, %3.s\n"			\
		     "revh %7.s, p0/m, %7.s\n"			\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "add %6.s, %6.s, %7.s\n"			\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "eor %5.d, %5.d, %6.d\n"			\
		     "lsr %8.s, %1.s, #20\n"			\
		     "lsr %9.s, %5.s, #20\n"			\
		     "lsl %1.s, %1.s, #12\n"			\
		     "lsl %5.s, %5.s, #12\n"			\
		     "eor %1.d, %1.d, %8.d\n"			\
		     "eor %5.d, %5.d, %9.d\n"			\
		     "\n"					\
		     "add %0.s, %0.s, %1.s\n"			\
		     "add %4.s, %4.s, %5.s\n"			\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "eor %7.d, %7.d, %4.d\n"			\
		     "lsr %8.s, %3.s, #24\n"			\
		     "lsr %9.s, %7.s, #24\n"			\
		     "lsl %3.s, %3.s, #8\n"			\
		     "lsl %7.s, %7.s, #8\n"			\
		     "eor %3.d, %3.d, %8.d\n"			\
		     "eor %7.d, %7.d, %9.d\n"			\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "add %6.s, %6.s, %7.s\n"			\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "eor %5.d, %5.d, %6.d\n"			\
		     "lsr %8.s, %1.s, #25\n"			\
		     "lsr %9.s, %5.s, #25\n"			\
		     "lsl %1.s, %1.s, #7\n"			\
		     "lsl %5.s, %5.s, #7\n"			\
		     "eor %1.d, %1.d, %8.d\n"			\
		     "eor %5.d, %5.d, %9.d\n"			\
		     "\n"					\
		     : "+&w" (x_##a), "+&w" (x_##b), "+&w" (x_##c), "+&w" (x_##d), "+&w" (x_##e), "+&w" (x_##f), "+&w" (x_##g), "+&w" (x_##h), "=w" (temp1), "=w" (temp2) : : "p0")

/* this one has too many arguments for GCC, works fine with armclang though */
#define VEC4_QUADQUARTERROUND_ASM(a,b,c,d, e,f,g,h, i,j,k,l, m,n,o,p)	\
	asm volatile("ptrue p0.s\n"				\
		     "add %0.s, %0.s, %1.s\n"			\
		     "add %4.s, %4.s, %5.s\n"			\
		     "add %8.s, %8.s, %9.s\n"			\
		     "add %12.s, %12.s, %13.s\n"		\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "eor %7.d, %7.d, %4.d\n"			\
		     "eor %11.d, %11.d, %8.d\n"			\
		     "eor %15.d, %15.d, %12.d\n"		\
		     "revh %3.s, p0/m, %3.s\n"			\
		     "revh %7.s, p0/m, %7.s\n"			\
		     "revh %11.s, p0/m, %11.s\n"		\
		     "revh %15.s, p0/m, %15.s\n"		\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "add %6.s, %6.s, %7.s\n"			\
		     "add %10.s, %10.s, %11.s\n"		\
		     "add %14.s, %14.s, %15.s\n"		\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "eor %5.d, %5.d, %6.d\n"			\
		     "eor %9.d, %9.d, %10.d\n"			\
		     "eor %13.d, %13.d, %14.d\n"		\
		     "lsr %16.s, %1.s, #20\n"			\
		     "lsr %17.s, %5.s, #20\n"			\
		     "lsr %18.s, %9.s, #20\n"			\
		     "lsr %19.s, %13.s, #20\n"			\
		     "lsl %1.s, %1.s, #12\n"			\
		     "lsl %5.s, %5.s, #12\n"			\
		     "lsl %9.s, %9.s, #12\n"			\
		     "lsl %13.s, %13.s, #12\n"			\
		     "eor %1.d, %1.d, %16.d\n"			\
		     "eor %5.d, %5.d, %17.d\n"			\
		     "eor %9.d, %9.d, %18.d\n"			\
		     "eor %13.d, %13.d, %19.d\n"		\
		     "\n"					\
		     "add %0.s, %0.s, %1.s\n"			\
		     "add %4.s, %4.s, %5.s\n"			\
		     "add %8.s, %8.s, %9.s\n"			\
		     "add %12.s, %12.s, %13.s\n"		\
		     "eor %3.d, %3.d, %0.d\n"			\
		     "eor %7.d, %7.d, %4.d\n"			\
		     "eor %11.d, %11.d, %8.d\n"			\
		     "eor %15.d, %15.d, %12.d\n"		\
		     "lsr %16.s, %3.s, #24\n"			\
		     "lsr %17.s, %7.s, #24\n"			\
		     "lsr %18.s, %11.s, #24\n"			\
		     "lsr %19.s, %15.s, #24\n"			\
		     "lsl %3.s, %3.s, #8\n"			\
		     "lsl %7.s, %7.s, #8\n"			\
		     "lsl %11.s, %11.s, #8\n"			\
		     "lsl %15.s, %15.s, #8\n"			\
		     "eor %3.d, %3.d, %16.d\n"			\
		     "eor %7.d, %7.d, %17.d\n"			\
		     "eor %11.d, %11.d, %18.d\n"		\
		     "eor %15.d, %15.d, %19.d\n"		\
		     "\n"					\
		     "add %2.s, %2.s, %3.s\n"			\
		     "add %6.s, %6.s, %7.s\n"			\
		     "add %10.s, %10.s, %11.s\n"		\
		     "add %14.s, %14.s, %15.s\n"		\
		     "eor %1.d, %1.d, %2.d\n"			\
		     "eor %5.d, %5.d, %6.d\n"			\
		     "eor %9.d, %9.d, %10.d\n"			\
		     "eor %13.d, %13.d, %14.d\n"		\
		     "lsr %16.s, %1.s, #25\n"			\
		     "lsr %17.s, %5.s, #25\n"			\
		     "lsr %18.s, %9.s, #25\n"			\
		     "lsr %19.s, %13.s, #25\n"			\
		     "lsl %1.s, %1.s, #7\n"			\
		     "lsl %5.s, %5.s, #7\n"			\
		     "lsl %9.s, %9.s, #7\n"			\
		     "lsl %13.s, %13.s, #7\n"			\
		     "eor %1.d, %1.d, %16.d\n"			\
		     "eor %5.d, %5.d, %17.d\n"			\
		     "eor %9.d, %9.d, %18.d\n"			\
		     "eor %13.d, %13.d, %19.d\n"		\
		     "\n"					\
		     : "+&w" (x_##a), "+&w" (x_##b), "+&w" (x_##c), "+&w" (x_##d), "+&w" (x_##e), "+&w" (x_##f), "+&w" (x_##g), "+&w" (x_##h), \
		       "+&w" (x_##i), "+&w" (x_##j), "+&w" (x_##k), "+&w" (x_##l), "+&w" (x_##m), "+&w" (x_##n), "+&w" (x_##o), "+&w" (x_##p), \
		       "=w" (temp1), "=w" (temp2), "=w" (temp3), "=w" (temp4) : : "p0")

  if (!bytes) return;
uint64_t vc = svcntb(); /* how many bytes in a vector */
if (bytes>=16*vc) {
  u32 in12, in13;
  svuint32_t x_0 = svdup_n_u32(x[0]);
  svuint32_t x_1 = svdup_n_u32(x[1]);
  svuint32_t x_2 = svdup_n_u32(x[2]);
  svuint32_t x_3 = svdup_n_u32(x[3]);
  svuint32_t x_4 = svdup_n_u32(x[4]);
  svuint32_t x_5 = svdup_n_u32(x[5]);
  svuint32_t x_6 = svdup_n_u32(x[6]);
  svuint32_t x_7 = svdup_n_u32(x[7]);
  svuint32_t x_8 = svdup_n_u32(x[8]);
  svuint32_t x_9 = svdup_n_u32(x[9]);
  svuint32_t x_10 = svdup_n_u32(x[10]);
  svuint32_t x_11 = svdup_n_u32(x[11]);
  svuint32_t x_12;// = svdup_n_u32(x[12]); /* useless */
  svuint32_t x_13;// = svdup_n_u32(x[13]); /* useless */
  svuint32_t x_14 = svdup_n_u32(x[14]);
  svuint32_t x_15 = svdup_n_u32(x[15]);
  svuint32_t orig0 = x_0;
  svuint32_t orig1 = x_1;
  svuint32_t orig2 = x_2;
  svuint32_t orig3 = x_3;
  svuint32_t orig4 = x_4;
  svuint32_t orig5 = x_5;
  svuint32_t orig6 = x_6;
  svuint32_t orig7 = x_7;
  svuint32_t orig8 = x_8;
  svuint32_t orig9 = x_9;
  svuint32_t orig10 = x_10;
  svuint32_t orig11 = x_11;
  svuint32_t orig12;// = x_12; /* useless */
  svuint32_t orig13;// = x_13; /* useless */
  svuint32_t orig14 = x_14;
  svuint32_t orig15 = x_15;
  svuint32_t t_0;
  svuint32_t t_1;
  svuint32_t t_2;
  svuint32_t t_3;
  svuint32_t t_4;
  svuint32_t t_5;
  svuint32_t t_6;
  svuint32_t t_7;
  svuint32_t t_8;
  svuint32_t t_9;
  svuint32_t t_10;
  svuint32_t t_11;
  svuint32_t t_12;
  svuint32_t t_13;
  svuint32_t t_14;
  svuint32_t t_15;

  while (bytes >= 16*vc) {
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


    /* svindex() makes it easy to build the input counter */
    const svuint64_t addv13 = svindex_u64(0, 1);
    const svuint64_t addv12 = svadd_n_u64_z(svptrue_b64(), addv13, vc/8);
    svuint64_t t12, t13;
    in12 = x[12];
    in13 = x[13];
    u64 in1213 = ((u64)in12) | (((u64)in13) << 32);
    t12 = svdup_n_u64(in1213);
    t13 = svdup_n_u64(in1213);

    x_12 = svreinterpret_u32_u64(svadd_u64_z(svptrue_b64(), addv12, t12));
    x_13 = svreinterpret_u32_u64(svadd_u64_z(svptrue_b64(), addv13, t13));

    svuint32_t t = x_12;
    x_12 = svuzp1_u32(x_13, x_12);
    x_13 = svuzp2_u32(x_13, t);


    orig12 = x_12;
    orig13 = x_13;

    in1213 += vc/4;
    
    x[12] = in1213 & 0xFFFFFFFF;
    x[13] = (in1213>>32)&0xFFFFFFFF;

    for (i = 0 ; i < ROUNDS ; i+=2) {
	    svuint32_t temp1, temp2, temp3, temp4;
#if 0
      VEC4_QUARTERROUND( 0, 4, 8,12);
      VEC4_QUARTERROUND( 1, 5, 9,13);
      VEC4_QUARTERROUND( 2, 6,10,14);
      VEC4_QUARTERROUND( 3, 7,11,15);
      VEC4_QUARTERROUND( 0, 5,10,15);
      VEC4_QUARTERROUND( 1, 6,11,12);
      VEC4_QUARTERROUND( 2, 7, 8,13);
      VEC4_QUARTERROUND( 3, 4, 9,14);
#else
#if 1
      VEC4_DOUBLEQUARTERROUND_ASM( 0, 4, 8,12, 1, 5, 9,13);
      VEC4_DOUBLEQUARTERROUND_ASM( 2, 6,10,14, 3, 7,11,15);
      VEC4_DOUBLEQUARTERROUND_ASM( 0, 5,10,15, 1, 6,11,12);
      VEC4_DOUBLEQUARTERROUND_ASM( 2, 7, 8,13, 3, 4, 9,14);
#else
      VEC4_QUADQUARTERROUND_ASM( 0, 4, 8,12, 1, 5, 9,13, 2, 6,10,14, 3, 7,11,15);
      VEC4_QUADQUARTERROUND_ASM( 0, 5,10,15, 1, 6,11,12, 2, 7, 8,13, 3, 4, 9,14);
#endif
#endif
    }

#define ONEQUAD_TRANSPOSE(a,b,c,d)                                      \
    {                                                                   \
      svuint32_t t00, t01, t10, t11;                                    \
      svuint64_t t0, t1, t2, t3;                                        \
      x_##a = svadd_u32_z(svptrue_b32(), x_##a, orig##a);                                \
      x_##b = svadd_u32_z(svptrue_b32(), x_##b, orig##b);                                \
      x_##c = svadd_u32_z(svptrue_b32(), x_##c, orig##c);                                \
      x_##d = svadd_u32_z(svptrue_b32(), x_##d, orig##d);                                \
      t00 = svtrn1_u32(x_##a,x_##b);\
      t01 = svtrn2_u32(x_##a,x_##b);\
      t10 = svtrn1_u32(x_##c,x_##d);\
      t11 = svtrn2_u32(x_##c,x_##d);\
      x_##a = svreinterpret_u32_u64(svtrn1_u64(svreinterpret_u64_u32(t00), svreinterpret_u64_u32(t10)));\
      x_##b = svreinterpret_u32_u64(svtrn1_u64(svreinterpret_u64_u32(t01), svreinterpret_u64_u32(t11)));\
      x_##c = svreinterpret_u32_u64(svtrn2_u64(svreinterpret_u64_u32(t00), svreinterpret_u64_u32(t10)));\
      x_##d = svreinterpret_u32_u64(svtrn2_u64(svreinterpret_u64_u32(t01), svreinterpret_u64_u32(t11)));\
      t0 = sveor_u64_z(svptrue_b64(), svreinterpret_u64_u32(x_##a), svld1_gather_s64offset_u64(svptrue_b64(), (uint64_t*)(m+0), gvv));             \
      svst1_scatter_s64offset_u64(svptrue_b64(), (uint64_t*)(out+0), gvv, t0);\
      t1 = sveor_u64_z(svptrue_b64(), svreinterpret_u64_u32(x_##b), svld1_gather_s64offset_u64(svptrue_b64(), (uint64_t*)(m+64), gvv));             \
      svst1_scatter_s64offset_u64(svptrue_b64(), (uint64_t*)(out+64), gvv, t1);\
      t2 = sveor_u64_z(svptrue_b64(), svreinterpret_u64_u32(x_##c), svld1_gather_s64offset_u64(svptrue_b64(), (uint64_t*)(m+128), gvv));             \
      svst1_scatter_s64offset_u64(svptrue_b64(), (uint64_t*)(out+128), gvv, t2);\
      t3 = sveor_u64_z(svptrue_b64(), svreinterpret_u64_u32(x_##d), svld1_gather_s64offset_u64(svptrue_b64(), (uint64_t*)(m+192), gvv));             \
      svst1_scatter_s64offset_u64(svptrue_b64(), (uint64_t*)(out+192), gvv, t3);\
    }

#define ONEQUAD_TRANSPOSE_ASM2(a,b,c,d) { 				\
	svuint32_t temp0, temp1, temp2, temp3;				\
	const u8 *m0 = m, *out0 = out;					\
	const u8 *m64 = m+64, *out64 = out+64;				\
	const u8 *m128 = m+128, *out128 = out+128;			\
	const u8 *m192 = m+192, *out192 = out+192;			\
	asm volatile("ptrue p0.d\n"					\
		     "add %[x_"#a"].s, %[x_"#a"].s, %[orig"#a"].s\n"	\
		     "add %[x_"#b"].s, %[x_"#b"].s, %[orig"#b"].s\n"	\
		     "add %[x_"#c"].s, %[x_"#c"].s, %[orig"#c"].s\n"	\
		     "add %[x_"#d"].s, %[x_"#d"].s, %[orig"#d"].s\n"	\
		     "trn1 %[temp0].s, %[x_"#a"].s, %[x_"#b"].s\n"	\
		     "trn2 %[temp1].s, %[x_"#a"].s, %[x_"#b"].s\n"	\
		     "trn1 %[temp2].s, %[x_"#c"].s, %[x_"#d"].s\n"	\
		     "trn2 %[temp3].s, %[x_"#c"].s, %[x_"#d"].s\n"	\
		     "trn1 %[x_"#a"].d, %[temp0].d, %[temp2].d\n"	\
		     "trn1 %[x_"#b"].d, %[temp1].d, %[temp3].d\n"	\
		     "trn2 %[x_"#c"].d, %[temp0].d, %[temp2].d\n"	\
		     "trn2 %[x_"#d"].d, %[temp1].d, %[temp3].d\n"	\
		     "ld1d { %[temp0].d }, p0/z, [%[m0], %[gvv].d]\n"	\
		     "ld1d { %[temp1].d }, p0/z, [%[m64], %[gvv].d]\n"	\
		     "ld1d { %[temp2].d }, p0/z, [%[m128], %[gvv].d]\n" \
		     "ld1d { %[temp3].d }, p0/z, [%[m192], %[gvv].d]\n" \
		     "eor %[x_"#a"].d, %[x_"#a"].d, %[temp0].d\n"	\
		     "eor %[x_"#b"].d, %[x_"#b"].d, %[temp1].d\n"	\
		     "eor %[x_"#c"].d, %[x_"#c"].d, %[temp2].d\n"	\
		     "eor %[x_"#d"].d, %[x_"#d"].d, %[temp3].d\n"	\
		     "st1d { %[x_"#a"].d }, p0, [%[out0], %[gvv].d]\n"	\
		     "st1d { %[x_"#b"].d }, p0, [%[out64], %[gvv].d]\n" \
		     "st1d { %[x_"#c"].d }, p0, [%[out128], %[gvv].d]\n" \
		     "st1d { %[x_"#d"].d }, p0, [%[out192], %[gvv].d]\n" \
		     : [x_##a] "+w" (x_##a), [x_##b] "+w" (x_##b), [x_##c] "+w" (x_##c), [x_##d] "+w" (x_##d), \
		       [temp0] "=w" (temp0), [temp1] "=w"(temp1), [temp2] "=w" (temp2), [temp3] "=w"(temp3) \
		     : [orig##a] "w" (orig##a), [orig##b] "w" (orig##b), [orig##c] "w" (orig##c), [orig##d] "w" (orig##d), \
		       [m0] "r" ((m0)), [m64] "r" ((m64)), [m128] "r" ((m128)), [m192] "r" ((m192)), \
		       [out0] "r" ((out0)), [out64] "r" ((out64)), [out128] "r" ((out128)), [out192] "r" ((out192)), \
		       [gvv] "w" (gvv)					\
		     : "p0", "memory"					\
		     );							\
    }

#define ONEQUAD_TRANSPOSE_ASM(a,b,c,d) { 				\
	svuint32_t temp0, temp1, temp2, temp3;				\
	asm volatile("ptrue p0.d\n"					\
		     "add %[x_"#a"].s, %[x_"#a"].s, %[orig"#a"].s\n"	\
		     "add %[x_"#b"].s, %[x_"#b"].s, %[orig"#b"].s\n"	\
		     "add x25, %[m0], #64\n"				\
		     "add %[x_"#c"].s, %[x_"#c"].s, %[orig"#c"].s\n"	\
		     "add %[x_"#d"].s, %[x_"#d"].s, %[orig"#d"].s\n"	\
		     "add x30, %[m0], #128\n"				\
		     "trn1 %[temp0].s, %[x_"#a"].s, %[x_"#b"].s\n"	\
		     "trn2 %[temp1].s, %[x_"#a"].s, %[x_"#b"].s\n"	\
		     "add x29, %[m0], #192\n"				\
		     "trn1 %[temp2].s, %[x_"#c"].s, %[x_"#d"].s\n"	\
		     "trn2 %[temp3].s, %[x_"#c"].s, %[x_"#d"].s\n"	\
		     "add x28, %[out0], #64\n"				\
		     "trn1 %[x_"#a"].d, %[temp0].d, %[temp2].d\n"	\
		     "trn1 %[x_"#b"].d, %[temp1].d, %[temp3].d\n"	\
		     "add x27, %[out0], #128\n"				\
		     "trn2 %[x_"#c"].d, %[temp0].d, %[temp2].d\n"	\
		     "trn2 %[x_"#d"].d, %[temp1].d, %[temp3].d\n"	\
		     "add x26, %[out0], #192\n"				\
		     "ld1d { %[temp0].d }, p0/z, [%[m0], %[gvv].d]\n"	\
		     "ld1d { %[temp1].d }, p0/z, [x25, %[gvv].d]\n"	\
		     "ld1d { %[temp2].d }, p0/z, [x30, %[gvv].d]\n"	\
		     "ld1d { %[temp3].d }, p0/z, [x29, %[gvv].d]\n"	\
		     "eor %[x_"#a"].d, %[x_"#a"].d, %[temp0].d\n"	\
		     "eor %[x_"#b"].d, %[x_"#b"].d, %[temp1].d\n"	\
		     "eor %[x_"#c"].d, %[x_"#c"].d, %[temp2].d\n"	\
		     "eor %[x_"#d"].d, %[x_"#d"].d, %[temp3].d\n"	\
		     "st1d { %[x_"#a"].d }, p0, [%[out0], %[gvv].d]\n"	\
		     "st1d { %[x_"#b"].d }, p0, [x28, %[gvv].d]\n"	\
		     "st1d { %[x_"#c"].d }, p0, [x27, %[gvv].d]\n"	\
		     "st1d { %[x_"#d"].d }, p0, [x26, %[gvv].d]\n"	\
		     : [x_##a] "+w" (x_##a), [x_##b] "+w" (x_##b), [x_##c] "+w" (x_##c), [x_##d] "+w" (x_##d), \
		       [temp0] "=w" (temp0), [temp1] "=w"(temp1), [temp2] "=w" (temp2), [temp3] "=w"(temp3) \
		     : [orig##a] "w" (orig##a), [orig##b] "w" (orig##b), [orig##c] "w" (orig##c), [orig##d] "w" (orig##d), \
		       [m0] "r" ((m)), [out0] "r" ((out)),		\
		       [gvv] "w" (gvv)					\
		     : "p0", "memory", "x25", "x30", "x29", "x28", "x27", "x26" \
		     );							\
    }
    
#define ONEQUAD(a,b,c,d) ONEQUAD_TRANSPOSE_ASM(a,b,c,d)

    svint64_t gvv, gvvl, gvvh;
    /* But beware, the range of immediates is small in svindex
     * So need to be a bit careful to construct the vector of gather/scatter indices
     */
    gvvl = svindex_s64(0, 1);
    gvvl = svlsl_n_s64_z(svptrue_b64(), gvvl, 8);
    gvvl = svzip1(gvvl, gvvl);
    gvv = svadd_s64_z(svptrue_b64(), gvvl, svdupq_n_s64(0,8));

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

    bytes -= 16*vc;
    out += 16*vc;
    m += 16*vc;
  }
 }
#undef VEC4_ROT
#undef VEC4_QUARTERROUND
#undef VEC4_QUARTERROUND_INT
#undef VEC4_QUARTERROUND_ASM
#undef VEC4_DOUBLEQUARTERROUND_ASM
#undef VEC4_QUADQUARTERROUND_ASM
