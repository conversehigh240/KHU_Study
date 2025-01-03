#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

#define MAX_PATH 256

void lsl(char *path)
{
    DIR *dp;
    struct dirent *dep;
    struct stat statbuf;
    char fullpath[MAX_PATH];
    char timebuf[20];
    struct tm *tm_info;
    long total_blocks = 0;

    /* 주어진 경로의 디렉토리를 open함 */
    if ((dp = opendir(".")) == NULL) {
        perror("opendir");
        exit(0);
    }

    /* readdir을 이용하여 디렉토리 내의 항목 탐색 */
    while (dep = readdir(dp)) {
        /* 이름이 "."이거나 ".."인 경우는 건너뜀 */
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;

        /* fullpath = 기존 경로 + "/" + 항목이름 */
        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, dep->d_name);

        /* fullpath 경로의 파일 상태 정보를 lstat으로 가져옴 */
        if (lstat(fullpath, &statbuf) < 0) {
            perror("lstat");
            continue;
        }

        /* 파일의 타입 출력 */
        if (S_ISREG(statbuf.st_mode))
            printf("%s", "-");
        else if (S_ISDIR(statbuf.st_mode))
            printf("%s", "d");
        else if (S_ISCHR(statbuf.st_mode))
            printf("%s", "c");
        else if (S_ISBLK(statbuf.st_mode))
            printf("%s", "b");
        else if (S_ISFIFO(statbuf.st_mode))
            printf("%s", "p");
        else if (S_ISLNK(statbuf.st_mode))
            printf("%s", "l");
        else if (S_ISSOCK(statbuf.st_mode))
            printf("%s", "s");
        else
            printf("%s", "?");

        /* 파일의 권한 출력 */
        printf("%c%c%c", (statbuf.st_mode & S_IRUSR) ? 'r' : '-',
                (statbuf.st_mode & S_IWUSR) ? 'w' : '-',
                (statbuf.st_mode & S_IXUSR) ? 'x' : '-');
        printf("%c%c%c", (statbuf.st_mode & S_IRGRP) ? 'r' : '-',
                (statbuf.st_mode & S_IWGRP) ? 'w' : '-',
                (statbuf.st_mode & S_IXGRP) ? 'x' : '-');
        printf("%c%c%c ", (statbuf.st_mode & S_IROTH) ? 'r' : '-',
                (statbuf.st_mode & S_IWOTH) ? 'w' : '-',
                (statbuf.st_mode & S_IXOTH) ? 'x' : '-');

        /* 링크 수 출력 */
        printf("%ld ", statbuf.st_nlink);

        /* user id를 이용하여 사용자 이름 출력 */
        printf("%s ", getpwuid(statbuf.st_uid)->pw_name);

        /* group id를 이용하여 그룹 이름 출력 */
        printf("%s ", getgrgid(statbuf.st_gid)->gr_name);

        /* 파일 크기 출력*/
        printf("%lld ", (long long)statbuf.st_size);

        /* 수정 시간 출력 */
        tm_info = localtime(&statbuf.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
        printf("%s ", timebuf);

        /* 파일 이름 출력 */
        printf("%s\n", dep->d_name);

        total_blocks += statbuf.st_blocks;

    }

    /* 블록의 총 갯수 출력 */
    printf("total %ld\n", total_blocks / 2);

    closedir(dp);
}

int main()
{
    lsl(".");
}