#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void child_handler(int sig)
{
    int status;
    waitpid(-1, &status, 0);

    printf("SIGCHLD received\n");
    printf("Child process completed\n");
}

int main()
{
    signal(SIGCHLD, child_handler);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process started\n");
        sleep(3);
        printf("Child process exiting\n");
        exit(0);
    }
    else
    {
        printf("Parent process waiting for SIGCHLD...\n");

        sleep(5);

        printf("Parent process completed\n");
    }

    return 0;
}
