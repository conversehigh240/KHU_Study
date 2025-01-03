#include <stdio.h>
#include <unistd.h>
#include "record.h"

/* 포인터를 사용하여 Record 정보를 출력함*/
void print_record(Record *rp)
{
    puts(rp -> stud);
    puts(rp -> num);
    puts(rp -> dept);
}

main(int argc, char *argv[])
{
    FILE *fp;
    Record rec;

    /* argc  갯수를 체크하여 맞지 않으면 프로그램 종료*/
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    /* command line에서 입력받은 argv[1]을 r+b 모드로 open하고,
       이것을 fp에 할당함. 이 때, r+는 read 모드로 오픈하는데 write까지
       하겠다는 뜻으로 기존 파일의 내용을 유지하며 write를 할 수 있음.*/
    if ((fp = fopen(argv[1], "r+b")) == NULL) {
        perror("fopen");
        exit(1);
    }

    printf("-----Ordered Record List-----\n");

    /* 파일에서 record를 순차적으로 읽어 출력함.
       출력한 후에는 포인터를 다시 시작 위치로 이동시켜주고,
       사용자의 입력을 받음.*/
    while (fread(&rec, sizeof(rec), 1, fp)) {
        print_record(&rec);
    }
    rewind(fp);
    getchar();

    printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");

    /* 파일 포인터를 3번째 레코드의 위치로 이동시켜 출력함.*/
    fseek(fp, sizeof(rec)*2L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);

    /* 파일 포인터를 6번째 레코드의 위치로 이동하여 출력함*/
    fseek(fp, sizeof(rec)*5L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);

    /* 파일 포인터를 2번째 레코드의 위치로 이동하여 출력함*/
    fseek(fp, sizeof(rec)*1L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);

    /* 파일 포인터를 4번째 레코드의 위치로 이동하여 출력함*/
    fseek(fp, sizeof(rec)*3L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);

    /* 파일 포인터를 1번째 레코드의 위치로 이동하여 출력함*/
    fseek(fp, sizeof(rec)*0L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);

    /* 파일 포인터를 5번째 레코드의 위치로 이동하여 출력함*/
    fseek(fp, sizeof(rec)*4L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    print_record(&rec);
    getchar(); //사용자의 입력을 받음

    printf("-----Updated Record List-----\n");

    /* 파일 포인터를 3번째 레코드의 위치로 이동하고, 데이터를 read함.
       그 후, dept 내용을 Movie로 변경하고, fwrite로 수정된 내용을
       rec 구조체에 write함.*/
    fseek(fp, sizeof(rec)*2L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    strcpy(rec.dept, "Movie");
    fseek(fp, sizeof(rec)*2L, SEEK_SET);
    fwrite(&rec, sizeof(rec), 1, fp);

    /* 파일 포인터를 6번째 레코드의 위치로 이동하고, 데이터를 read함.
       그 후, dept 내용을 Baseball로 변경하고, fwrite로 수정된 내용을
       rec 구조체에 write함.*/
    fseek(fp, sizeof(rec)*5L, SEEK_SET);
    fread(&rec, sizeof(rec), 1, fp);
    strcpy(rec.dept, "Baseball");
    fseek(fp, sizeof(rec)*5L, SEEK_SET);
    fwrite(&rec, sizeof(rec), 1, fp);

    rewind(fp); // 파일 포인터를 다시 맨처음으로 돌려줌

    /* rec에 있는 모든 학생 정보를 출력함*/
    while (fread(&rec, sizeof(rec), 1, fp)) {
        print_record(&rec);
    }

    /* 파일을 닫아줌*/
    fclose(fp);
}