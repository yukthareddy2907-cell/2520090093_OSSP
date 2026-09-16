#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define BUFFER_SIZE 512

int main()
{
    char buffer[BUFFER_SIZE];

    if (mkfifo(SERVER_FIFO, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    while (1)
    {
        int fd = open(SERVER_FIFO, O_RDONLY);

        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            printf("\nReceived: %s\n", buffer);

            char *separator = strchr(buffer, '|');

            if (separator != NULL)
            {
                *separator = '\0';

                char *client_pid = buffer;
                char *message = separator + 1;

                printf("Client PID : %s\n", client_pid);
                printf("Message    : %s\n", message);

                char response[BUFFER_SIZE];

                snprintf(response,
                         sizeof(response),
                         "Server processed your message: %s",
                         message);

                char client_fifo[100];

                snprintf(client_fifo,
                         sizeof(client_fifo),
                         "/tmp/client_%s_fifo",
                         client_pid);

                int response_fd = open(client_fifo, O_WRONLY);

                if (response_fd != -1)
                {
                    write(response_fd,
                          response,
                          strlen(response) + 1);

                    close(response_fd);

                    printf("Response sent to client %s\n",
                           client_pid);
                }
            }
        }

        close(fd);
    }

    return 0;
}
