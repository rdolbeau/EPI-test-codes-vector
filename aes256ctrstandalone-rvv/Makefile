SRCs=riscvv.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=clang
OPT=-I. -O2 -fno-vectorize

all: aes256ctr aes256ctr_small

clean:
	rm -f $(OBJs) try.o try_small.o riscvv.o aes256ctr aes256ctr_small

%.o: %.c
	$(CC) $(OPT) $< -c -o $@

try.o: try.c
	$(CC) $(OPT) $< -c -o $@

try_small.o: try.c
	$(CC) $(OPT) $< -c -o $@ -DSMALL

riscvv.o: riscvv.c
	$(CC) -mepi $(OPT) $< -c -o $@

aes256ctr: $(OBJs) riscvv.o try.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)

aes256ctr_small: $(OBJs) riscvv.o try_small.o
	$(CC) -mepi $(OPT) $^ -o $@ $(SCLIBS)
