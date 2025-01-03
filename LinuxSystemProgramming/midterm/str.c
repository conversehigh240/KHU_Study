#include <stdio.h>
  
#define MAX_BUF 1024

int strcasecmp(char *dst, char *src)
{
    char str1 = toupper(*dst);
    char str2 = toupper(*src);

    return str1 - str2;
}

main(int argc, char *argv[])
{
    int result;

    if (argc != 3) {
        printf("Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    result = strcasecmp(argv[1], argv[2]);

    printf("%d\n", result);

}