#include <stdio.h>
  
main(int argc, char *argv[])
{
    int i;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    /* command line으로 전달된 파일들을 모두 삭제함 */
    for (i = 1; i < argc; i++) {
        if (remove(argv[i]) < 0) {
            perror("remove");
            exit(1);
        }
    }
}