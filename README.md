# EPI-test-codes-vector

Some test codes for vector capabilities targeted in EPI (SVE, RISC-V V)

# Content

## Cryptographic

Cryptographic codes here are 'standalone' versions of implementations designed for the Supercop benchmark <https://bench.cr.yp.to/supercop.html>, and include some of the Supercop source code. They also include some objects file generated from Supercop (cpucycles.o, kernelrandombytes.o) to simplify usage.

All codes currently use ACLE (Arm C Language Extension) for SVE, and EPI Builtins for V (<https://repo.hca.bsc.es/gitlab/rferrer/epi-builtins-ref/>).

* aes256ctrstandalone-rvv: AES-256 in counter mode, for RISC-V with V extension. This doesn't use any AES-specific instructions, but vectorize a scalar AES-256 (specifically, the implementation from `supercop/crypto_core/aes256encrypt/dolbeau/std-4ft`).

* aes256ctrstandalone-sve: same as above, but for Arm SVE.

* aes256encryptstandalone-rvv: AES-256 encryption vectorized per-block for RISC-V with V extension, same idea as aes256ctrstandalone-rvv above minus the counter mode.

* chacha20standalone-rvv: Chacha20 vectorized for RISC-V with V extension, basically the RISC-V version of the SVE version below.

* chacha20standalone-sve: Chacha20 vectorized for Arm SVE. This is a standalone version of `supercop/crypto_stream/chacha20/dolbeau/arm-sve`.

The first part of the output (an hexadecimal string) should match the ckechsum from the benchmark, to for instance the output of chacha20_small should match the content of `supercop/crypto_stream/chacha20/checksumsmall`.

## Other

* simplefft16-rvv: 16-points in-register double-precision DFT/FFT (so requires a 1024+ bits vector that can fit 16 double-precision values). Multiple implementations of the algorithm, both as DFT and FFT.

* FFTW3 can be found in https://github.com/rdolbeau/fftw3, see the wiki there for the relevant branches.

# Acknowledgements

This work has partly been done as part of the European Processor Initiative project.

The European Processor Initiative (EPI) (FPA: 800928) has received funding from the European Union's Horizon 2020 research and innovation programme under grant agreement EPI-SGA1: 826647
