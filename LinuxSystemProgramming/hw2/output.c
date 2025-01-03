#include <stdio.h>
  
main()
{
    char c = 'a', s[] = "hello";
    int i = 100; long l = 99999;
    float f = 3.14; double d = 99.999;
    int *p = &i;

    printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
    putchar(c);
    puts(s);
}

/*
   C++에서 cout을 쓰듯이 C에서는 printf를 이용한다.
   이 printf 안에서 쓰는 형식을 format string이라고 하고,
   여기에 변수를 출력하기 위해서는 format을 지정해주어야 한다.
   %c는 char, %s는 string, %d는 int(decimal),
   %#X는 Hexa(여기서 X를 대문자로 쓰면 A-E가 대문자로 출력되고
   소문자로 쓰면 a-e가 소문자로 출력된다.
   또, #을 붙이면 0x를 같이 출력한다.)
   %ld는 long int(long decimal)
   %.4f는 floating point인데 소수점 넷째자리까지 출력되고,
   %.2lf는 long floating point(double)이며 소수점 둘째자리까지 출력되며
   %p는 pointer를 의미한다.

   또, putchar는 character를 출력하고 puts는 string을 출력한다.
*/