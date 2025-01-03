#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 1024

long filesize(FILE *fp)
{
    long cur, size;
   
    cur = ftell(fp); // 파일의 현재 포인터 위치를 cur에 할당
    fseek(fp, 0L, SEEK_END); // 파일 포인터를 파일의 끝으로 이동
    size = ftell(fp); // 파일의 끝 위치를 size에 할당
    fseek(fp, cur, SEEK_SET); // 파일 포인터를 초기 위치로 이동
    return(size); // size 반환
}

main(int argc, char *argv[])
{
    FILE *src, *dst1, *dst2;
    char buf[MAX_BUF];
    int count, size;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 4) {
        fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
        exit(1);
    }

    /*command line에서 입력받은 argv[1]을 read binary 모드로 open하고,
      그것을 src에 할당*/
    if ((src = fopen(argv[1], "rb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /*command line에서 입력받은 argv[2]를 write binary 모드로 open하고,
      그것을 dst1에 할당*/
    if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
            perror("fopen");
                exit(1);
    }

    /*command line에서 입력받은 argv[3]을 write binary 모드로 open하고,
      그것을 dst2에 할당*/
    if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
            perror("fopen");
                exit(1);
    }

    /* 파일 크기의 절반을 size에 할당*/
    size = filesize(src) / 2;

    /* dst1에 src 파일의 절반을 복사함. 이 때, size가 MAX_BUF보다 크면
       MAX_BUF를 count에 할당하고, size가 작으면 size를 할당함.
       그 후 fread를 이용하여 src에서 count만큼 읽어와 buf에 저장하고,
       fwrite를 이용하여 dst1에 count만큼 write함. */
    while (size > 0) {
        count = (size > MAX_BUF) ? MAX_BUF : size;
        fread(buf, 1, count, src);
        fwrite(buf, 1, count, dst1);
        size -= count;
    }

    /* 남은 파일의 절반을 dst2에 저장함.
    결과적으로 src 파일을 dst1, dst2로 절반씩 나누어 복사하는 내용임 */
    while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
        fwrite(buf, 1, count, dst2);
    }

    /* 파일을 닫아줌*/
    fclose(src);
    fclose(dst1);
    fclose(dst2);
}