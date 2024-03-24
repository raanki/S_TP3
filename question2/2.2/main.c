#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Main : PID=%d, PPID=%d\n", getpid(), getppid());

    int pid = fork();

    if (pid == -1)
    {
        perror("Erreur lors de la création du processus fils");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Fils : PID=%d, PPID=%d\n", getpid(), getppid());
    }
    else
    {
        printf("Père : PID=%d, PID du fils=%d\n", getpid(), pid);
    }

    if (pid > 0)
    {
        wait(NULL);
    }

    return 0;
}
