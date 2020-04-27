#include "n1_16_simplified.h"
#define R double
#define E double
#define stride int
#define INT int
#define DK(a,b) double a = b;
#define MAKE_VOLATILE_STRIDE(a,b)  b+=0

#define WS(stride, i) (stride * i)

#define FMA(a, b, c) (((a) * (b)) + (c))
#define FMS(a, b, c) (((a) * (b)) - (c))
#define FNMA(a, b, c) (- (((a) * (b)) + (c)))
#define FNMS(a, b, c) ((c) - ((a) * (b)))



void n1_16_simplified(const R * ri, const R * ii, R * ro, R * io, stride is, stride os, INT v, INT ivs, INT ovs)
{
  DK (KP923879532, +0.923879532511286756128183189396788286822416626);
  DK (KP414213562, +0.414213562373095048801688724209698078569671875);
  DK (KP707106781, +0.707106781186547524400844362104849039284835938);
  {
    INT i;
    for (i = v; i > 0;
	 i = i - 1, ri = ri + ivs, ii = ii + ivs, ro = ro + ovs, io =
	 io + ovs, MAKE_VOLATILE_STRIDE (64, is), MAKE_VOLATILE_STRIDE (64,
									os))
      {
	E T7, T1R, T25, TC, TN, T1x, T1H, T1l, Tt, T22, T2h, T1b, T1g, T1E,
	  T1Z;
	E T1D, Te, T1S, T26, TJ, TQ, T1m, T1n, TT, Tm, T1X, T2g, T10, T15,
	  T1B;
	E T1U, T1A, T3, TL, Ty, T1k, T6, T1j, TB, TM, T1, T2, Tw, Tx, T4;
	E T5, Tz, TA, Tp, T1c, T1a, T20, Ts, T17, T1f, T21, Tn, To, T18, T19;
	E Tq, Tr, T1d, T1e, Ta, TP, TF, TO, Td, TR, TI, TS, T8, T9, TD;
	E TE, Tb, Tc, TG, TH, Ti, T11, TZ, T1V, Tl, TW, T14, T1W, Tg, Th;
	E TX, TY, Tj, Tk, T12, T13, Tf, Tu, T2j, T2k, Tv, TK, T2f, T2i, T1T;
	E T27, T24, T28, T1Y, T23, T29, T2d, T2c, T2e, T2a, T2b, TV, T1v, T1p,
	  T1r;
	E T1i, T1q, T1u, T1w, TU, T1o, T16, T1h, T1s, T1t, T1z, T1L, T1J, T1P,
	  T1G;
	E T1K, T1O, T1Q, T1y, T1I, T1C, T1F, T1M, T1N;
	T1 = ri[0];
	T2 = ri[16];
	T3 = T1 + T2;
	TL = T1 - T2;
	Tw = ii[0];
	Tx = ii[16];
	Ty = Tw + Tx;
	T1k = Tw - Tx;
	T4 = ri[8];
	T5 = ri[24];
	T6 = T4 + T5;
	T1j = T4 - T5;
	Tz = ii[8];
	TA = ii[24];
	TB = Tz + TA;
	TM = Tz - TA;
	T7 = T3 + T6;
	T1R = T3 - T6;
	T25 = Ty - TB;
	TC = Ty + TB;
	TN = TL - TM;
	T1x = TL + TM;
	T1H = T1k - T1j;
	T1l = T1j + T1k;
	Tn = ri[30];
	To = ri[14];
	Tp = Tn + To;
	T1c = Tn - To;
	T18 = ii[30];
	T19 = ii[14];
	T1a = T18 - T19;
	T20 = T18 + T19;
	Tq = ri[6];
	Tr = ri[22];
	Ts = Tq + Tr;
	T17 = Tq - Tr;
	T1d = ii[6];
	T1e = ii[22];
	T1f = T1d - T1e;
	T21 = T1d + T1e;
	Tt = Tp + Ts;
	T22 = T20 - T21;
	T2h = T20 + T21;
	T1b = T17 + T1a;
	T1g = T1c - T1f;
	T1E = T1a - T17;
	T1Z = Tp - Ts;
	T1D = T1c + T1f;
	T8 = ri[4];
	T9 = ri[20];
	Ta = T8 + T9;
	TP = T8 - T9;
	TD = ii[4];
	TE = ii[20];
	TF = TD + TE;
	TO = TD - TE;
	Tb = ri[28];
	Tc = ri[12];
	Td = Tb + Tc;
	TR = Tb - Tc;
	TG = ii[28];
	TH = ii[12];
	TI = TG + TH;
	TS = TG - TH;
	Te = Ta + Td;
	T1S = TF - TI;
	T26 = Td - Ta;
	TJ = TF + TI;
	TQ = TO - TP;
	T1m = TR - TS;
	T1n = TP + TO;
	TT = TR + TS;
	Tg = ri[2];
	Th = ri[18];
	Ti = Tg + Th;
	T11 = Tg - Th;
	TX = ii[2];
	TY = ii[18];
	TZ = TX - TY;
	T1V = TX + TY;
	Tj = ri[10];
	Tk = ri[26];
	Tl = Tj + Tk;
	TW = Tj - Tk;
	T12 = ii[10];
	T13 = ii[26];
	T14 = T12 - T13;
	T1W = T12 + T13;
	Tm = Ti + Tl;
	T1X = T1V - T1W;
	T2g = T1V + T1W;
	T10 = TW + TZ;
	T15 = T11 - T14;
	T1B = TZ - TW;
	T1U = Ti - Tl;
	T1A = T11 + T14;
	Tf = T7 + Te;
	Tu = Tm + Tt;
	ro[16] = Tf - Tu;
	ro[0] = Tf + Tu;
	T2j = TC + TJ;
	T2k = T2g + T2h;
	io[16] = T2j - T2k;
	io[0] = T2j + T2k;
	Tv = Tt - Tm;
	TK = TC - TJ;
	io[8] = Tv + TK;
	io[24] = TK - Tv;
	T2f = T7 - Te;
	T2i = T2g - T2h;
	ro[24] = T2f - T2i;
	ro[8] = T2f + T2i;
	T1T = T1R + T1S;
	T27 = T25 - T26;
	T1Y = T1U + T1X;
	T23 = T1Z - T22;
	T24 = T1Y + T23;
	T28 = T23 - T1Y;
	ro[20] = FNMS (KP707106781, T24, T1T);
	io[12] = FMA (KP707106781, T28, T27);
	ro[4] = FMA (KP707106781, T24, T1T);
	io[28] = FNMS (KP707106781, T28, T27);
	T29 = T1R - T1S;
	T2d = T26 + T25;
	T2a = T1X - T1U;
	T2b = T1Z + T22;
	T2c = T2a - T2b;
	T2e = T2a + T2b;
	ro[28] = FNMS (KP707106781, T2c, T29);
	io[4] = FMA (KP707106781, T2e, T2d);
	ro[12] = FMA (KP707106781, T2c, T29);
	io[20] = FNMS (KP707106781, T2e, T2d);
	TU = TQ - TT;
	TV = FMA (KP707106781, TU, TN);
	T1v = FNMS (KP707106781, TU, TN);
	T1o = T1m - T1n;
	T1p = FNMS (KP707106781, T1o, T1l);
	T1r = FMA (KP707106781, T1o, T1l);
	T16 = FMA (KP414213562, T15, T10);
	T1h = FNMS (KP414213562, T1g, T1b);
	T1i = T16 - T1h;
	T1q = T16 + T1h;
	T1s = FMA (KP414213562, T1b, T1g);
	T1t = FNMS (KP414213562, T10, T15);
	T1u = T1s - T1t;
	T1w = T1t + T1s;
	ro[22] = FNMS (KP923879532, T1i, TV);
	io[22] = FNMS (KP923879532, T1u, T1r);
	ro[6] = FMA (KP923879532, T1i, TV);
	io[6] = FMA (KP923879532, T1u, T1r);
	io[14] = FNMS (KP923879532, T1q, T1p);
	ro[14] = FNMS (KP923879532, T1w, T1v);
	io[30] = FMA (KP923879532, T1q, T1p);
	ro[30] = FMA (KP923879532, T1w, T1v);
	T1y = T1n + T1m;
	T1z = FMA (KP707106781, T1y, T1x);
	T1L = FNMS (KP707106781, T1y, T1x);
	T1I = TQ + TT;
	T1J = FNMS (KP707106781, T1I, T1H);
	T1P = FMA (KP707106781, T1I, T1H);
	T1C = FMA (KP414213562, T1B, T1A);
	T1F = FNMS (KP414213562, T1E, T1D);
	T1G = T1C + T1F;
	T1K = T1F - T1C;
	T1M = FNMS (KP414213562, T1A, T1B);
	T1N = FMA (KP414213562, T1D, T1E);
	T1O = T1M - T1N;
	T1Q = T1M + T1N;
	ro[18] = FNMS (KP923879532, T1G, T1z);
	io[18] = FNMS (KP923879532, T1Q, T1P);
	ro[2] = FMA (KP923879532, T1G, T1z);
	io[2] = FMA (KP923879532, T1Q, T1P);
	io[26] = FNMS (KP923879532, T1K, T1J);
	ro[26] = FNMS (KP923879532, T1O, T1L);
	io[10] = FMA (KP923879532, T1K, T1J);
	ro[10] = FMA (KP923879532, T1O, T1L);
      }
  }
}
