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

#define THREAD_NUM 2

int buffer[10];
int count = 0;
pthread_mutex_t mutexBuffer;
sem_t semEmpty;
sem_t semFull;

void* producer(void* args) {
    while(1){
        // Produce
        int x = rand()%100;
        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        printf("Produced : %d\n", buffer[count]);
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args) {
    while(1){
        // Remove from Buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        int y = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        // Consume
        printf("Consumed : %d\n", buffer[count]);
    }

}


int main(){
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    pthread_mutex_init(&mutexBuffer, NULL);
    for(int i = 0; i < THREAD_NUM; i++){
        if(i%2 == 0){
            if(pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if(pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
        
    }
    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}