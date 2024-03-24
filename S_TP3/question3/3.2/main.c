#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int N = 3;
int count = 0;

void *thread_function(void *id)
{
    long tid = (long)id;

    sleep(rand() % 5 + 1);

    pthread_mutex_lock(&mutex);
    count++;
    printf("[thread %ld] Point atteint\n", tid);
    if (count == N)
    {
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *barrier_thread_function(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (count < N)
    {
        printf("[b_thread] Barrière atteinte, en attente...\n");
        pthread_cond_wait(&cond, &mutex);
    }
    printf("[b_thread] Je peux continuer !\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t threads[N];
    pthread_t b_thread;

    pthread_create(&b_thread, NULL, barrier_thread_function, NULL);

    for (long i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, (void *)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_join(b_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
