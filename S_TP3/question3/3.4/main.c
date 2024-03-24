#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5 // Nombre de philosophes et de baguettes
#define MAX_WAIT_TIME 5 // Temps maximal avant mort
#define TIME_TO_EAT 1 // seconde
#define TIME_TO_THINK 1 // seconde
#define CYCLE 3 // seconde



pthread_mutex_t mutex;
pthread_mutex_t monitor_mutex;
pthread_cond_t monitor_cond;
sem_t baguettes[N];
pthread_t philosophes[N];
time_t dernier_repas[N];
time_t debut;
int monitor_actif = 1;

void prendre_baguettes(int i)
{
    pthread_mutex_lock(&mutex);
    sem_wait(&baguettes[i]);
    sem_wait(&baguettes[(i + 1) % N]);
    pthread_mutex_unlock(&mutex);
}

void poser_baguettes(int i)
{
    sem_post(&baguettes[i]);
    sem_post(&baguettes[(i + 1) % N]);
}

void* monitor(void* arg)
{
    while (monitor_actif)
    {
        pthread_mutex_lock(&monitor_mutex);
        time_t maintenant = time(NULL);
        for (int i = 0; i < N; ++i)
        {
            if (maintenant - dernier_repas[i] > MAX_WAIT_TIME) 
            {
                printf("%lds Philosophe %d n'a pas mangé depuis %d secondes.\n", time(NULL) - debut, i, MAX_WAIT_TIME);
                exit(0);
            }
        }
        pthread_mutex_unlock(&monitor_mutex);
    }
    return NULL;
}

void penser(int i)
{
    printf("%lds Philosophe %d pense.\n", time(NULL) - debut, i);
    sleep(TIME_TO_THINK);
}

void manger(int i)
{
    printf("%lds Philosophe %d mange.\n", time(NULL) - debut, i);
    dernier_repas[i] = time(NULL);
    sleep(TIME_TO_EAT);
}


void* philosophe(void* num)
{
    int i = *(int*)num;

    dernier_repas[i] = time(NULL);

    int cycles = CYCLE;
    for(int c = 0; c < cycles; c++)
    {
        penser(i);
        prendre_baguettes(i);
        manger(i);
        poser_baguettes(i);
    }

    return NULL;
}

int main()
{
    pthread_t mon;
    debut = time(NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&monitor_mutex, NULL);
    pthread_cond_init(&monitor_cond, NULL);

    for(int i = 0; i < N; i++)
    {
        sem_init(&baguettes[i], 0, 1);
    }

    int ids[N];
    for(int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&philosophes[i], NULL, philosophe, &ids[i]);
    }

    pthread_create(&mon, NULL, monitor, NULL);

    for(int i = 0; i < N; i++)
    {
        pthread_join(philosophes[i], NULL);
    }

    monitor_actif = 0;
    pthread_cond_signal(&monitor_cond);
    pthread_join(mon, NULL);
    printf("\n%lds Chaque philosophe a mangé %d fois.\n-- Fin --\n", time(NULL) - debut, N);

    for(int i = 0; i < N; i++)
    {
        sem_destroy(&baguettes[i]);
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&monitor_mutex);
    pthread_cond_destroy(&monitor_cond);

    return 0;
}
