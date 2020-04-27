SRCs=core.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=clang
OPT=-I. -O2 -fno-vectorize

all: aes256encrypt aes256encrypt_small

clean:
	rm -f $(OBJs) try.o try_small.o core.o aes256encrypt aes256encrypt_small

%.o: %.c
	$(CC) $(OPT) $< -c -o $@

try.o: try.c
	$(CC) $(OPT) $< -c -o $@

try_small.o: try.c
	$(CC) $(OPT) $< -c -o $@ -DSMALL

core.o: core.c
	$(CC) -mepi $(OPT) $< -c -o $@

aes256encrypt: $(OBJs) core.o try.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)

aes256encrypt_small: $(OBJs) core.o try_small.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)
