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

static void n1fr5v_16(const R * ri, const R * ii, R * ro, R * io, stride is, stride os, INT v, INT ivs, INT ovs) {
	DK (KP382683432, +0.382683432365089771728459984030398866761344562);
	DK (KP923879532, +0.923879532511286756128183189396788286822416626);
	DK (KP707106781, +0.707106781186547524400844362104849039284835938);

	INT i, j;
	 // 1.00-0.00i  0.92-0.38i  0.71-0.71i  0.38-0.92i  0.00-1.00i -0.38-0.92i -0.71-0.71i -0.92-0.38i -1.00-0.00i -0.92+0.38i -0.71+0.71i -0.38+0.92i -0.00+1.00i  0.38+0.92i  0.71+0.71i  0.92+0.38i
	 //
	static const R mst[32] = {	1., 0.,
			KP923879532, -KP382683432,
			KP707106781, -KP707106781,
			KP382683432, -KP923879532,
			0., -1.,
			-KP382683432, -KP923879532,
			-KP707106781, -KP707106781,
			-KP923879532, -KP382683432,
			-1., 0.,
			-KP923879532, KP382683432,
			-KP707106781, KP707106781,
			-KP382683432, KP923879532,
			0., 1.,
			KP382683432, KP923879532,
			KP707106781, KP707106781,
			KP923879532, KP382683432 };


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

        __epi_1xf64 voutr = __builtin_epi_vfmv_v_f_1xf64(ri[0], 16);
        __epi_1xf64 vouti = __builtin_epi_vfmv_v_f_1xf64(ri[1], 16);
        __epi_1xf64 vinr , vini;
        __epi_1xf64 vmr, vmi;
#if 0
        fake_vlseg2e(ri, vinr, vini, 16);
#endif
        for (i = 1 ; i < 16 ; i++) {
                __epi_1xi64 idx = __builtin_epi_vid_1xi64(16); // 0, 1, 2, ...
                __epi_1xi64 v255 = __builtin_epi_vmv_v_x_1xi64(255, 16); // 255, 255, 255, ...
                __epi_1xi64 idx16 = __builtin_epi_vmul_1xi64(idx,__builtin_epi_vmv_v_x_1xi64(2*sizeof(double),16),16);
                __epi_1xi64 voi = __builtin_epi_vmv_v_x_1xi64(i, 16); // i, i, i, ...
                __epi_1xi64 vtr = __builtin_epi_vmul_1xi64(idx16, voi, 16); // 0, 16*i, 32*i, 48*i, ...
                __epi_1xi64 vidxr = __builtin_epi_vand_1xi64(vtr, v255, 16); // vtr % 255
                vmr = __builtin_epi_vload_indexed_1xf64(mst, vidxr, 16);
                vmi = __builtin_epi_vload_indexed_1xf64(mst + 1, vidxr, 16);

#if 0
                __epi_1xf64 vibr = __builtin_epi_vsplat_1xf64(vinr, i, 16); // not supported in vehave
                __epi_1xf64 vibi = __builtin_epi_vsplat_1xf64(vini, i, 16);
#else
                __epi_1xf64 vibr = __builtin_epi_vfmv_v_f_1xf64(ri[2*i], 16); // is it costlier than dense load + splat ?
                __epi_1xf64 vibi = __builtin_epi_vfmv_v_f_1xf64(ri[2*i+1], 16); // splat & broadcast are both vrgather anyway ...
#endif
                __epi_1xf64 voo = __builtin_epi_vfmul_1xf64(vibi, vmi, 16);
                __epi_1xf64 veemoo = __builtin_epi_vfmsac_1xf64(voo, vibr, vmr, 16);
                __epi_1xf64 veo = __builtin_epi_vfmul_1xf64(vibr, vmi, 16);
                __epi_1xf64 veomoe = __builtin_epi_vfmacc_1xf64(veo, vibi, vmr, 16);

                voutr = __builtin_epi_vfadd_1xf64(voutr, veemoo, 16);
                vouti = __builtin_epi_vfadd_1xf64(vouti, veomoe, 16);
        }
        fake_vsseg2e(ro, voutr, vouti, 16);
	} // loop on #transform 'v'
}

static const kdft_desc desc = { 16, "n1fr5v_16", { 17, 2, 1, 0 }, &GENUS, 2, 2, 0, 0 };

void X(codelet_n1fr5v_16)(planner *p) { X(kdft_register) (p, n1fr5v_16, &desc); }

