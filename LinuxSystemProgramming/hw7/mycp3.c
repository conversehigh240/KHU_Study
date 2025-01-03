#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
    int         fdin, fdout;
    char        *src, *dst;
    struct stat statbuf;

    if (argc != 3)  {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* 복사할 소스 파일을 read only 모드로 open함 */
    if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
        perror("open");
        exit(1);
    }

    /* 복사되는 대상 파일을 read-write 모드로 open함. 파일이 없으면 생성하고,
    기존의 내용은 삭제함. */
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
        perror("open");
        exit(1);
    }

    /* 소스 파일 크기 가져옴 */
    if (fstat(fdin, &statbuf) < 0)  {
        perror("fstat");
        exit(1);
    }

    /* 대상 파일의 크기를 소스 파일의 크기와 동일하게 설정함 */
    if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
        perror("lseek");
        exit(1);
    }
    write(fdout, "", 1);

    /* 소스 파일을 read 전용으로 메모리에 맵핑함 */
    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
        == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }

    /* 대상 파일을 write 전용으로 메모리에 맵핑함 */
    if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0))
        == (caddr_t) -1)  {
        perror("mmap");
        exit(1);
    }

    /* 메모리에 맵핑된 소스 내용을 대상 파일에 복사함 */
    memcpy(dst, src, statbuf.st_size);

    close(fdin);
    close(fdout);
}