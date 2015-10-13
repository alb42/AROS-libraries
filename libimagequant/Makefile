VPATH = 
CFLAGS = -O2 -g -fomit-frame-pointer -W -Wall -Wno-parentheses
CC = i386-aros-gcc
LD = i386-aros-gcc
LDFLAGS = -nostartfiles -Wl,-Map -Xlinker linkermap
LIBS = -lgcc -lautoinit -llibinit -static -larosc
STRIP = i386-aros-strip --strip-unneeded --remove-section .comment

OBJS = lib_header.o libimagequant.a

all: imagequant.library

imagequant.library: $(OBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

lib_header.o: lib_header.c lib.h

clean:
	rm -f *.o *.library *.ppu testlibrary linkermap