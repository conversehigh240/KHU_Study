#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

main()
{
    DIR *dp;
    struct dirent *dep;

    /* opendir의 return value는 DIR 구조체의 포인터임 */
    if ((dp = opendir(".")) == NULL) {
        perror("opendir");
        exit(0);
    }

    /* 이 포인터 값을 파라미터로 받은 readdir을 한 번 할 때마다
       하나의 파일 엔트리가 넘어오고, 그 구조체가 dirent임.
       즉, 디렉토리 내의 모든 항목을 순차적으로 읽고, 마지막까지 모두 읽으면
       NULL을 return함. */
    while (dep = readdir(dp)) {
        printf("%s\n", dep->d_name);
    }

    closedir(dp);
}