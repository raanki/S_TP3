#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1(void* arg)
{
    pthread_mutex_lock(&mutex1);
    printf("je ");
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex1);
    printf("mes ");
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

void* thread2(void* arg)
{
    pthread_mutex_lock(&mutex2);
    printf("synchronise ");
    pthread_mutex_unlock(&mutex1);

    pthread_mutex_lock(&mutex2);
    printf("threads\n");
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_lock(&mutex2);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
