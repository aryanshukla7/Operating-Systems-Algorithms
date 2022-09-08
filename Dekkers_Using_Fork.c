#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 1
#define M 1000000

typedef struct {
    int child_wants_to_enter;
    int parent_wants_to_enter;
    int turn;
    int sharedVar;
} SHARED;

// turn = 0 -> child
// turn = 1 -> parent

SHARED * smp;

void child(){
    smp->child_wants_to_enter = 1;
    while(smp->parent_wants_to_enter){
        if(smp->turn){
            smp->child_wants_to_enter = 0;
            while(smp->turn)
                ;
        }
    }
    smp->sharedVar += 1;
    // printf("Child Process : %d\n", smp->sharedVar);

    smp->child_wants_to_enter = 0;
    smp->turn = 1;
}

void parent(){

    smp->parent_wants_to_enter = 1;
    while(smp->child_wants_to_enter){
        if(!smp->turn){
            smp->parent_wants_to_enter = 0;
            while(!smp->turn)
                ;
        }
    }
    smp->sharedVar += 1;
    // printf("Parent Process : %d\n", smp->sharedVar);
    smp->parent_wants_to_enter = 0;
    smp->turn = 0;
}

int main(){
    int shmid = shmget(1234, sizeof(SHARED), IPC_CREAT | 0777);
    if(shmid < 0){
        perror("Failed TO Get Shared Memory\n");
        exit(0);
    }
    smp = shmat(shmid, (void*)0, 0);
    if(smp == (void*)-1){
        perror("Shared Memory Attach Failed\n");
        exit(0);
    }
    smp->child_wants_to_enter = 0;
    smp->parent_wants_to_enter = 0;
    smp->turn = 0;
    smp->sharedVar = 0;
    
    printf("Initial Value : %d\n", smp->sharedVar);

    int rc = fork();
    if(-1 == rc){
        printf("Fork Failed\n");
        exit(0);
    }
    for(int i = 0; i < M; i++){
        if(0 == rc){
            child();
    } else if(rc > 0){
        parent();
        wait(NULL);
    }
    }
    printf("Final Value : %d\n", smp->sharedVar);
    return 0;
}

