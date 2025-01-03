#include <stdio.h>
  
main()
{
    char c, s[80];
    int i; long l;
    float f; double d;

    scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d);
    printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);

    c = getchar();
    putchar(c);

    gets(s);
    puts(s);
}

/*
   C++의 cin처럼 C에서는 scanf를 이용하여 사용자의 입력을 받는다.
   C에서는 reference type이 없기 때문에 &를 이용하여 포인터 변수를
   넘겨주는 방식으로 이용한다.
   여기서 s는 char array이기 때문에 그자체를 포인터라고 볼 수 있기에
   &를 쓰지 않아도 무방하다.

   또, character 하나만을 입력받을 때에는 getchar를 이용할 수 있고
   string을 입력받을 때에는 gets를 이용할 수 있다.
*/
