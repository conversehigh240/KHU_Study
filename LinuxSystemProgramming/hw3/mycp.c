#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

main(int argc, char *argv[])
{
    int fd1, fd2, count;
    char buf[MAX_BUF];

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 3) {
        printf("Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* READ ONLY로 파일을 오픈함*/
    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    /* WRITE ONLY로 파일을 오픈함. 이 때, 오픈하려는 파일이 없다면 CREATE하고,
       만약 파일이 있는데 이미 다른 내용이 저장되어있다면 TRUNC로 내용을 날림.
       또, User-Group-Other에 대한 R-W-X 권한을 8진수를 이용하여 지정함.
       여기서 8진수로 644의 권한을 가지므로 User-Group-Other의 권한은 RW-R-R
       이 됨. */
    if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        perror("open");
        exit(1);
    }

    /* while문을 이용하여 fd1을 끝까지 읽고, 더이상 읽을 게 없을 때 count 값이
       0이 되므로 while문을 빠져나감.
       이 fd1에서 읽은 내용을 fd2에 write하여 복사함. */
    while ((count = read(fd1, buf, MAX_BUF)) > 0) {
        write(fd2, buf, count);
    }

    /* open했던 파일을 모두 close함 */
    close(fd1);
    close(fd2);
}