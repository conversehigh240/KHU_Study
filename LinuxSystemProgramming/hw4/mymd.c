#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dirname\n", argv[0]);
        exit(1);
    }

    /* argv[1] 이름의 디렉토리를 생성함. 권한은 755로 user에게는
       read-write-execute 권한을 모두 부여하고, group과 other에게는
       read와 execute 권한만을 부여함.*/
    if (mkdir(argv[1], 0755) < 0) {
        perror("mkdir");
        exit(1);
    }
}