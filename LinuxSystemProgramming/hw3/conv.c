#include <stdio.h>
#include <string.h>
#include "record.h" // record.h 헤더 포함

main(int argc, char *argv[])
{
    FILE *src, *dst;
    Record rec;

    /* argc 갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source dest\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 read text 모드로 open하고,
       이것을 src에 할당함*/
    if ((src = fopen(argv[1], "rt")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* command line에서 입력받은 argv[2]를 write binary 모드로 open하고,
       이것을 dst에 할당함*/
    if ((dst = fopen(argv[2], "wb")) == NULL) {
        perror("fopen");
        exit(1);
    }

    /* fgets로 src에서 정보를 읽어옴. 이 때, SMAX의 크기만큼 읽어오고, 읽어온
       문자열에서 '\n'을 '\0'으로 변경함.
       또, NMAX의 크기만큼 읽어오고, '\n'을 '\0'으로 변경함.
       DMAX의 크기만큼 읽어오고, '\n'을 '\0'으로 변경함.
       그 후, 읽어온 record의 정보를 binary 형태로 dst에 write함.
       이 코드가 실행되고나면, dst 파일에는 학생들의 이름-학번-학과의 내용이
       binary 형태로 연속적으로 저장되게 됨.*/
    while (fgets(rec.stud, SMAX, src)) {
        *strchr(rec.stud, '\n') = '\0';
        fgets(rec.num, NMAX, src);
        *strchr(rec.num, '\n') = '\0';
        fgets(rec.dept, DMAX, src);
        *strchr(rec.dept, '\n') = '\0';

        fwrite(&rec, sizeof(rec), 1, dst);
    }

    /* 파일을 닫아줌*/
    fclose(src);
    fclose(dst);
}