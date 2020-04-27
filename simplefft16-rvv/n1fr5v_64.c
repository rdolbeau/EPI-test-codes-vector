#include "dft/codelet-dft.h"

#include "dft/scalar/n.h"

#include "vf64.h"

static void n1fr5v_64(const R * ri, const R * ii, R * ro, R * io, stride is, stride os, INT v, INT ivs, INT ovs) {
DK(KP098017140, +0.098017140329560601994195563888641845861136673);
DK(KP195090322, +0.195090322016128267848284868477022240927691618);
DK(KP290284677, +0.290284677254462367636192375817395274691476278);
DK(KP382683432, +0.382683432365089771728459984030398866761344562);
DK(KP471396737, +0.471396736825997648556387625905254377657460319);
DK(KP555570233, +0.555570233019602224742830813948532874374937191);
DK(KP634393284, +0.634393284163645498215171613225493370675687095);
DK(KP707106781, +0.707106781186547524400844362104849039284835938);
DK(KP773010453, +0.773010453362736960810906609758469800971041293);
DK(KP831469612, +0.831469612302545237078788377617905756738560812);
DK(KP881921264, +0.881921264348355029712756863660388349508442621);
DK(KP923879533, +0.923879532511286756128183189396788286822416626);
DK(KP956940336, +0.956940335732208864935797886980269969482849206);
DK(KP980785280, +0.980785280403230449126182236134239036973933731);
DK(KP995184727, +0.995184726672196886244836953109479921575474869);

	INT i, j;
	static const R mst[128] = {
1., 0.,
KP995184727, -KP098017140,
KP980785280, -KP195090322,
KP956940336, -KP290284677,
KP923879533, -KP382683432,
KP881921264, -KP471396737,
KP831469612, -KP555570233,
KP773010453, -KP634393284,
KP707106781, -KP707106781,
KP634393284, -KP773010453,
KP555570233, -KP831469612,
KP471396737, -KP881921264,
KP382683432, -KP923879533,
KP290284677, -KP956940336,
KP195090322, -KP980785280,
KP098017140, -KP995184727,
0., -1.,
-KP098017140, -KP995184727,
-KP195090322, -KP980785280,
-KP290284677, -KP956940336,
-KP382683432, -KP923879533,
-KP471396737, -KP881921264,
-KP555570233, -KP831469612,
-KP634393284, -KP773010453,
-KP707106781, -KP707106781,
-KP773010453, -KP634393284,
-KP831469612, -KP555570233,
-KP881921264, -KP471396737,
-KP923879533, -KP382683432,
-KP956940336, -KP290284677,
-KP980785280, -KP195090322,
-KP995184727, -KP098017140,
-1., 0.,
-KP995184727, KP098017140,
-KP980785280, KP195090322,
-KP956940336, KP290284677,
-KP923879533, KP382683432,
-KP881921264, KP471396737,
-KP831469612, KP555570233,
-KP773010453, KP634393284,
-KP707106781, KP707106781,
-KP634393284, KP773010453,
-KP555570233, KP831469612,
-KP471396737, KP881921264,
-KP382683432, KP923879533,
-KP290284677, KP956940336,
-KP195090322, KP980785280,
-KP098017140, KP995184727,
0., 1.,
KP098017140, KP995184727,
KP195090322, KP980785280,
KP290284677, KP956940336,
KP382683432, KP923879533,
KP471396737, KP881921264,
KP555570233, KP831469612,
KP634393284, KP773010453,
KP707106781, KP707106781,
KP773010453, KP634393284,
KP831469612, KP555570233,
KP881921264, KP471396737,
KP923879533, KP382683432,
KP956940336, KP290284677,
KP980785280, KP195090322,
KP995184727, KP098017140
			};


	for (j = 0 ; j < v ; j++, ri += ivs, ii += ivs, ro += ovs) {

#define fake_vlseg2e(addr, o1, o2, vc)					\
	do {								\
		__epi_1xf64 l1 = __builtin_epi_vload_1xf64((addr), vc);	\
		__epi_1xf64 l2 = __builtin_epi_vload_1xf64((addr) + vc, vc); \
		o1 = __builtin_epi_uzp1_1xf64(l1, l2, vc);		\
		o2 = __builtin_epi_uzp2_1xf64(l1, l2, vc);		\
	} while (0)
	

#define fake_vsseg2e(addr, o1, o2, vc)					\
	do {								\
		__epi_1xf64 l1 = __builtin_epi_zip1_1xf64(o1, o2, vc);	\
		__epi_1xf64 l2 = __builtin_epi_zip2_1xf64(o1, o2, vc);	\
		__builtin_epi_vstore_1xf64((addr), l1, vc);		\
		__builtin_epi_vstore_1xf64((addr) + vc, l2, vc);	\
	} while (0)

#define SIZE 64
#define MASK (SIZE*16-1)

        __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64(ri[0], SIZE);
        __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64(ii[0], SIZE);
        __epi_1xf64 vinr, vini;
        __epi_1xf64 vmr, vmi;
        const __epi_1xi64 idx = __builtin_epi_vid_1xi64(SIZE); // 0, 1, 2, ...
        const __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64(MASK, SIZE); // MASK, ...
        const __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64(idx,__builtin_epi_vmv_v_x_1xi64(2*sizeof(double),SIZE),SIZE);

	fake_vlseg2e(mst, vmr, vmi, SIZE); // iter 1: dense mst

        __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64(ri[is], SIZE);
        __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64(ii[is], SIZE);

        __epi_1xf64 voo = __builtin_epi_vfmsac_1xf64(voutr, vibi, vmi, SIZE); // -*- -> + for voutr
        __epi_1xf64 veo = __builtin_epi_vfmacc_1xf64(vouti, vibr, vmi, SIZE);
        voutr = __builtin_epi_vfmsac_1xf64(voo, vibr, vmr, SIZE);
        vouti = __builtin_epi_vfmacc_1xf64(veo, vibi, vmr, SIZE);

	for (i = 2 ; i < SIZE ; i++) {
                __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64(i, SIZE); // i, i, i, ...
                __epi_1xi64 vtr = __builtin_epi_vmul_1xi64(idx16, voi, SIZE); // 0, SIZE*i, ...
                __epi_1xi64 vidxr = __builtin_epi_vand_1xi64(vtr, v255, SIZE); // vtr % (MASK+1)
                vmr = __builtin_epi_vload_indexed_1xf64(mst, vidxr, SIZE);
                vmi = __builtin_epi_vload_indexed_1xf64(mst + 1, vidxr, SIZE);

                vibr = __builtin_epi_vfmv_v_f_1xf64(ri[is*i], SIZE);
                vibi = __builtin_epi_vfmv_v_f_1xf64(ii[is*i], SIZE);

                voo = __builtin_epi_vfmsac_1xf64(voutr, vibi, vmi, SIZE); // -*- -> + for voutr
                veo = __builtin_epi_vfmacc_1xf64(vouti, vibr, vmi, SIZE);
                voutr = __builtin_epi_vfmsac_1xf64(voo, vibr, vmr, SIZE);
                vouti = __builtin_epi_vfmacc_1xf64(veo, vibi, vmr, SIZE);
        }
        fake_vsseg2e(ro, voutr, vouti, SIZE);
	} // loop on #transform 'v'
}

static const kdft_desc desc = { 64, "n1fr5v_64", { 17, 2, 1, 0 }, &GENUS, 0, 2, 0, 0 };

void X(codelet_n1fr5v_64)(planner *p) { X(kdft_register) (p, n1fr5v_64, &desc); }

