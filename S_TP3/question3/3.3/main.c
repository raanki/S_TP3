#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define NB_READ 20

#define P(sem) sem_wait(&sem)
#define V(sem) sem_post(&sem)

int tampon[N];
int indexEcriture = 0;
int indexLecture = 0;

sem_t mutexLecture, mutexEcriture, placesDisponibles, placesOccupees;

void initialiser_semaphore(void)
{
    sem_init(&mutexLecture, 0, 1);
    sem_init(&mutexEcriture, 0, 1);
    sem_init(&placesDisponibles, 0, N);
    sem_init(&placesOccupees, 0, 0);
}

void ecrire_tampon(int data)
{
    P(placesDisponibles);
    P(mutexEcriture);
    tampon[indexEcriture] = data;
    indexEcriture = (indexEcriture + 1) % N;
    V(mutexEcriture);
    V(placesOccupees);
}

int lecture_tampon(void)
{
    int data;
    P(placesOccupees);
    P(mutexLecture);
    data = tampon[indexLecture];
    indexLecture = (indexLecture + 1) % N;
    V(mutexLecture);
    V(placesDisponibles);
    return data;
}

void* thread_ecrivain(void* arg)
{
    for(int i = 0; i < NB_READ; i++)
    {
        ecrire_tampon(i);
        printf("Écrit : %d\n", i);
    }
    return NULL;
}

void* thread_lecteur(void* arg)
{
    for(int i = 0; i < 20; i++)
    {
        int data = lecture_tampon();
        printf("Lu : %d\n", data);
    }
    return NULL;
}

int main()
{
    pthread_t ecrivain, lecteur;

    initialiser_semaphore();

    pthread_create(&ecrivain, NULL, thread_ecrivain, NULL);
    pthread_create(&lecteur, NULL, thread_lecteur, NULL);

    pthread_join(ecrivain, NULL);
    pthread_join(lecteur, NULL);

    sem_destroy(&mutexLecture);
    sem_destroy(&mutexEcriture);
    sem_destroy(&placesDisponibles);
    sem_destroy(&placesOccupees);

    return 0;
}
