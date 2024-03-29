#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define NB_OPERATIONS 10
#define NB_ECRIVAINS_ET_LECTEURS 20

#define P(sem) sem_wait(&(sem))
#define V(sem) sem_post(&(sem))

int tampon[N];
int indexEcriture = 0, indexLecture = 0;
sem_t Lib, Occ, mutexE, mutexL;

void initialiser_semaphores()
{
    sem_init(&Lib, 0, N);
    sem_init(&Occ, 0, 0);
    sem_init(&mutexE, 0, 1);
    sem_init(&mutexL, 0, 1);
}

void ecrire_tampon(int data)
{
    P(Lib);
    P(mutexE);
    tampon[indexEcriture] = data;
    indexEcriture = (indexEcriture + 1) % N;
    V(mutexE);
    V(Occ);
}

int lire_tampon()
{
    P(Occ);
    P(mutexL);
    int data = tampon[indexLecture];
    indexLecture = (indexLecture + 1) % N;
    V(mutexL);
    V(Lib);
    return data;
}

void* thread_ecrivain(void* arg)
{
    int id = *(int*)arg;
    for (int i = 0; i < NB_OPERATIONS; i++)
    {
        ecrire_tampon(i);
        printf("Écrivain %d écrit %d\n", id, i + id);
    }
    return NULL;
}

void* thread_lecteur(void* arg)
{
    int id = *(int*)arg;
    for (int i = 0; i < NB_OPERATIONS; i++)
    {
        int data = lire_tampon();
        printf("Lecteur %d lit %d\n", id, data);
    }
    return NULL;
}

int main()
{
    pthread_t ecrivains[NB_ECRIVAINS_ET_LECTEURS], lecteurs[NB_ECRIVAINS_ET_LECTEURS];
    int idsE[NB_ECRIVAINS_ET_LECTEURS], idsL[NB_ECRIVAINS_ET_LECTEURS];

    initialiser_semaphores();

    for (int i = 0; i < NB_ECRIVAINS_ET_LECTEURS; i++)
    {
        idsE[i] = i;
        pthread_create(&ecrivains[i], NULL, thread_ecrivain, &idsE[i]);
    }
    for (int i = 0; i < NB_ECRIVAINS_ET_LECTEURS; i++)
    {
        idsL[i] = i;
        pthread_create(&lecteurs[i], NULL, thread_lecteur, &idsL[i]);
    }

    for (int i = 0; i < NB_ECRIVAINS_ET_LECTEURS; i++)
    {
        pthread_join(ecrivains[i], NULL);
    }
    for (int i = 0; i < NB_ECRIVAINS_ET_LECTEURS; i++)
    {
        pthread_join(lecteurs[i], NULL);
    }

    sem_destroy(&Lib);
    sem_destroy(&Occ);
    sem_destroy(&mutexE);
    sem_destroy(&mutexL);

    return 0;
}
