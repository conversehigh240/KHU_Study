#include <stdio.h>
  
main(int argc, char *argv[])
{
    FILE *fp;
    int ch;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "rt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {
        fputc(toupper(ch), stdout);
    }

    fclose(fp);
}