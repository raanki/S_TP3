#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    printf("[fils] PID <%d> PPID <%d>\n", getpid(), getppid());
    printf("[fils] commande de lancement : %s\n", argv[0]);
    printf("[fils] Bonjour %s\n", argv[1]);

    return 0;
}
