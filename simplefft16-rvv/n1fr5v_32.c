/*

RISC-V V DFT/FFT demo code.

Copyright ? 2020 Romain Dolbeau <Romain.Dolbeau@sipearl.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "dft/codelet-dft.h"

#include "dft/scalar/n.h"

#include "vf64.h"

static void n1fr5v_32(const R * ri, const R * ii, R * ro, R * io, stride is, stride os, INT v, INT ivs, INT ovs) {
	DK(KP831469612, +0.831469612302545237078788377617905756738560812);
	DK(KP555570233, +0.555570233019602224742830813948532874374937191);
	DK(KP195090322, +0.195090322016128267848284868477022240927691618);
	DK(KP980785280, +0.980785280403230449126182236134239036973933731);
	DK(KP923879532, +0.923879532511286756128183189396788286822416626);
	DK(KP382683432, +0.382683432365089771728459984030398866761344562);
	DK(KP707106781, +0.707106781186547524400844362104849039284835938);

	INT i, j;
	/*	1.00-0.00i  0.98-0.20i  0.92-0.38i  0.83-0.56i 
		0.71-0.71i  0.56-0.83i  0.38-0.92i  0.20-0.98i
		0.00-1.00i -0.20-0.98i -0.38-0.92i -0.56-0.83i
		-0.71-0.71i -0.83-0.56i -0.92-0.38i -0.98-0.20i
		-1.00-0.00i -0.98+0.20i -0.92+0.38i -0.83+0.56i
		-0.71+0.71i -0.56+0.83i -0.38+0.92i -0.20+0.98i
		-0.00+1.00i  0.20+0.98i  0.38+0.92i  0.56+0.83i
		0.71+0.71i  0.83+0.56i  0.92+0.38i  0.98+0.20i
	*/
	static const R mst[64] = {
			1., 0.,
			KP980785280, -KP195090322,
			KP923879532, -KP382683432,
			KP831469612, -KP555570233,
			KP707106781, -KP707106781,
			KP555570233, -KP831469612,
			KP382683432, -KP923879532,
			KP195090322, -KP980785280,
			0., -1.,
			-KP195090322, -KP980785280,
			-KP382683432, -KP923879532,
			-KP555570233, -KP831469612,
			-KP707106781, -KP707106781,
			-KP831469612, -KP555570233,
			-KP923879532, -KP382683432,
			-KP980785280, -KP195090322,
			-1., 0.,
			-KP980785280, KP195090322,
			-KP923879532, KP382683432,
			-KP831469612, KP555570233,
			-KP707106781, KP707106781,
			-KP555570233, KP831469612,
			-KP382683432, KP923879532,
			-KP195090322, KP980785280,
			0., 1.,
			KP195090322, KP980785280,
			KP382683432, KP923879532,
			KP555570233, KP831469612,
			KP707106781, KP707106781,
			KP831469612, KP555570233,
			KP923879532, KP382683432,
			KP980785280, KP195090322
			};


	for (j = 0 ; j < v ; j++, ri += ivs, ro += ovs) {

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

        __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64(0., 32);
        __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64(0., 32);
        __epi_1xf64 vinr, vini;
        __epi_1xf64 vmr, vmi;
#if 0
        fake_vlseg2e(ri, vinr, vini, 32);
#endif
        for (i = 0 ; i < 32 ; i++) {
                __epi_1xi64 idx = __builtin_epi_vid_1xi64(32); // 0, 1, 2, ...
                __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64(255, 32); // 255, 255, 255, ...
                __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64(idx,__builtin_epi_vmv_v_x_1xi64(2*sizeof(double),32),32);
                __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64(i, 32); // i, i, i, ...
                __epi_1xi64 vtr = __builtin_epi_vmul_1xi64(idx16, voi, 32); // 0, 32*i, ...
                __epi_1xi64 vidxr = __builtin_epi_vand_1xi64(vtr, v255, 32); // vtr % 255
                vmr = __builtin_epi_vload_indexed_1xf64(mst, vidxr, 32);
                vmi = __builtin_epi_vload_indexed_1xf64(mst + 1, vidxr, 32);

#if 0
                __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64(vinr, i, 32); // not supported in vehave
                __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64(vini, i, 32);
#else
                __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64(ri[2*i], 32); // is it costlier than dense load + splat ?
                __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64(ri[2*i+1], 32); // splat & broadcast are both vrgather anyway ...
#endif
                __epi_1xf64 voo = __builtin_epi_vfmul_1xf64(vibi, vmi, 32);
                __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64(voo, vibr, vmr, 32);
                __epi_1xf64 veo = __builtin_epi_vfmul_1xf64(vibr, vmi, 32);
                __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64(veo, vibi, vmr, 32);

                voutr = __builtin_epi_vfadd_1xf64(voutr, veemoo, 32);
                vouti = __builtin_epi_vfadd_1xf64(vouti, veomoe, 32);
        }
        fake_vsseg2e(ro, voutr, vouti, 32);
	} // loop on #transform 'v'
}

static const kdft_desc desc = { 32, "n1fr5v_32", { 17, 2, 1, 0 }, &GENUS, 2, 2, 0, 0 };

void X(codelet_n1fr5v_32)(planner *p) { X(kdft_register) (p, n1fr5v_32, &desc); }

