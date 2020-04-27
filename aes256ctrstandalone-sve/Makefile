SRCs=armsve.c try-anything.c
OBJs=$(SRCs:.c=.o)
SCLIBS=cpucycles.o kernelrandombytes.o

CC=armclang
OPT=-I. -O2 -march=armv8-a+sve

all: aes256ctr aes256ctr_small

clean:
	rm -f $(OBJs) try.o try_small.o armsve.o aes256ctr aes256ctr_small

%.o: %.c
	$(CC) $(OPT) $< -c -o $@

try.o: try.c
	$(CC) $(OPT) $< -c -o $@

try_small.o: try.c
	$(CC) $(OPT) $< -c -o $@ -DSMALL

armsve.o: armsve.c
	$(CC) $(OPT) $< -c -o $@

aes256ctr: $(OBJs) armsve.o try.o
	$(CC) $(OPT) $^ -o $@ $(SCLIBS)

aes256ctr_small: $(OBJs) armsve.o try_small.o
	$(CC) $(OPT) $^ -o $@ $(SCLIBS)
