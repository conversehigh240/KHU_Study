#include <stdio.h>
  
#define MAX_BUF 1024

main(int argc, char *argv[])
{
    FILE *src, *dst;
    char buf[MAX_BUF];
    int count;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 read binary 모드로 open하고,
       이것을 src에 할당함*/
    if ((src = fopen(argv[1], "rb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* command line에서 입력받은 argv[2]를 write binary 모드로 open하고,
       이것을 dst에 할당함*/
    if ((dst = fopen(argv[2], "wb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* fread를 이용하 src 파일을 읽음. 이 때, 1바이트의 크기를 갖는 object를
       MAX_BUF만큼 읽어들여 buf에 저장함.
       읽어 들인 내용을 fwrite를 이용하여 dst에 write함. 여기서, fread에는
       MAX_BUF를 사용하고 fwrite에는 count를 사용하는 이유는, 읽어 들인
       데이터가 MAX_BUF보다 작을 수 있기 때문에 실제로 읽어 들인 데이터의
       크기를 주어 잘못된 데이터가 기록되지 않게 함*/
    while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
        fwrite(buf, 1, count, dst);
    }

    /* 파일을 닫아줌*/
    fclose(src);
    fclose(dst);
}