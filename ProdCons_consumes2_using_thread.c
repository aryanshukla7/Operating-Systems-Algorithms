#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10
#define ITER 10
int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutexBuffer;
sem_t semEmpty, semFull;
int producerFinished = 0;

void* producer(){
    for(int i = 0; i < ITER; i++){
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = count;
        printf("Producer inserted %d \n", buffer[count]);
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
    producerFinished = 1;
    printf("Producer finished producing\n");
}

void* consumer(){
    for(int i = 0; i < ITER; i++){
        if(producerFinished && (count <= 1)){
            printf("Producer stopped producing, currently buffer has %d elements\n", count);
            break;
        }
        sem_wait(&semFull);
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        printf("Consumer consumed %d %d\n", buffer[count-1], buffer[count-2]);
        count -= 2;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        sem_post(&semEmpty);
    }
    printf("consumer finished consuming\n");
}

int main(){
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFER_SIZE);
    sem_init(&semFull, 0, 0);
    if(pthread_create(&thread1, NULL, &producer, NULL) != 0) {
        perror("Failed to create producer thread");
    };
    if(pthread_create(&thread2, NULL, &consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
}