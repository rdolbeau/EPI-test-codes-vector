SRCS=main.c  n1_16.c  test_fft16.c n1_16_simplified.c
OBJS=$(SRCS:.c=.o)

ARCH=$(shell arch)

ifeq ($(ARCH),x86_64)
CC=icc
CFLAGS=-O3 -mtune=native -march=native -mkl
OBJS+= test_fft16_zgemv_blas.o
LDFLAGS=
MAINFLAGS=-DBLAS
endif

ifeq ($(ARCH),riscv64)
CC=clang
CFLAGS=-O2 -fno-vectorize -mepi #-I/home/dolbeau/blis/include/generic
OBJS+= test_fft16_zgemv_1.o test_fft16_zgemv_2.o test_fft16_zgemv_2b.o test_fft16_zgemv_3.o test_fft16_zgemv_4.o test_fft16_zgemv_5.o\
        test_fft16_zgemv_6.o test_fft16_zgemv_7.o test_fft16_zgemv_8.o test_fft16_zgemv_9.o test_fft16_zgemv_10.o test_fft16_zgemv_11.o\
	test_fft16_zgemv_1T.o test_fft16_zgemv_2T.o test_fft16_zgemv_2bT.o test_fft16_zgemv_3T.o test_fft16_zgemv_4T.o test_fft16_zgemv_5T.o\
        test_fft16_zgemv_6T.o test_fft16_zgemv_7T.o test_fft16_zgemv_8T.o test_fft16_zgemv_9T.o test_fft16_zgemv_10T.o test_fft16_zgemv_11T.o\
	 #test_fft16_zgemv_blas.o
LDFLAGS=#-L/home/dolbeau/blis/lib/generic -Wl,-rpath,/home/dolbeau/blis/lib/generic -lblis
MAINFLAGS=#-DBLAS
endif

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

all: test.exe

main.o: CFLAGS+=$(MAINFLAGS)

test_fft16_zgemv_%.o: test_fft16_zgemv.c
	$(CC) $(CFLAGS) $< -c -o $@

test_fft16_zgemv_blas.o: CFLAGS+=-DBLAS

test_fft16_zgemv_1.o: CFLAGS+=-DRISCV_V_VARIANT1

test_fft16_zgemv_2.o: CFLAGS+=-DRISCV_V_VARIANT2

test_fft16_zgemv_2b.o: CFLAGS+=-DRISCV_V_VARIANT2B

test_fft16_zgemv_3.o: CFLAGS+=-DRISCV_V_VARIANT3

test_fft16_zgemv_4.o: CFLAGS+=-DRISCV_V_VARIANT4

test_fft16_zgemv_5.o: CFLAGS+=-DRISCV_V_VARIANT5

test_fft16_zgemv_6.o: CFLAGS+=-DRISCV_V_VARIANT6

test_fft16_zgemv_7.o: CFLAGS+=-DRISCV_V_VARIANT7

test_fft16_zgemv_8.o: CFLAGS+=-DRISCV_V_VARIANT8

test_fft16_zgemv_9.o: CFLAGS+=-DRISCV_V_VARIANT9

test_fft16_zgemv_10.o: CFLAGS+=-DRISCV_V_VARIANT10

test_fft16_zgemv_11.o: CFLAGS+=-DRISCV_V_VARIANT11


test_fft16_zgemv_1T.o: CFLAGS+=-DRISCV_V_VARIANT1 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_2T.o: CFLAGS+=-DRISCV_V_VARIANT2 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_2bT.o: CFLAGS+=-DRISCV_V_VARIANT2B -DRISCV_V_TRUESEGLS

test_fft16_zgemv_3T.o: CFLAGS+=-DRISCV_V_VARIANT3 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_4T.o: CFLAGS+=-DRISCV_V_VARIANT4 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_5T.o: CFLAGS+=-DRISCV_V_VARIANT5 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_6T.o: CFLAGS+=-DRISCV_V_VARIANT6 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_7T.o: CFLAGS+=-DRISCV_V_VARIANT7 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_8T.o: CFLAGS+=-DRISCV_V_VARIANT8 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_9T.o: CFLAGS+=-DRISCV_V_VARIANT9 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_10T.o: CFLAGS+=-DRISCV_V_VARIANT10 -DRISCV_V_TRUESEGLS

test_fft16_zgemv_11T.o: CFLAGS+=-DRISCV_V_VARIANT11 -DRISCV_V_TRUESEGLS

test.exe: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lm

clean:
	rm -f test*.exe *.o
