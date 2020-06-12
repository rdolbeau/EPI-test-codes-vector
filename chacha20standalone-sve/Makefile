SRCs=api.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=armclang
OPT=-I. -O3 -march=armv8-a+sve+crypto

all: chacha20 chacha20_small

clean:
	rm -f $(OBJs) try.o try_small.o chacha.o chacha20 chacha20_small

%.o: %.c
	$(CC) $(OPT) $< -c -o $@

try.o: try.c
	$(CC) $(OPT) $< -c -o $@

try_small.o: try.c
	$(CC) $(OPT) $< -c -o $@ -DSMALL

chacha.o: chacha.c uX.h
	$(CC) $(OPT) $< -c -o $@

chacha20: $(OBJs) chacha.o try.o
	$(CC) $(OPT) -static $^ -o $@ $(SCLIBS)

chacha20_small: $(OBJs) chacha.o try_small.o
	$(CC) $(OPT) -static $^ -o $@ $(SCLIBS)

