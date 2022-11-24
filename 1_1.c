#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define count 323

int A, B, C;

void *countA(void *arg)
{
    int i=1;
    struct timeval start, end;

    pthread_setschedparam(pthread_self(), SCHED_OTHER, 0);

    gettimeofday(&start, NULL);

    while(i<=count){
        A++;
        i++;
    }

    gettimeofday(&end, NULL);

    unsigned long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Thread_1 Completed in %ld Microseconds\n", elapsed);

    pthread_exit(NULL);
}

void *countB(void *arg)
{
    int j=1;
    struct timeval start, end;

    pthread_setschedparam(pthread_self(), SCHED_RR, 0);


    gettimeofday(&start, NULL);

    
    while(j<=count){
        B++;
        j++;
    }

    gettimeofday(&end, NULL);

    unsigned long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Thread_2 completed in %ld Microseconds\n", elapsed);

    pthread_exit(NULL);
}

void *countC(void *arg)
{
    int k = 1;
    struct timeval start, end;

    pthread_setschedparam(pthread_self(), SCHED_FIFO, 0);

    gettimeofday(&start, NULL);

    
    while(k<=count){
        C++;
        k++;
    }

    gettimeofday(&end, NULL);

    unsigned long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Thread_3 completed in %ld Microseconds\n", elapsed);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid1, tid2, tid3;

    if (pthread_create(&tid1, NULL, countA, NULL) != 0)
    {
        fprintf(stderr, "Unable to Create thread_1\n");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, countB, NULL) != 0)
    {
        fprintf(stderr, "Unable to Create thread_2\n");
        exit(1);
    }

    if (pthread_create(&tid3, NULL, countC, NULL) != 0)
    {
        fprintf(stderr, "Unable to Create thread_3\n");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    printf("Thread_1 Value: %d\n", A);
    printf("Thread_2 Value: %d\n", B);
    printf("Thread_3 Value: %d\n", C);

    return 0;
}