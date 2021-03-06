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

#ifndef USE_EPI_CUSTOM
//ZIP1 Interleave elements from low halves of two vectors
static inline __epi_1xf64 __builtin_epi_zip1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64 res;
	__epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
	__epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
	__epi_1xi64 hidx = __builtin_epi_vsrl_1xi64(idx, vone, vc); // (0, 0, 1, 1, 2, 2, ...)
	__epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
	__epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
	__epi_1xi64 mask = __builtin_epi_vand_1xi64(idx, vone, vc); // (0, 1, 0, 1, 0, ...)
	__epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64(mask); // idem, as mask
	res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ...
	return res;
}

//ZIP2 Interleave elements from high halves of two vectors
static inline __epi_1xf64 __builtin_epi_zip2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
        __epi_1xf64 res;
        __epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
	__epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
        __epi_1xi64 hidx = __builtin_epi_vsrl_1xi64(idx, vone, vc); // (0, 0, 1, 1, 2, 2, ...)
	hidx = __builtin_epi_vadd_1xi64(hidx, __builtin_epi_vmv_v_x_1xi64(vc/2, vc), vc); // assume vc&1==0
        __epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
        __epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
        __epi_1xi64 mask = __builtin_epi_vand_1xi64(idx, vone, vc); // (0, 1, 0, 1, 0, ...)
	__epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64(mask); // idem, as mask
	res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ...
        return res;
}

//TRN1 Interleave even elements from two vectors
static inline __epi_1xf64 __builtin_epi_trn1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64 res;
	__epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
	__epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
	__epi_1xi64 hidx = __builtin_epi_vsrl_1xi64(idx, vone, vc); // (0, 0, 1, 1, 2, 2, ...)
	hidx = __builtin_epi_vsll_1xi64(hidx, vone, vc); // (0, 0, 2, 2, 4, 4, ...) // vsrl+vsll could be done with & ~1
	__epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
	__epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
	__epi_1xi64 mask = __builtin_epi_vand_1xi64(idx, vone, vc); // (0, 1, 0, 1, 0, ...)
	__epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64(mask); // idem, as mask
	res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ...
	return res;
}

//TRN2 Interleave odd elements from two vectors
static inline __epi_1xf64 __builtin_epi_trn2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
        __epi_1xf64 res;
        __epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
	__epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
        __epi_1xi64 hidx = __builtin_epi_vsrl_1xi64(idx, vone, vc); // (0, 0, 1, 1, 2, 2, ...)
	hidx = __builtin_epi_vsll_1xi64(hidx, vone, vc); // (0, 0, 2, 2, 4, 4, ...) // vsrl+vsll could be done with & ~1
	hidx = __builtin_epi_vadd_1xi64(hidx, vone, vc); // (1, 1, 3, 3, 5, 5, ...)
        __epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
        __epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
        __epi_1xi64 mask = __builtin_epi_vand_1xi64(idx, vone, vc); // (0, 1, 0, 1, 0, ...)
	__epi_1xi1 mmask = __builtin_epi_cast_1xi1_1xi64(mask); // idem, as mask
	res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ... // check registers - 5 ?!?
        return res;
}

//UZP1 Concatenate even elements from two vectors
static inline __epi_1xf64 __builtin_epi_uzp1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
        __epi_1xf64 res;
        __epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
        __epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
        __epi_1xi64 hidx = __builtin_epi_vand_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(((vc/2)-1), vc), vc); // (0, 1, 2, ..., 0, 1, 2, ...)
// assume vc is a power of 2 // might not be necessary, what happens if gathering non-exsitent lanes ?
        hidx = __builtin_epi_vsll_1xi64(hidx, vone, vc); // (0, 2, 4, ..., 0, 2, 4, ...)
        __epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
        __epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
        __epi_1xi1 mmask = __builtin_epi_vmsle_1xi64(__builtin_epi_vmv_v_x_1xi64(vc/2, vc), idx, vc); // (0, 0, 0, ..., 1, 1, 1)
        res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ...
        return res;
}

//UZP2 Concatenate odd elements from two vectors
static inline __epi_1xf64 __builtin_epi_uzp2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
        __epi_1xf64 res;
        __epi_1xi64 idx = __builtin_epi_vid_1xi64(vc); // (0, 1, 2, 3, ...)
        __epi_1xi64 vone = __builtin_epi_vmv_v_x_1xi64(1, vc);
        __epi_1xi64 hidx = __builtin_epi_vand_1xi64(idx, __builtin_epi_vmv_v_x_1xi64(((vc/2)-1), vc), vc); // (0, 1, 2, ..., 0, 1, 2, ...)
// assume vc is a power of 2 // might not be necessary, what happens if gathering non-exsitent lanes ?
        hidx = __builtin_epi_vsll_1xi64(hidx, vone, vc); // (0, 2, 4, ..., 0, 2, 4, ...)
        hidx = __builtin_epi_vadd_1xi64(hidx, vone, vc); // (1, 3, 5, ..., 1, 3, 5, ...)
        __epi_1xf64 v0r = __builtin_epi_vrgather_1xf64(v0, hidx, vc);
        __epi_1xf64 v1r = __builtin_epi_vrgather_1xf64(v1, hidx, vc);
        __epi_1xi1 mmask = __builtin_epi_vmsle_1xi64(__builtin_epi_vmv_v_x_1xi64(vc/2, vc), idx, vc); // (0, 0, 0, ..., 1, 1, 1)
        res = __builtin_epi_vfmerge_1xf64(v0r, v1r, mmask, vc); // should give us what we want ...
        return res;
}
#else // USE_EPI_CUSTOM
//ZIP1 Interleave elements from low halves of two vectors
static inline __epi_1xf64 __builtin_epi_zip1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vzip2_1xf64x2(v0, v1, vc);
	return res.v0;
}

//ZIP2 Interleave elements from high halves of two vectors
static inline __epi_1xf64 __builtin_epi_zip2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vzip2_1xf64x2(v0, v1, vc);
	return res.v1;
}

//TRN1 Interleave even elements from two vectors
static inline __epi_1xf64 __builtin_epi_trn1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vtrn_1xf64x2(v0, v1, vc);
	return res.v0;
}

//TRN2 Interleave odd elements from two vectors
static inline __epi_1xf64 __builtin_epi_trn2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vtrn_1xf64x2(v0, v1, vc);
	return res.v1;
}

//UZP1 Concatenate even elements from two vectors
static inline __epi_1xf64 __builtin_epi_uzp1_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vunzip2_1xf64x2(v0, v1, vc);
	return res.v0;
}

//UZP2 Concatenate odd elements from two vectors
static inline __epi_1xf64 __builtin_epi_uzp2_1xf64(__epi_1xf64 v0, __epi_1xf64 v1, unsigned long int vc) {
	__epi_1xf64x2 res = __builtin_epi_vunzip2_1xf64x2(v0, v1, vc);
	return res.v1;
}
#endif
