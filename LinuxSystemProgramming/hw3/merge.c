#include <stdio.h>
  
#define MAX_BUF 1024

main(int argc, char *argv[])
{
    FILE *src1, *src2, *dst;
    char buf[MAX_BUF];
    int count;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 4) {
        fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 read binary 모드로 open하고,
       그것을 src1에 할당*/
    if ((src1 = fopen(argv[1], "rb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* command line에서 입력받은 argv[2]를 read binary 모드로 open하고,
      그것을 src2에 할당*/
    if ((src2 = fopen(argv[2], "rb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* command line에서 입력받은 argv[3]을 write binary 모드로 open하고,
       그것을 dst에 할당*/
    if ((dst = fopen(argv[3], "wb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* fread를 이용하여 src1을 끝까지 읽어들이고, 이것을 buf에 저장함.
       fwrite를 이용하여 buf에 저장된 내용을 dst에 write함.*/
    while ((count = fread(buf, 1, MAX_BUF, src1)) > 0) {
        fwrite(buf, 1, count, dst);
    }

    /* fread를 이용하여 src2를 끝까지 읽어들이고, 이것을 buf에 저장함.
       fwrite를 이용하여 buf에 저장된 내용을 dst에 write함.
       즉, 이 코드는 src1과 src2의 내용을 dst에 모두 복사하는 것임*/
    while ((count = fread(buf, 1, MAX_BUF, src2)) > 0) {
        fwrite(buf, 1, count, dst);
    }

    /* 파일을 닫아줌.*/
    fclose(src1);
    fclose(src2);
    fclose(dst);
}