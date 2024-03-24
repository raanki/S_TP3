#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NOMBRE_DE_TOURS 5

int v_globale = 100;

int main()
{
    int pid;
    int i;
    unsigned int seed;

    printf("Valeur initiale de v_globale : %d\n", v_globale);

    seed = time(NULL);
    srand(seed);

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur de fork");
        return 1;
    }

    if (pid == 0)
    {
        srand(seed + getpid());
        for (i = 0; i < NOMBRE_DE_TOURS; i++)
        {
            int attente = rand() % 5 + 1;
            printf("[fils] PID=%d, attente=%d secondes\n", getpid(), attente);
            sleep(attente);
            v_globale += 10;
            printf("[fils] PID=%d, PPID=%d, v_globale=%d\n", getpid(), getppid(), v_globale);
        }
    }
    else
    {
        srand(seed + getpid());
        for (i = 0; i < NOMBRE_DE_TOURS; i++)
        {
            int attente = rand() % 5 + 1;
            printf("[pere] PID=%d, attente=%d secondes\n", getpid(), attente);
            sleep(attente);
            v_globale *= 2;
            printf("[pere] PID=%d, PPID=%d, v_globale=%d\n", getpid(), getppid(), v_globale);
        }

        wait(NULL);
    }

    printf("** Fin du processus <%d>, v_globale=%d **\n", getpid(), v_globale);
    return 0;
}
