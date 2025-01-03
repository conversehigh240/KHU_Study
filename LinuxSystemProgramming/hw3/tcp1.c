#include <stdio.h>
  
main(int argc, char *argv[])
{
    FILE *src, *dst;
    int ch;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료 */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 read text 모드로 open하고,
     이것을 src에 할당함*/
    if ((src = fopen(argv[1], "rt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* command line에서 입력받은 argv[2]를 write text 모드로 open하고,
       이것을 dst에 할당함*/
    if ((dst = fopen(argv[2], "wt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* fgetc를 이용하여 src에서 한 문자씩 읽어와 ch에 저장함.
       이것을 End Of File이 될 때까지 반복하며, ch를 이용하여
       dst에 fputc로 write함*/
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    /* 사용이 끝난 파일을 fclose로 닫아줌*/
    fclose(src);
    fclose(dst);
}