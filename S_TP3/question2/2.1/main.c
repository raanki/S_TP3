#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NOMBRE_DE_TOURS 5

int v_globale = 100;

int main()
{
    int pid;
    int i;

    printf("Valeur initiale de v_globale : %d", v_globale);
    fflush(stdout);

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur de fork");
        return 1;
    }

    if (pid == 0)
    {
        // Processus fils
        for (i = 0; i < NOMBRE_DE_TOURS; i++)
        {
            v_globale += 10;
            printf("[fils] PID=%d, PPID=%d, v_globale=%d", getpid(), getppid(), v_globale);
            fflush(stdout);
            printf("[fils] <%d> : i=%d, v_globale=%d", getpid(), i, v_globale);
            fflush(stdout);
        }
    }
    else
    {
        // Processus père
        for (i = 0; i < NOMBRE_DE_TOURS; i++)
        {
            v_globale *= 2;
            printf("[pere] PID=%d, PPID=%d, v_globale=%d", getpid(), getppid(), v_globale);
            fflush(stdout);
            printf("[pere] <%d> : i=%d, v_globale=%d", getpid(), i, v_globale);
            fflush(stdout);
        }

        wait(NULL); // Attendre que le fils termine
    }

    printf("** Fin du processus <%d>, v_globale=%d **", getpid(), v_globale);
    fflush(stdout);
    return 0;
}
