#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2;

#define P(sem) sem_wait(&sem)
#define V(sem) sem_post(&sem)

void* thread1(void* arg)
{
    printf("je ");
    V(sem1);

    P(sem2);
    printf("mes ");
    V(sem1);
    return NULL;
}

void* thread2(void* arg)
{
    P(sem1);
    printf("synchronise ");
    V(sem2);
    
    P(sem1);
    printf("threads");
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    
    return 0;
}
