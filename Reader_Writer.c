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

#define THREAD_NUM 100
int sharedVar = 0;
pthread_mutex_t mutex;
sem_t reading;
sem_t writing;

void* reader(){
    sem_wait(&writing);
    printf("Started Reading : %d\n", sharedVar);
    printf("Finished Reading : %d\n", sharedVar);
    sem_post(&writing);
    
}

void* writer(){
    // pthread_mutex_lock(&mutex);
    sem_wait(&reading);
    sem_wait(&writing);
    printf("Started Writing : %d\n", sharedVar);
    sharedVar++;
    printf("Finished Writing : %d\n", sharedVar);
    sem_post(&writing);
    sem_post(&reading);
    // pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t th[THREAD_NUM];
    sem_init(&reading, 0, 1);
    sem_init(&reading, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < THREAD_NUM; i++){
        if(i%2){
            if(pthread_create(&th[i], NULL, &reader, NULL) != 0) {
                perror("Failed to create thread");
            };
        } else {
            if(pthread_create(&th[i], NULL, &writer, NULL) != 0) {
                perror("Failed to create thread");
            };
        }
    }
    sleep(1);
    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_join(th[i], NULL) != 0){
            perror("Failed to join");
        };
    }
    sem_destroy(&reading);
    sem_destroy(&writing);
    pthread_mutex_destroy(&mutex);
}