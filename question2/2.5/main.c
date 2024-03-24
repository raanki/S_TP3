#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid = fork();

    if (pid == -1)
    {
        perror("Erreur lors de la création du processus fils");
        return 1;
    }
    else if (pid == 0)
    {
        printf("[fils] PID=%d, PPID=%d\n", getpid(), getppid());
        int sleepTime = 42 * getpid();
        printf("[fils] Je m’endors pour %dus...\n", sleepTime);
        usleep(sleepTime);
        printf("[fils] Je suis reveille !\n");
    }
    else
    {
        printf("[pere] PID=%d, PID du fils=%d\n", getpid(), pid);
        printf("[pere] En attente de mon fils...\n");
        wait(NULL);
    }

    printf("** Fin du process <%d> **\n", getpid());
    return 0;
}
