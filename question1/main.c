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

    printf("Valeur initiale de v_globale : %d\n", v_globale);

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
            printf("[fils] PID=%d, PPID=%d, v_globale=%d\n", getpid(), getppid(), v_globale);
            printf("[fils] <%d> : i=%d, v_globale=%d\n", getpid(), i, v_globale);
        }
    }
    else
    {
        // Processus père
        for (i = 0; i < NOMBRE_DE_TOURS; i++)
        {
            v_globale *= 2;
            printf("[pere] PID=%d, PPID=%d, v_globale=%d\n", getpid(), getppid(), v_globale);
            printf("[pere] <%d> : i=%d, v_globale=%d\n", getpid(), i, v_globale);
        }

        wait(NULL);
    }

    printf("** Fin du processus <%d>, v_globale=%d **\n", getpid(), v_globale);
    return 0;
}
