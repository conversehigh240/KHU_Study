#include <stdio.h>
  
main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++)
        printf("argv[%d]=%s\n", i, argv[i]);
}

/*
    argc 갯수만큼 루프를 반복하여 각 요소를 하나씩 출력하는 프로그램이다.
    만약 ./args hello output이라고 입력한다면
    argv[0]=./args
    argv[1]=hello
    argv[2]=output
    이렇게 출력될 것이라는걸 알 수 있다.
*/
