/*
execlp를 사용해서 컴파일하는 예제
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("temp.c 파일을 컴파일합니다.\n");

    // execlp를 사용하여 gcc를 호출
    if (execlp("gcc", "gcc", "temp.c", "-o", "temp", (char *)NULL) == -1) {
        perror("execlp 실패");
        exit(EXIT_FAILURE);
    }

    // execlp는 성공적으로 호출되면 아래 코드가 실행되지 않음
    printf("컴파일이 완료되었습니다.\n");

    return 0;
}

/*
execle를 사용한 방법
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // 환경 변수 설정
    char *envp[] = {
        "PATH=/usr/bin:/bin",  // PATH 환경 변수 (gcc 실행을 위해 필요)
        NULL                  // 환경 변수 배열 종료
    };

    printf("temp.c 파일을 컴파일합니다.\n");

    // execle를 사용하여 gcc 실행
    if (execle("/usr/bin/gcc", "gcc", "temp.c", "-o", "temp", (char *)NULL, envp) == -1) {
        perror("execle 실패");
        exit(EXIT_FAILURE);
    }

    // execle 호출이 성공하면 이 아래 코드는 실행되지 않음
    printf("컴파일이 완료되었습니다.\n");

    return 0;
}

/*
execve를 사용한 방법
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // 실행할 프로그램의 경로
    const char *path = "/usr/bin/gcc";

    // 프로그램 인자 리스트 (argv 배열)
    char *argv[] = {
        "gcc",       // 프로그램 이름 (gcc)
        "temp.c",    // 컴파일할 소스 파일
        "-o",        // 출력 파일 옵션
        "temp",      // 출력 파일 이름
        NULL         // 인자 리스트 종료
    };

    // 환경 변수 리스트 (envp 배열)
    char *envp[] = {
        "PATH=/usr/bin:/bin",  // PATH 환경 변수
        NULL                  // 환경 변수 배열 종료
    };

    printf("temp.c 파일을 컴파일합니다.\n");

    // execve 호출
    if (execve(path, argv, envp) == -1) {
        perror("execve 실패");
        exit(EXIT_FAILURE);
    }

    // execve가 성공하면 이 아래 코드는 실행되지 않음
    printf("컴파일이 완료되었습니다.\n");

    return 0;
}

/*
execvp 방법
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // 프로그램 인자 리스트 (argv 배열)
    char *argv[] = {
        "gcc",       // 실행할 프로그램 이름
        "temp.c",    // 컴파일할 소스 파일
        "-o",        // 출력 파일 옵션
        "temp",      // 출력 파일 이름
        NULL         // 인자 리스트 종료
    };

    printf("temp.c 파일을 컴파일합니다.\n");

    // execvp 호출
    if (execvp("gcc", argv) == -1) {
        perror("execvp 실패");
        exit(EXIT_FAILURE);
    }

    // execvp가 성공하면 이 아래 코드는 실행되지 않음
    printf("컴파일이 완료되었습니다.\n");

    return 0;
}