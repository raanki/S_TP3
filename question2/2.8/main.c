#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return 0;
    
    char *prenom = argv[1]; 

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
        printf("[fils] Je fais la commande !\n");
        //prog_fils
        printf("[fils] PID <%d> PPID <%d>\n", getpid(), getppid());
        printf("[fils] commande de lancement : %s\n", argv[0]);
        printf("[fils] Bonjour %s\n", argv[1]);

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
