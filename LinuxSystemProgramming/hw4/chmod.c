#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
    struct stat statbuf;

    /* 파일 "bar"의 정보를 불러와 statbuf에 저장함 */
    if (stat("bar", &statbuf) < 0) {
        perror("stat");
        exit(1);
    }

    /* 파일 "bar"의 권한을 변경함.
       group의 write 권한을 제거하고, Set User Id를 추가함 */
    if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0) {
        perror("chmod");
        exit(1);
    }

    /* 파일 "foo"의 권한을 변경함.
       user에 read, write 권한을 추가하고 group과 other에 read 권한을 추가함*/
    if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        perror("chmod");
        exit(1);
    }
}