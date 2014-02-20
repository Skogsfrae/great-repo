# Makefile

CC = arm-none-eabi-gcc
CFLAGS = -mcpu=arm7tdmi -c -o
LD = arm-none-eabi-ld
EXECUTABLE = p1test
LDFLAGS = -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x  -o  /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o

all: p1test

p1test: c/asl.o c/pcb.o p1test.o
	$(LD) $(LDFLAGS) c/asl.o c/pcb.o p1test.o $(EXECUTABLE)

asl.o: c/asl.c
	$(CC) $(CFLAGS) c/asl.o c/asl.c

pcb.o: c/pcb.c
	$(CC) $(CFLAGS) c/pcb.o c/pcb.c

p1test.o: p1test.c
	$(CC) $(CFLAGS) p1test.o p1test.c
