VPATH = 
CFLAGS = -O2 -g -fomit-frame-pointer -W -Wall -Wno-parentheses
CC = i386-aros-gcc
LD = i386-aros-gcc
LDFLAGS = -nostartfiles -Wl,-Map -Xlinker linkermap
LIBS = -lsqlite3 -lgcc -lautoinit -llibinit -static -larosc
STRIP = i386-aros-strip --strip-unneeded --remove-section .comment

MYOBJS = lib_header.o

all: sqlite3.library testsqlite3

sqlite3.library: $(MYOBJS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

lib_header.o: lib_header.c lib.h

testsqlite3:
	fpc testsqlite3.pas

clean:
	rm -f *.o *.ppu *.library linkermap testsqlite3


