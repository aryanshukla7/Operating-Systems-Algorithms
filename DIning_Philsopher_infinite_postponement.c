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

#define NUM_PHILOSOPHERS 5
#define RUNS 100

sem_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;

void* philosopher(void* arg){
    int p_num = *((int*)arg);
    while(1){
        pthread_mutex_lock(&mutex);
        sem_wait(&forks[p_num]);
        int val = -1;
        // might cause infinite postment, beacuse what if every time the same
        // process picks one fork and has to wait for the other
        sem_getvalue(&forks[(p_num+1)%NUM_PHILOSOPHERS], &val);
        if(val == 0){
            sem_post(&forks[p_num]);
            pthread_mutex_lock(&mutex);
        }
        sem_wait(&forks[(p_num+1)%NUM_PHILOSOPHERS]);
        pthread_mutex_unlock(&mutex);
        printf("Philosopher %d is eating\n", p_num);
        sem_post(&forks[(p_num+1)%NUM_PHILOSOPHERS]);
        sem_post(&forks[p_num]);
    }
}


int main() {
    pthread_t p[NUM_PHILOSOPHERS];
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        sem_init(&forks[i], 0, 1);
    }
    int args[NUM_PHILOSOPHERS];
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        args[i] = i;
    }
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        if(pthread_create(&p[i], NULL, &philosopher, (void*)&args[i]) != 0){
        fprintf(stderr, "Unable to create Philosopher Thread %d", args[i]);
    }
    }
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        pthread_join(p[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        sem_destroy(&forks[i]);
    }
    return 0;
}