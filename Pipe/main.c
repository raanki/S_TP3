#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256
#define MAX_ARGS 64

void parseCommand(char* cmd, char** args)
{
    char* token;
    token = strtok(cmd, " \n");
    int i = 0;
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

int main()
{
    char cmd1[MAX_CMD_LEN];
    char cmd2[MAX_CMD_LEN];
    char* args1[MAX_ARGS];
    char* args2[MAX_ARGS];
    int pipefd[2];
    pid_t cpid;

    printf("Entrez la première commande: ");
    fgets(cmd1, MAX_CMD_LEN, stdin);
    printf("Entrez la deuxième commande: ");
    fgets(cmd2, MAX_CMD_LEN, stdin);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        parseCommand(cmd1, args1);
        execvp(args1[0], args1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        wait(NULL);

        parseCommand(cmd2, args2);
        execvp(args2[0], args2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    return 0;
}
