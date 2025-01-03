CC = gcc #컴파일러를 gcc를 쓴다는 것
CFLAGS = -g #디버깅을 할 수 있도록 하는 옵션
LDFLAGS =

.SUFFIXES: .c .o #SUFFIXES RULE을 적어줌 (.c를 이용하여 .o를 만들겠다는 뜻)
.c.o:
    $(CC) -c $(CFLAGS) $<

ALL = hello output input args string

all: $(ALL)

#아래는 모두 dependency line과 command line
hello: hello.o
    $(CC) -o  $@ $< $(LDFLAGS)

output: output.o
    $(CC) -o  $@ $< $(LDFLAGS)

input: input.o
    $(CC) -o  $@ $< $(LDFLAGS)

args: args.o
    $(CC) -o  $@ $< $(LDFLAGS)

string: string.o
    $(CC) -o  $@ $< $(LDFLAGS)

clean:
    rm -rf *.o $(ALL)