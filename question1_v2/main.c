#include <stdio.h>
#include <pthread.h>

#define NOMBRE_DE_TOURS 5

int v_globale = 100;
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;

void* fonction_fils(void* arg)
{
    int i;
    for (i = 0; i < NOMBRE_DE_TOURS; i++)
    {
        pthread_mutex_lock(&verrou);
        v_globale += 10;
        printf("[fils] Thread=%ld, v_globale=%d\n", pthread_self(), v_globale);
        pthread_mutex_unlock(&verrou);
    }
    return NULL;
}

void* fonction_pere(void* arg)
{
    int i;
    for (i = 0; i < NOMBRE_DE_TOURS; i++)
    {
        pthread_mutex_lock(&verrou);
        v_globale *= 2;
        printf("[pere] Thread=%ld, v_globale=%d\n", pthread_self(), v_globale);
        pthread_mutex_unlock(&verrou);
    }
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    printf("Valeur initiale de v_globale : %d\n", v_globale);

    pthread_create(&thread1, NULL, fonction_fils, NULL);
    pthread_create(&thread2, NULL, fonction_pere, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("** Fin du processus, v_globale=%d **\n", v_globale);
    
    pthread_mutex_destroy(&verrou);
    return 0;
}

