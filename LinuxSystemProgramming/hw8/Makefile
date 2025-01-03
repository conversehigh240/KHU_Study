CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o :
    $(CC) -c $(CFLAGS) $<


all: $(ALL)

prodcons_t: prodcons_t.o
    $(CC) -o $@ $< $(LDFLAGS)

producer: producer.o
    $(CC) -o $@ $< $(LDFLAGS)

consumer: consumer.o
    $(CC) -o $@ $< $(LDFLAGS)

producer_s: producer_s.o semlib.o
    $(CC) -o $@ $^ $(LDFLAGS)

consumer_s: consumer_s.o semlib.o
    $(CC) -o $@ $^ $(LDFLAGS)

prodcons: prodcons.o
    $(CC) -o $@ $< $(LDFLAGS)

dining: dining.o
    $(CC) -o $@ $< $(LDFLAGS)

dining2: dining2.o
    $(CC) -o $@ $< $(LDFLAGS)

prodcons_m: prodcons_m.o
    $(CC) -o $@ $< $(LDFLAGS)

prodcons_s: prodcons_s.o semlib2.o
    $(CC) -o $@ $^ $(LDFLAGS)

sipc1_hw2: sipc1_hw2.o semlib.o
    $(CC) -o $@ $^ $(LDFLAGS)

sipc2_hw2: sipc2_hw2.o semlib.o
    $(CC) -o $@ $^ $(LDFLAGS)

sipc_hw3_1: sipc_hw3_1.o
    $(CC) -o $@ $^ $(LDFLAGS)

sipc_hw3_2: sipc_hw3_2.o
    $(CC) -o $@ $^ $(LDFLAGS)

clean:
    rm -rf *.o $(ALL)