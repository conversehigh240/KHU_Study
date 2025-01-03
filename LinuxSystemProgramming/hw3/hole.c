#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
    int fd;
    char ch = '\0';

    /* file.hole 파일을 생성함. 이 때, User-Group-Other의 권한은 User에만
       Read 권한이 있고, 나머지에는 아무 권한 없음.*/
    if ((fd = creat("file.hole", 0400)) < 0) {
        perror("creat");
        exit(1);
    }

    /* 기준점인 whence가 SEEK_SET이고, offset이1023이므로 파일의 시작 지점에서
       1023 바이트 떨어진 곳으로 포인터를 이동시킴*/
    if (lseek(fd, 1023, SEEK_SET) < 0) {
        perror("lseek");
        exit(1);
    }

    /* 파일의 시작 지점에서 1023 바이트 떨어진 곳에 '\0'을 write함.
       즉, 이 file.hole은 중간 1023바이트는 비어있고 마지막에 NULL character가
       있는 1024 바이트 크기의 파일임.*/
    write(fd, &ch, 1);

    /* file.hole을 다 사용했으므로 close해줌*/
    close(fd);
}