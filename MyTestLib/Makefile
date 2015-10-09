VPATH = 
CFLAGS = -O2 -g -fomit-frame-pointer -I$(FT2_INCLUDE) -W -Wall -Wno-parentheses
CC = i386-aros-gcc
LD = i386-aros-gcc
LDFLAGS = -nostartfiles -Wl,-Map -Xlinker linkermap
LIBS = 
STRIP = i386-aros-strip --strip-unneeded --remove-section .comment

FT2_INCLUDE = 
FTOBJS = 
TTOBJS = lib_header.o 
CRHOBJS =

all: mytest.library

mytest.library: $(FTOBJS) $(TTOBJS) $(CRHOBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

lib_header.o: lib_header.c lib.h

clean:
	rm -f *.o *.library

