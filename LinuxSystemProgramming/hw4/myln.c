#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    /* argc의 갯수가 3개인지 체크 */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* argv[1], argv[2]를 이용하여 hard link를 생성함. */
    if (link(argv[1], argv[2]) < 0) {
        perror("link");
        exit(1);
    }
}