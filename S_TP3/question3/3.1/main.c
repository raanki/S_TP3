#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2;
//faire une version sleep et mutex
//faire un
void* thread1_function(void* arg)
{
    printf("je ");
    sem_post(&sem2);
    sem_wait(&sem1);

    printf("mes ");
    return NULL;
}

void* thread2_function(void* arg)
{
    sem_wait(&sem2);
    printf("synchronise ");
    sem_post(&sem1);
    printf("threads\n");
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
