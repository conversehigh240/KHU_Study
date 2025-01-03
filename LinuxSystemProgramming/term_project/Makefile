C = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o :
    $(CC) -c $(CFLAGS) $<

ALL = chats chatc chats_hw2 chatc_hw3

all: $(ALL)

chats: chats.o
    $(CC) -o $@ $< $(LDFLAGS)

chatc: chatc.o
    $(CC) -o $@ $< $(LDFLAGS)

chats_hw2: chats_hw2.o
    $(CC) -o $@ $< $(LDFLAGS)

chatc_hw3: chatc_hw3.o
    $(CC) -o $@ $< $(LDFLAGS)

clean :
    rm -rf *.o $(ALL)