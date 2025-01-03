C = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o :
    $(CC) -c $(CFLAGS) $<

ALL = sig sync ftpc ftps

all: $(ALL)

sig: sig.o
    $(CC) -o $@ $< $(LDFLAGS)

sync: sync.o
    $(CC) -o $@ $< $(LDFLAGS)

ftpc: ftpc.o
    $(CC) -o $@ $< $(LDFLAGS)

ftps: ftps.o
    $(CC) -o $@ $< $(LDFLAGS)

clean :
    rm -rf *.o $(ALL)