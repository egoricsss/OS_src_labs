#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread1(void* arg) {
    int count = 0;
    while(1) {
        printf("Thread 1: intervals = %d\n", ++count);
        sleep(5);
    }
    return NULL;
}

void* thread2(void* arg) {
    int count = 0;
    while(1) {
        printf("Thread 2: intervals = %d\n", ++count);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}