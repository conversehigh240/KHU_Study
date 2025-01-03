#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
    /* umask(0)은 권한 제한 없이 파일 생성 시 요청한 권한이 그대로 적용됨 */
    umask(0);

    /* 파일 "bar"를 생성하고, User-Group-Other 모두 read, write 권한을 가짐*/
    if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) {
        perror("creat");
        exit(1);
    }

    /* group과 other에read, write 권한을 차단함*/
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    /* 파일 "foo"를 생성하고, User-Group-Other에 read, write 권한을 주지만
       umask에 의해 group과 other에는 read, write 권한이 차단됨.
       즉, user에게만 read, write 권한이 주어짐*/
    if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) {
        perror("creat");
        exit(1);
    }
}