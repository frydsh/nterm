
NTERM_OBJECTS = nterm.o telnet.o decode.o wrapsock.o
ZMODEM_OBJECTS=rzsz/lsz.o rzsz/zreadline.o rzsz/canit.o rzsz/zm.o rzsz/crctab.o rzsz/rbsb.o rzsz/lrz.o \
		rzsz/zmodem.o
CFLAGS=-g -O2 -I/usr/include/cdk
LDFLAGS=-lncurses -lcdk -lconfig
nterm:$(NTERM_OBJECTS) $(ZMODEM_OBJECTS)
	gcc $(CFLAGS) -o nterm $(NTERM_OBJECTS) $(ZMODEM_OBJECTS) $(LDFLAGS)

nterm.o:nterm.h 
telnet.o:nterm.h telnet.h
decode.o:nterm.h decode.h

TAGS:nterm.c nterm.h telnet.h telnet.c decode.c
	etags *.{c,h}
clean:
	rm -rf *.o  nterm TAGS rzsz/*.o 
