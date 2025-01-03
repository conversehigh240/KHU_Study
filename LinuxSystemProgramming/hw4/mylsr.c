#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH 256

void JustDoIt(char *path)
{
    DIR *dp;
    struct dirent *dep;
    struct stat statbuf;
    char fullpath[MAX_PATH];

    /* 주어진 경로의 디렉토리를 open함 */
    if ((dp = opendir(path)) == NULL) {
        perror("opendir");
        exit(0);
    }

    /* 현재 탐색중인 디렉토리 이름 출력 */
    printf("\n%s:\n", path);

    /* readdir을 이용하여 디렉토리 내의 모든 항목의 이름을 출력함.
       이 때, 이름이 "."이거나 ".."인 경우는 건너뜀 */
    while (dep = readdir(dp)) {
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;
        printf("%s\n", dep->d_name);
    }

    /* dp를 맨 처음으로 다시 되돌림 */
    rewind(dp);

    /* 디렉토리 내부를 재귀적으로 탐색함 */
    while (dep = readdir(dp)) {
        /* 이름이 "."이거나 ".."인 경우는 건너뜀 */
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;

        /* fullpath = 기존경로 + "/" + 항목 이름 */
        strcpy(fullpath, path); // fullpath 변수에 기존 변수를 복사함
        strcat(fullpath, "/"); // "/"를 fullpath 맨 뒤에 추가함
        strcat(fullpath, dep->d_name); // fullpath 뒤에 항목 이름을 추가함

        /* fullpath 경로의 파일 상태 정보를 lstat으로 가져옴 */
        if (lstat(fullpath, &statbuf) < 0) {
            perror("lstat");
            exit(1);
        }

        /* 만약 항목이 디렉토리인 경우, 재귀적으로 자기 자신 함수를 호출함 */
        if (S_ISDIR(statbuf.st_mode)) {
            JustDoIt(fullpath);
        }
    }
    closedir(dp);
}

main()
{
    /* 현재 디렉토리를 시작점으로 탐색을 수행함 */
    JustDoIt(".");
}