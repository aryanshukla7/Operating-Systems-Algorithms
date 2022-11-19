#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 4

#define M 1000

int main(){
    int shmid = shmget(1234, N*sizeof(int), IPC_CREAT | 0777);
    
}