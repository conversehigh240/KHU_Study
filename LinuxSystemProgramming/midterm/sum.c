#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
    int a;
    int b;
    int sum;

    if (argc != 3) {
        printf("Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    sum = a + b;

    printf("%d+%d=%d\n", a, b, sum);
}