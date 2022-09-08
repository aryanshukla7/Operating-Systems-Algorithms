#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define n 5

int tickets[n];
int choosing[n];
int shmid;
int* sharedVariable;


int max(int *arr)
{
    int i, max = arr[0];
    for (i = 1; i < n; i++)
        if (tickets[i] > max)
            return tickets[i];
    return tickets[0];
}
void lock(int x)
{

    choosing[x] = 1;
    int max_ticket = max(tickets);
    tickets[x] = max_ticket + 1;
    choosing[x] = 0;

    for (int k = 0; k < n; ++k)
    {

        while (choosing[k])
        {
            ;
        }

        while (tickets[k] != 0 && (tickets[k], k) < (tickets[x], x))
        {
            ;
        }
    }
}

void unlock(int x)
{
    tickets[x] = 0;
}

void *thread_body(void *arg)
{

    long thread = (long)arg;
    lock(thread);
    printf("%ld thread is in critical section \n", thread);
    *sharedVariable = *sharedVariable + 1;
    printf("Shared Variable: %d\n", *sharedVariable);
    printf("%ld thread is out of critical section \n", thread);
    
    unlock(thread);
    return NULL;
}

int main(int argc, char **argv)
{

    memset((void *)tickets, 0, sizeof(tickets));
    memset((void *)choosing, 0, sizeof(choosing));

    pthread_t threads[n];

    shmid = shmget(1234, 1 * sizeof(int), IPC_CREAT | 0777);
    if (shmid < 0)
    {
        perror("PARENT: Failed to get shared memory segment\n");
        exit(0);
    }
    printf("PARENT: Shared memory obtained\n");
    printf("shmid = %d \t ptr = %p\n", shmid, sharedVariable);
    // Parent process attaches shared segment to its address sprocessNumberce
    sharedVariable = (int *)shmat(shmid, (void *)0, 0);

    if ((void *)-1 == sharedVariable)
    {
        perror("Failed: \n");
        exit(0);
    }
    printf("PARENT %p \n", sharedVariable);
    if (NULL == sharedVariable)
    {
        printf("PARENT: Failed to attach shared memory segment.\n");
        exit(0);
    }

    *sharedVariable = 0;
    for (int i = 0; i < n; ++i)
    {
        pthread_create(&threads[i], NULL, &thread_body, (void *)((long)i));
    }   

    for (int i = 0; i < n; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}