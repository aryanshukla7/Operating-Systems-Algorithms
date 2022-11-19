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
pthread_mutex_t mutexPhilosopher;
pthread_cond_t condStatus;


typedef struct {
    int fork;
    int reqf;
    sem_t dirty;
} FORKS;

FORKS forks[NUM_PHILOSOPHERS];

void request(int forkID){
    sem_wait(&forks[forkID]);
}

void releaseFork(int forkID){
    sem_post(&forks[forkID]);
}

void* philosopher(void* arg){
    int p_num = *((int*)arg);
    pthread_cond_wait(&condStatus, &mutexPhilosopher);
    pthread_mutex_lock(&mutexPhilosopher);
    request(p_num);
    request((p_num+1)%NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&mutexPhilosopher);
    printf("Philosopher %d is eating\n", p_num);
    releaseFork(p_num);
    releaseFork((p_num+1)%NUM_PHILOSOPHERS);
    pthread_cond_broadcast(&condStatus);
}


int main() {
    pthread_t p[NUM_PHILOSOPHERS];
    pthread_mutex_init(&mutexPhilosopher, NULL);

    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        sem_init(&forks[i].dirty, 0, 1);
    }

    int args[NUM_PHILOSOPHERS];
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        args[i] = i;
    }

    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        if(pthread_create(&p[i], NULL, &philosopher, (void*)&args[i]) != 0){
            perror("Failed to create Philosopher thread");
        }
    }
    

    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        if(pthread_join(&p[i], NULL) != 0){
            perror("Failed to join Philosopher thread");
        }
    }
    return 0;
}