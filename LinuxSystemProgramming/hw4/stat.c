#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
    struct stat statbuf; // 파일 상태를 저장하는 구조체
    char *mode;
    int i;

    for (i = 1; i < argc; i++)
    {
        printf("%s: ", argv[i]);

        /* lstat을 이용하여 파일의 상태 정보를 가져옴.
           첫번째 파라미터에는 경로가 들어가는데, 이 경로가 symbolic link일
           경우 lstat은 링크 자체의 메타데이터를 가져옴.*/
        if (lstat(argv[i], &statbuf) < 0) {
            perror("lstat");
            continue;
        }

        /* 파일의 종류를 판별하여 mode에 해당 문자열을 저장함 */
        if (S_ISREG(statbuf.st_mode))
            mode = "regular";
        else if (S_ISDIR(statbuf.st_mode))
            mode = "directory";
        else if (S_ISCHR(statbuf.st_mode))
            mode = "character special";
        else if (S_ISBLK(statbuf.st_mode))
            mode = "block special";
        else if (S_ISFIFO(statbuf.st_mode))
            mode = "FIFO";
        else if (S_ISLNK(statbuf.st_mode))
            mode = "symbolic link";
        else if (S_ISSOCK(statbuf.st_mode))
            mode = "socket";

        /* stat 구조체의 필드 값들을 출력함 */
        printf("%s\n", mode);
        printf("\tst_mode = %d\n", statbuf.st_mode);
        printf("\tst_ino = %d\n", statbuf.st_ino);
        printf("\tst_dev = %d\n", statbuf.st_dev);
        printf("\tst_rdev = %d\n", statbuf.st_rdev);
        printf("\tst_nlink = %d\n", statbuf.st_nlink);
        printf("\tst_uid = %d\n", statbuf.st_uid);
        printf("\tst_gid = %d\n", statbuf.st_gid);
        printf("\tst_size = %d\n", statbuf.st_size);
        printf("\tst_atime = %d\n", statbuf.st_atime);
        printf("\tst_mtime = %d\n", statbuf.st_mtime);
        printf("\tst_ctime = %d\n", statbuf.st_ctime);
        printf("\tst_blksize = %d\n", statbuf.st_blksize);
        printf("\tst_blocks = %d\n", statbuf.st_blocks);
    }
}