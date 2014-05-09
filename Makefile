# Makefile
srcdir = src/
objectdir = build/

CC = arm-none-eabi-gcc
CFLAGS = -mcpu=arm7tdmi -c -o
LD = arm-none-eabi-ld
EXECUTABLE = bin/p1test
CRTSO = /usr/include/uarm/crtso.o
LIBUARM = /usr/include/uarm/libuarm.o
LDFLAGS = -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x  -o
CONVERTER = elf2uarm
CONVFLAGS = -k

OBJECTS = $(objectdir)asl.o $(objectdir)pcb.o $(objectdir)p1test.0.1.o
SOURCES = $(srcdir)asl.c $(srcdir)pcb.c $(srcdir)p1test.0.1.c


all: kernel.core.uarm clean

kernel.core.uarm: p1test
	@echo "\nConverting elf to uarm executable............."
	$(CONVERTER) $(CONVFLAGS) $(EXECUTABLE)

p1test: .c.o
	@echo "\nLinking all modules............."
	$(LD) $(LDFLAGS) $(EXECUTABLE) $(CRTSO) $(LIBUARM) $(OBJECTS)

$(objectdir)%.o: %.c
	@echo "\nCompiling all modules............"
	$(CC) -c $(CFLAGS) $(srcdir)$< -o $@

clean: 
	@echo "\nCleaning object files............."
	rm -rf $(EXECUTABLE) 

