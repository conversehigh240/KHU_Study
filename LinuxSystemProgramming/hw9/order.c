#include <stdio.h>
  
main()
{
    int     a = 0x12345678;
    char    *p = (char *)&a;

/* 메모리에서 변수 a의 저장 방식이 빅 엔디안인지 리틀 엔디안인지 확인함 */
#if 1
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
#else
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
#endif
}