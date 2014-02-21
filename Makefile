# Makefile

CC = arm-none-eabi-gcc
CFLAGS = -mcpu=arm7tdmi -c -o
LD = arm-none-eabi-ld
EXECUTABLE = p1test
CRTSO = /usr/include/uarm/crtso.o
LIBUARM = /usr/include/uarm/libuarm.o
LDFLAGS = -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x  -o
CONVERTER = elf2uarm
CONVFLAGS = -k

all: kernel.core.uarm clean

kernel.core.uarm: p1test
	$(CONVERTER) $(CONVFLAGS) p1test

p1test: asl.o pcb.o p1test.o
	$(LD) $(LDFLAGS) $(EXECUTABLE) $(CRTSO) $(LIBUARM) c/asl.o c/pcb.o p1test.0.1.o

asl.o: c/asl.c
	$(CC) $(CFLAGS) c/asl.o c/asl.c

pcb.o: c/pcb.c
	$(CC) $(CFLAGS) c/pcb.o c/pcb.c

p1test.o: p1test.c
	$(CC) $(CFLAGS) p1test.0.1.o p1test.0.1.c

clean: 
	rm -r -f p1test.0.1.o c/asl.o c/pcb.o

