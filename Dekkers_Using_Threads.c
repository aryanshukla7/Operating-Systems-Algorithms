#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 4
#define M 1000000

int thread1_wants_to_enter;
int thread2_wants_to_enter;
int turn;
int sharedVar;

// turn = 1 -> thread1
// turn = 2 -> thread2


void* routine1(){
    thread1_wants_to_enter = 1;
    while(thread2_wants_to_enter){
        if(turn == 2){
            thread1_wants_to_enter = 0;
            while(turn == 2)
                    ;
        }
    }
    for(int i = 0; i < M; i++){
        sharedVar += 1;
    }
    thread1_wants_to_enter = 0;
    turn = 2;
    return NULL;
}

void* routine2(){
    thread2_wants_to_enter = 1;
    while(thread1_wants_to_enter){
        if(turn == 1){
            thread2_wants_to_enter = 0;
            while(turn == 1)
                    ;
        }
    }
    for(int i = 0; i < M; i++){
        sharedVar += 1;
    }
    thread2_wants_to_enter = 0;
    turn = 1;
    return NULL;
}


int main(){
    pthread_t thread1, thread2;
    int t1_id, t2_id;
    printf("Before Threads Start...\n");
    sharedVar = 0;
    thread1_wants_to_enter = 0;
    thread2_wants_to_enter = 0;
    turn = 1;

    t1_id = pthread_create(&thread1, (void*)NULL, routine1, (void*)NULL);
    t2_id = pthread_create(&thread2, (void*)NULL, routine2, (void*)NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Final value is : %d\n",sharedVar);
    return 0;
}