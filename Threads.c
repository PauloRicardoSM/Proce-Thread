#include <pthread.h>  
#include <stdio.h>  

int counter = 0;  
pthread_mutex_t lock;  

void* increment(void* arg) {  
    pthread_mutex_lock(&lock);  
    counter++;  // Seção crítica!  
    pthread_mutex_unlock(&lock);  
    return NULL;  
}  

int main() {  
    pthread_t t1, t2;  
    pthread_mutex_init(&lock, NULL);  
    pthread_create(&t1, NULL, increment, NULL);  
    pthread_create(&t2, NULL, increment, NULL);  
    pthread_join(t1, NULL);  
    pthread_join(t2, NULL);  
    printf("Counter: %d\n", counter);  // Sem mutex, pode ser 1 ou 2!  
    return 0;  
}  