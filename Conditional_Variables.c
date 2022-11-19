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

#define RUNS 5

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

void* fuelFilling() {
    for(int i = 0; i < RUNS; i++){
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Fuel filled: %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car() {
    pthread_mutex_lock(&mutexFuel);
    while(fuel < 40){
        printf("No Fuel. Waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        // Equivalent to:
        // pthread_mutex_unlock(&mutexFuel);
        // wait for signal on condFuel
        // pthread_mutex_lock(&mutexFuel);
    }
    fuel -= 40;
    printf("Got Fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main() {
    pthread_t th[6];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);
    for(int i = 0; i < 6; i++){
        if(i < 4){
            if(pthread_create(&th[i], NULL, &car, NULL) != 0){
                perror("Failed to create thread");
            }
        } else {
            if(pthread_create(&th[i], NULL, &fuelFilling, NULL) != 0){
                perror("Failed to create thread");
            }
        }
    }
    for(int i = 0; i < 6; i++){
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}