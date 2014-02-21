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
	@echo "\nConverting elf to uarm executable"
	$(CONVERTER) $(CONVFLAGS) p1test

p1test: asl.o pcb.o p1test.o
	@echo "\nLinking all modules"
	$(LD) $(LDFLAGS) $(EXECUTABLE) $(CRTSO) $(LIBUARM) asl.o pcb.o p1test.0.1.o

asl.o: asl.c
	@echo "\nCompiling asl module"
	$(CC) $(CFLAGS) asl.o asl.c

pcb.o: pcb.c
	@echo "\nCompiling pcb module"
	$(CC) $(CFLAGS) pcb.o pcb.c

p1test.o: p1test.c
	@echo "\nCompiling p1test main module"
	$(CC) $(CFLAGS) p1test.0.1.o p1test.0.1.c

clean: 
	@echo "\nCleaning object files"
	rm -rf p1test.0.1.o asl.o pcb.o

