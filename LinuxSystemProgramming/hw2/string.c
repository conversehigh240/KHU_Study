#include <stdio.h>
  
int strlen_p(char *str)
{
    int len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

/*
   (strlen_p)
   포인터를 이용하여 string의 길이를 구하는 함수이다.
   while문을 이용하여 *str이 끝이 날 때까지 한 글자씩 이동하며 loop를 돌면서
   길이를 구한다.
*/

int strlen_a(char str[])
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
        ;
    return i;
}

/*
   (strlen_a)
   array를 이용하여 string의 길이를 구하는 함수이다.
   for문을 이용해서 str[]의 끝까지 한 글자씩 인덱스를 이동하며 loop를 돌면서
   길이를 구한다.
*/

void strcpy_p(char *dst, char *src)
{
    while (*src) {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = *src;
}

/*
   (strcpy_p)
   포인터를 이용하여 string을 copy하는 함수이다.
   *src가 null일 때까지 while loop를 돌며, *dst에 *src를 한 글자씩
   복사하는 것이다.
   또, 마지막에 *dst = *src를 하는 이유는, null을 만나면 loop를 빠져나오기
   때문에 string의 마지막'\0'까지 완벽히 복사를 하기 위함이다.
*/

void strcpy_a(char dst[], char src[])
{
    int i;
    for (i = 0; src[i] != '\0'; i++)
        dst[i] = src[i];
    dst[i] = src[i];
}

/*
   (strcpy_a)
   array를 이용하여 string을 copy하는 함수이다.
   for loop를 돌며 src와 동일한 dst의 인덱스에 값을 복사한다.
   loop를 다 돌고 난 후에는 string의 마지막인 '\0'을 복사하며 마무리한다.
*/

void strcat_p(char *dst, char *src)
{
    while (*dst++)
        ;
    dst--;
    while (*src) {
        *dst++ = *src++;
    }
    *dst = *src;
}

/*
   (strcat_p)
   strcat은 기존 문자열 뒤에 append 하는 함수이다.
   dst 뒤에 src를 append 해야 하니 일단 dst의 제일 마지막으로 이동한다.
   여기서 while을 이용하여 맨 끝까지 간 후, null character의 한 칸 앞으로
   옮겨준다.
   그 후 strcpy처럼 src의 끝까지 loop를 돌며 dst 뒤에 복사한다.
   마지막 '\0' 역시 복사하여준다.
*/

void strcat_a(char dst[], char src[])
{
    int i, j;
    for (i = 0; dst[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dst[i+j] = src[j];
    dst[i+j] = src[j];
}

/*
   (strcat_a)
   array를 이용하여 strcat을 하는 함수이다.
   strcat_p와 로직은 동일하며, array이기 때문에 for loop를 이용하여
   인덱스를 이동시켜주는 방식을 사용한다.
   dst의제일 마지막 인덱스는 '\0'이므로 그것보다 한 칸 앞으로 이동한 후
   src의 내용을 복사한다.
   마지막에는 '\0'을 복사하여 마무리한다.
*/

int strcmp_p(char *dst, char *src)
{
    while (*dst || *src) {
        if (*dst != *src) {
            return *dst - *src;
        }
        dst++;
        src++;
    }
    return *dst - *src;
}

/*
   (strcmp_p)
   pointer를 이용하여 문자열을 비교하는 함수이다.
   두 문자열 모두 끝나기 전까지 while loop를 돌며 비교한다.
   만약 가리키는 값이 동일하다면 dst와 src 모두 다음 값으로 이동시켜주고,
   값이 다르다면 그 값의 차이를 return한다.
   strcmp의 return 값은 dst와 src의 차이이며 이 때 계산을 할 때에는
   ASCII CODE를 이용한다.
*/

int strcmp_a(char dst[], char src[])
{
    int i;
    for (i = 0; dst[i] != '\0' || src[i] != '\0'; i++) {
        if (dst[i] != src[i]) {
            return dst[i] - src[i];
        }
    }
    return 0;
}

/*
   (strcmp_a)
   array를 이용하여 문자열을 비교하는 함수이다.
   for loop을 이용하여 dst와 src 모두 끝나기 전까지 돌며 비교한다.
   이 때, 두 array의 인덱스 값이 동일하다면 별 일 없이 for loop이
   끝나게 되고, 0을 리턴한다.
   두 array의 인덱스 값이 다르다면 값들의 차이를 return한다.
*/

main()
{
    int len1, len2;
    char str1[20], str2[20];

    len1 = strlen_p("Hello");
    len2 = strlen_a("Hello");
    printf("strlen: p=%d, a=%d\n", len1, len2);

    strcpy_p(str1, "Hello");
    strcpy_a(str2, "Hello");
    printf("strcpy: p=%s, a=%s\n", str1, str2);

    strcat_p(str1, ", World!");
    strcat_a(str2, ", World!");
    printf("strcat: p=%s, a=%s\n", str1, str2);

/*
   위에서 만든 함수들을 사용하는 예시이다.
*/

    int result1, result2;
    strcpy_p(str1, "String");
    strcpy_a(str2, "Comparing");
    result1 = strcmp_p(str1, str2);
    result2 = strcmp_a(str1, str2);
    printf("strcmp: p=%d, a=%d\n", result1, result2);

/*
   숙제2의 사용 예시이다.
   우선 비교를 위해 strcpy를 이용하여 str1, str2에 저장되어 있던
   "Hello, World!"에 각각 "String", "Comparing"을 덮어씌웠다.
   그 후, 작성한 strcmp_p와 strcmp_a의 결과를 각각 result1, result2에
   저장한 뒤, printf로 출력하도록 하였다.
*/
}