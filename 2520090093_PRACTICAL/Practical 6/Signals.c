#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO,
              "\nSIGINT received (Ctrl+C)\n",
              27);
    }
    else if (sig == SIGTERM)
    {
        write(STDOUT_FILENO,
              "\nSIGTERM received\n",
              19);
    }
    else if (sig == SIGUSR1)
    {
        write(STDOUT_FILENO,
              "\nSIGUSR1 received\n",
              19);
    }
}

int main()
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = handle_signal;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    printf("Signal handling program started.\n");
    printf("Process ID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (1)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    return 0;
}
