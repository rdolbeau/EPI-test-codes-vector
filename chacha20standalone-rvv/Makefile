SRCs=api.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=clang
#OPT=-I. -O2 -fno-vectorize #-DUSE_EPI_CUSTOM
OPT=-I. -O3 -march=rv64gcv_zba_zbb_zbc_zbs

all: chacha20 chacha20_small

clean:
	rm -f $(OBJs) try.o try_small.o chacha.o chacha20 chacha20_small chacha20_small_var chacha.s chacha_var.s

%.o: %.c
	$(CC) $(OPT) $< -c -o $@

try.o: try.c
	$(CC) $(OPT) $< -c -o $@

try_small.o: try.c
	$(CC) $(OPT) $< -c -o $@ -DSMALL

chacha.s: chacha.c uX.h
	$(CC) $(OPT) $< -S -o $@

chacha.o: chacha.s
	$(CC) $(OPT) $< -c -o $@

chacha20: $(OBJs) chacha.o try.o  $(SCLIBS)
	$(CC) $(OPT) $^ -o $@

chacha20_small: $(OBJs) chacha.o try_small.o  $(SCLIBS)
	$(CC) $(OPT) $^ -o $@

kernelrandombytes.o: getrandom.c
	$(CC) $(OPT) $< -c -o $@

cpucycles.o: riscv.c
	$(CC) $(OPT) $< -c -o $@
