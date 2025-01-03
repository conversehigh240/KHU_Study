#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 1024

main(int argc, char *argv[])
{
    FILE *src, *dst;
    char buf[MAX_BUF];
    int ch;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    if ((src = fopen(argv[1], "rt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    if ((dst = fopen(argv[2], "wt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(toupper(ch), dst);
    }

    fclose(src);
    fclose(dst);
}