/*
thread 10개를 만들고, 스레드 함수는 한개만 만들어서
1부터 100까지 정수를 10개 단위로 쪼개서 더하도록 하는 내용
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUMBERS_PER_THREAD 10

// 전역 변수: 합계와 뮤텍스
int total_sum = 0;
pthread_mutex_t mutex;

// 스레드 함수
void sum_numbers(void *arg) {
    int start = *(int *)arg; // 파라미터로 받은 시작 숫자
    free(arg); // 동적 메모리 해제
    int end = start + NUMBERS_PER_THREAD - 1;
    int local_sum = 0;

    // 주어진 범위의 숫자를 더함
    for (int i = start; i <= end; i++) {
        local_sum += i;
    }

    // 뮤텍스를 사용하여 total_sum에 추가
    pthread_mutex_lock(&mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;
    
    // 뮤텍스 초기화
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("뮤텍스 초기화 실패\n");
        return 1;
    }   
    
    // 스레드 생성
    for (i = 0; i < NUM_THREADS; i++) {
        int *start = malloc(sizeof(int)); // 시작 숫자를 동적 메모리에 저장
        if (start == NULL) {
            perror("메모리 할당 실패");
            return 1;
        }   
        *start = i * NUMBERS_PER_THREAD + 1; // 각 스레드의 시작 숫자 계산
        
        if (pthread_create(&threads[i], NULL, (void *)sum_numbers, start) != 0) {
            perror("스레드 생성 실패"); 
            return 1;
        }   
    }   
    
    // 모든 스레드가 작업을 완료할 때까지 대기
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }   
    
    // 결과 출력
    printf("1부터 %d까지의 합: %d\n", NUM_THREADS * NUMBERS_PER_THREAD, total_sum);
    
    // 뮤텍스 파괴
    pthread_mutex_destroy(&mutex);
    
    return 0;
}   