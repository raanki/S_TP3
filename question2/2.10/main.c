#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h> 

int main(int argc, char **argv)
{
    int i;
    int status;

    if (argc != 3)
    {
        printf("Usage: %s <prenom> <nombre_de_processus_fils>\n", argv[0]);
        return 1;
    }

    char *prenom = argv[1];
    int nombre_fils = atoi(argv[2]);

    for (i = 0; i < nombre_fils; i++)
    {
        int pid = fork();

        if (pid == -1)
        {
            perror("Erreur lors de la création du processus fils");
            return 1;
        }
        else if (pid == 0)
        {
            printf("[fils %d] PID=%d, PPID=%d\n", i+1, getpid(), getppid());
            int sleepTime = 42 * (getpid() % 10);
            printf("[fils %d] Je m’endors pour %dus...\n", i+1, sleepTime);
            usleep(sleepTime);
            printf("[fils %d] Je suis reveille !\n", i+1);
            printf("[fils %d] Je fais la commande !\n", i+1);
            execlp("echo", "echo", "Bonjour", prenom, (char *)NULL);
            perror("execlp a échoué");
            _exit(1);
        }
    }

    printf("[pere] PID=%d, en attente de mes fils...\n", getpid());
    for (i = 0; i < nombre_fils; i++)
    {
        wait(&status);
    }

    printf("** Fin du processus <%d> **\n", getpid());
    return 0;
}
