CC = gcc #컴파일러를 gcc로 이용하겠다는 것이다.

.PHONY: default all clean
    

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

HEADERS = $(wildcard *.h)

#위와 마찬가지로 현재 디렉토리 내의 모든 .h 파일을 HEADERS 변수에 저장하는 것이다.

%.o: %.c $(HEADERS)
    $(CC) $(CFLAGS) -c $< -o $@

#dependency line과 command line이다. 여기서 %는 wildcard를 의미한다.  우선 dependency line서는, 디렉토리 내의 모든 .o 파일이 target이고, 모든 .c 파일과 헤더들이 prerequisite인 것을 >알 수 있다. 이 때, prerequisites에 헤더 파일이 포함되었기 때문에 헤더파일 수정 시 그냥 make>만 해도 수정사항이 반영된다. 

#command line에서는 위의 dependency line을 수행할 때 이용하는 옵션이다. 변수로 작성된 부분 풀어 쓰면 gcc -g -Wall -c $< -o $@인데, 위에서 정의한 변수를 제외하고 -c는 컴파일만 하라는 뜻이며 -o는 output file의 이름을 지정하는 옵션이다. 즉, 현재 prerequisite 파일을 컴파일만 고, 현재 target의 이름으로 output 파일 이름을 지정하라는 뜻이다.

.PRECIOUS: $(TARGET) $(OBJECTS)

#TARGET과 OBJECTS들이 중간에 지워지지 않도록 보호해주는 옵션이다.

$(TARGET): $(OBJECTS)
    $(CC) $(OBJECTS) -Wall $(LIBS) -o $@

#이것도 dependency line과 command line이다. dependency line은 위에서 정의한 TARGET과 OBJECTS들이 각각 target, prerequisites이다. command line은 gcc 컴파일러를 이용해서 OBJECTS 파일들 컴파일하겠다는 의미인데, -Wall을 이용해서 모든 경고문을 출력하는 옵션을 주었다. 또, LIBS는 위에서 -lm으로 정의했기 때문에 수학 라이브러리가 링킹된다. -o $@를 이용해서 output file의 름을 target 이름으로 지정되도록 하였다.

clean:
    -rm -f *.o
    -rm -f $(TARGET)

#make clean을 했을 때, 모든 .o 파일과, 만들어진 실행 파일 (prog)를 지우라는 의미이다.
