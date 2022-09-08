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

void* producer(void* args) {
    while(1){
        // Produce
        int x = rand()%100;

        // Add to the buffer
        buffer[count] = x;
        count++;
    }
}

void* consumer(void* args) {
    // Remove from Buffer
    int y = buffer[count - 1];

    // Consume
    printf("Got %d\n", y);

}


int main(){
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
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
    return 0;
}