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

void generated_double_fft16(
        /* int n */
        double * in,
        double * out);

#if __riscv_vector_version == 700
#define __builtin_epi_vmv_v_x_2xi32(a,b) __builtin_epi_vbroadcast_2xi32(a,b)
#define __builtin_epi_vfmv_v_f_2xf32(a,b) __builtin_epi_vbroadcast_2xf32(a,b)
#endif
#if __riscv_vector_version == 700
#define __builtin_epi_vmv_v_x_1xi64(a,b) __builtin_epi_vbroadcast_1xi64(a,b)
#define __builtin_epi_vfmv_v_f_1xf64(a,b) __builtin_epi_vbroadcast_1xf64(a,b)
#endif
#if __riscv_vector_version == 700
#define __builtin_epi_vfmv_f_s_1xf64(a) __builtin_epi_vgetfirst_1xf64(a)
#endif
