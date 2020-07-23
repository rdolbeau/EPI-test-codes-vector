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

void gemv_double_fft16_blas(double * in, double * out, int v, int ivs, int ovs);

void gemv_double_fft16_1(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_2(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_2b(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_3(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_4(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_5(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_6(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_7(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_8(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_9(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_10(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_11(double * in, double * out, int v, int ivs, int ovs);


void gemv_double_fft16_1T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_2T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_2bT(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_3T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_4T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_5T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_6T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_7T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_8T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_9T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_10T(double * in, double * out, int v, int ivs, int ovs);
void gemv_double_fft16_11T(double * in, double * out, int v, int ivs, int ovs);

