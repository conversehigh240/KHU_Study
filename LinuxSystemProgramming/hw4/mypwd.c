#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 256

main(int argc, char *argv[])
{
    char buf[MAX_BUF];

    /* getcwd를 이용하여 현재 작업 디렉토리 경로를 buf에 저장함 */
    if (getcwd(buf, MAX_BUF) == NULL) {
        perror("getcwd");
        exit(1);
    }
    printf("%s\n", buf);
}