#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dirname\n", argv[0]);
        exit(1);
    }

    /* argv[1] 이름의 디렉토리를 remove 함. */
    if (rmdir(argv[1]) < 0) {
        perror("rmdir");
        exit(1);
    }
}