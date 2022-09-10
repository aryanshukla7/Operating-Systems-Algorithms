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

#define BUFFER_SIZE 5
#define ITER 12
// int buffer[BUFFER_SIZE];

typedef struct {
    sem_t mutexBuffer;
    sem_t semEmpty, semFull;
    int producerFinished;
    int buffer[BUFFER_SIZE];
    int count;
} SHARED;


SHARED* smp;


void* producer(){
    for(int i = 0; i < ITER; i++){
        sem_wait(&(smp->semEmpty));
        int val = -1;
        sem_getvalue(&(smp->semEmpty), &val);
        sem_getvalue(&(smp->mutexBuffer), &val);
        
        sem_wait(&(smp->mutexBuffer));
        smp->buffer[smp->count] = smp->count;
        printf("Producer inserted %d \n", smp->buffer[smp->count]);
        (smp->count)++;
        if(i == ITER-1)
        smp->producerFinished = 1;
        // pthread_mutex_unlock(&(smp->mutexBuffer));
        sem_post(&(smp->mutexBuffer));
        sem_getvalue(&smp->mutexBuffer,&val);
        // printf("OFnsef%d\n",val);
        sem_post(&(smp->semFull));
        sleep(2);
    }
    printf("Producer finished producing\n");
}

void* consumer(){
    for(int i = 0; i < ITER; i++){
        if((smp->producerFinished) && ((smp->count) <= 1)){
            printf("Producer stopped producing, currently buffer has %d elements\n", smp->count);
            break;
        }
        int val = -1;
        sem_getvalue(&(smp->semFull), &val);
        if(smp->producerFinished && smp->count < 2){
            break;
        }
        sem_wait(&(smp->semFull));
        if(smp->producerFinished && smp->count < 2){
            break;
        }
        sem_wait(&(smp->semFull));
        // pthread_mutex_lock(&(smp->mutexBuffer));
        sem_getvalue(&smp->mutexBuffer,&val);
        sem_wait(&(smp->mutexBuffer));
        printf("Consumer consumed %d %d\n", smp->buffer[(smp->count)-1], smp->buffer[(smp->count)-2]);
        (smp->count) -= 2;
        // pthread_mutex_unlock(&(smp->mutexBuffer));
        sem_post(&(smp->mutexBuffer));
        sem_post(&(smp->semEmpty));
        sem_post(&(smp->semEmpty));
    }
    printf("consumer finished consuming\n");
}

int main(){
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
    sem_init(&(smp->mutexBuffer), 1, 1);
    sem_init(&(smp->semEmpty), 1, BUFFER_SIZE);
    sem_init(&(smp->semFull), 1, 0);
    smp->count = 0;
    smp->producerFinished = 0;
    int rc = fork();
    if(rc < 0){
        perror("Forking Failed\n");
        exit(0);
    } else if (0 == rc){
        printf("Child Process\n");
        consumer();
    } else {
        printf("Parent Process\n");
        producer();
        wait(NULL);
        shmdt(&smp);
        shmctl(shmid, IPC_RMID, NULL);
    }
    sem_destroy(&(smp->semEmpty));
    sem_destroy(&(smp->semFull));
    sem_destroy(&(smp->mutexBuffer));
    // pthread_mutex_destroy(&(smp->mutexBuffer));
}