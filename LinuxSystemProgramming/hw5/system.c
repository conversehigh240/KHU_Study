#include <stdio.h>
#include <stdlib.h>

main()
{
    int status;

    /* 현재 날짜를 출력해줌 */
    if ((status = system("date")) < 0) {
        perror("system");
        exit(1);
    }

    printf("exit status = %d\n", status);

    /* 존재하지 않는 명령어를 실행함 */
    if ((status = system("nosuchcommand")) < 0) {
        perror("system");
        exit(1);
    }

    printf("exit status = %d\n", status);

    /* who, exit 44를 실행함 */
    if ((status = system("who; exit 44")) < 0) {
        perror("system");
        exit(1);
    }

    printf("exit status = %d\n", status);
}