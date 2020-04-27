SRCs=api.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=clang
OPT=-I. -O2 -fno-vectorize

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
	$(CC) -mepi $(OPT) $< -S -o $@

chacha.o: chacha.s
	$(CC) -mepi $(OPT) $< -c -o $@

chacha20: $(OBJs) chacha.o try.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)

chacha20_small: $(OBJs) chacha.o try_small.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)
