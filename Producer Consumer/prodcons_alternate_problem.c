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

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutexBuffer;


void* producer(){
    while(1){
        
        if(count < BUFFER_SIZE){
            pthread_mutex_lock(&mutexBuffer);
            buffer[count] = count;
            printf("Producer inserted %d \n", buffer[count]);
            count++;
            pthread_mutex_unlock(&mutexBuffer);
        }
        
    }
}

void* consumer(){
    while(1){
        pthread_mutex_lock(&mutexBuffer);
        if(count > 0){
            printf("Consumer consumed %d\n", buffer[count-1]);
            count--;
        }
        pthread_mutex_unlock(&mutexBuffer);
    }
}

int main(){
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutexBuffer, NULL);

    if(pthread_create(&thread1, NULL, &producer, NULL) != 0) {
        perror("Failed to create producer thread");
    };
    if(pthread_create(&thread2, NULL, &consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&mutexBuffer);
}