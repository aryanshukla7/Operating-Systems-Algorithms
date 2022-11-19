#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define M 1000000

int sharedVar = 0;
pthread_mutex_t mutex;

void* routine(){
    for(int i = 0; i < M; i++){
        pthread_mutex_lock(&mutex);
        sharedVar++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(){
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    if(pthread_create(&thread1, NULL, &routine, NULL) != 0) {
        return 1;
    }
    if(pthread_create(&thread2, NULL, &routine, NULL) != 0) {
        return 2;
    }
    if(pthread_join(thread1, NULL) != 0){
        return 3;
    }
    if(pthread_join(thread2, NULL) != 0){
        return 4;
    }
    pthread_mutex_destroy(&mutex);
    printf("Final Value of sharedVar = %d\n", sharedVar);
    return 0;
}