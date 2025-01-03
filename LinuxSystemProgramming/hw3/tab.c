#include <stdio.h>
  
/* enum을 이용하여 FALSE는 0, TRUE는 1로 이용할 수 있도록 함*/
enum { FALSE, TRUE };

void conv(char *fname) {
    FILE *src, *dst;
    char fdest[40];
    int ch, first;

    /* 파라미터로 넘겨받은 파일을 read text 모드로 open하고,
       그것을 src에 할당함*/
    if ((src = fopen(fname, "rt")) == NULL) {
        perror("fopen");
        return;
    }

    /* fname 파일을 fdest로 복사하고, 뒤에 .t 확장자를 추가함*/
    strcpy(fdest, fname);
    strcat(fdest, ".t");

    /* fdest를 write text 모드로 open하고, 그것을 dst에 할당함*/
    if ((dst = fopen(fdest, "wt")) == NULL) {
        perror("fopen");
        return;
    }

    first = TRUE;

    /* fgets를 이용하여 src 파일을 End Of File까지 line단위로 읽어옴.
       그 후, 첫번째 line에서 tab이 있다면 tab을 4칸으로 변경하여
       dst에 write함. */
    while ((ch = fgetc(src)) != EOF) {
        if (first && ch == '\t') {
            fputc(' ', dst);
            fputc(' ', dst);
            fputc(' ', dst);
            fputc(' ', dst);
        }
        /* 만약 첫번째 line이 아니거나 tab이 없는 경우, 현재 line을
           dst에 write함.*/
        else {
            fputc(ch, dst);
            if (first)
                first = FALSE;
            if (ch == '\n')
                first = TRUE;
        }
    }

    fclose(src);
    fclose(dst);
}

main(int argc, char *argv[])
{
    /* command line으로 전달받은 모든 argc에 대해서 conv를 진행함*/
    while (--argc) {
        conv(argv[argc]);
    }
}

/* 즉, 이 코드는 어떤 파일에서 tab을 4칸으로 변경하여 결과를 새 파일로
   저장하는 것임*/