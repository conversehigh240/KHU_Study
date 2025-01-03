#include <stdio.h>
  
#define MAX_BUF 256

main(int argc, char *argv[])
{
    FILE *fp;
    char buf[MAX_BUF];
    int line;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 "rt", 즉 read text 모드로 open함*/
    if ((fp = fopen(argv[1], "rt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /*fgets를 이용하여 한 라인씩 읽어 buf에 저장하고, 그것들을 출력함.
      fgets는 파일 끝에 도달하면 NULL을 반환하며 while문을 빠져나감.*/
    line = 1;
    while (fgets(buf, MAX_BUF, fp)) {
        printf("%4d: %s", line++, buf);
    }

    /* 파일을 다 썼으니 fclose로 닫아줌*/
    fclose(fp);
}