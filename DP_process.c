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
#define RUNS 7

typedef struct {
    sem_t forks[NUM_PHILOSOPHERS];
    pthread_mutex_t mutex;
} SHARED;

SHARED* smp;

void philosopher(int arg){
    int p_num = arg;
    for(int i = 0; i < RUNS; i++){
        if(p_num < NUM_PHILOSOPHERS-1){
            sem_wait(&(smp->forks[p_num]));
            sem_wait(&(smp->forks[(p_num+1)%NUM_PHILOSOPHERS]));
            printf("Philosopher %d is eating\n", p_num);
            sem_post(&(smp->forks[(p_num+1)%NUM_PHILOSOPHERS]));
            sem_post(&(smp->forks[p_num]));
        } else {
            sem_wait(&(smp->forks[(p_num+1)%NUM_PHILOSOPHERS]));
            sem_wait(&(smp->forks[p_num]));
            printf("Philosopher %d is eating\n", p_num);
            sem_post(&(smp->forks[p_num])); 
            sem_post(&(smp->forks[(p_num+1)%NUM_PHILOSOPHERS]));
        }
    }
}


int main() {
    int shmid = shmget(1234, sizeof(SHARED), IPC_CREAT | 0777);
    if(shmid < 0){
        perror("Failed To Get Shared Memory\n");
        exit(0);
    }
    smp = (SHARED*)shmat(shmid, (void*)0, 0);
    if(smp == (void*)-1){
        perror("Shared Memory Attach Failed\n");
        exit(0);
    }
    pthread_mutex_init(&(smp->mutex), NULL);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        sem_init(&(smp->forks[i]), 1, 1);
    }

    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        int rc = fork();
        if(rc == 0){
            printf("I = %d\n", i);
            philosopher(i);
        } else {
            wait(NULL);
        }
    }

    pthread_mutex_destroy(&(smp->mutex));
    for(int i = 0; i < NUM_PHILOSOPHERS; i++){
        sem_destroy(&(smp->forks[i]));
    }
    return 0;
}