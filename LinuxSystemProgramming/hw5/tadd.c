/*
   컴파일 한 후 ./tadd를 입력하여 프로그램을 실행함.
   프로그램이 실행되면 thread가 두 개 만들어지고, 각각 1부터 50까지의 합과 51부터 100까지의 합을
   구하여 전역변수에 값을 저장함.
   각 thread가 종료된 후에는 전역변수에 저장한 값을 더하여 1부터 100까지의 값을 구하여 출력함.

   (예시)
   $ make
   $ ./tadd
   sum1(1 to 50)=1275, sum2(51 to 100)=3775
   result(1 to 100)=5050

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 각 thread 함수의 결과를 저장하는 전역 변수를 선언 */
int sum1 = 0;
int sum2 = 0;
         
/* 1부터 50까지의 합을 구하는 thread 함수 */
void tadd1()
{       
    int i;
             
    for (i = 1; i <= 50; i++)
    {
        sum1 += i; 
    }

    pthread_exit(NULL);
}   
    
/* 51부터 100까지의 합을 구하는 thread 함수 */
void tadd2()
{
    int i;

    for (i = 51; i <= 100; i++)
    {
        sum2 += i;
    }

    pthread_exit(NULL);
}

{
    pthread_t tid1, tid2;
    int result;
    
    /* 첫번째 thread 생성 (1부터 50까지의 합을 구하는 thread) */
    if (pthread_create(&tid1, NULL, (void *)tadd1, NULL) != 0)
    {
        perror("pthread_create");
        exit(1);
    }   
    
    /* 두번째 thread 생성 (51부터 100까지 합을 구하는 thread) */
    if (pthread_create(&tid2, NULL, (void *)tadd2, NULL) != 0)
    {
        perror("pthread_create");
        exit(1);
    }   
    
    /* 첫 번째 thread의 종료를 기다림 */
    if (pthread_join(tid1, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }   
    
    /* 두 번재 thread의 종료를 기다림 */
    if (pthread_join(tid2, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }   
    
    /* 각 thread에서 구한 1부터 50까지의 합의 결과와 51부터 100까지의 합의 결과를 더하여
       최종적으로 얻고자 하는 1부터 100까지의 합을 구함 */
    result = sum1 + sum2;
    
    /* 결과 출력 */
    printf("sum1(1 to 50)=%d, sum2(51 to 100)=%d\n", sum1, sum2);
    printf("result(1 to 100)=%d\n", result);
} 