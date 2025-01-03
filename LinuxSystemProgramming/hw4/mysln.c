#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* argv[1], argv[2]를 이용하여 symbolic link를 생성함 */
    if (symlink(argv[1], argv[2]) < 0) {
        perror("link");
        exit(1);
    }
}