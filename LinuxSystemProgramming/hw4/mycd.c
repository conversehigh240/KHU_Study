#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dirname\n", argv[0]);
        exit(1);
    }

    /* chdir을 이용하여 argv[1]의 경로로 현재 디렉토리 변경 */
    if (chdir(argv[1]) < 0) {
        perror("chdir");
        exit(1);
    }
}