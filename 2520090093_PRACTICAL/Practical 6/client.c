#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define BUFFER_SIZE 512

int main()
{
    char message[BUFFER_SIZE];
    char client_fifo[100];
    char buffer[BUFFER_SIZE];

    pid_t pid = getpid();

    snprintf(client_fifo,
             sizeof(client_fifo),
             "/tmp/client_%d_fifo",
             pid);

    if (mkfifo(client_fifo, 0666) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Client started. PID = %d\n", pid);

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    message[strcspn(message, "\n")] = '\0';

    int server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("Opening server FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    char full_message[BUFFER_SIZE];

    snprintf(full_message,
             sizeof(full_message),
             "%d|%s",
             pid,
             message);

    write(server_fd,
          full_message,
          strlen(full_message) + 1);

    close(server_fd);

    printf("Message sent to server.\n");
    printf("Waiting for response...\n");

    int client_fd = open(client_fifo, O_RDONLY);

    if (client_fd == -1)
    {
        perror("Opening client FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes = read(client_fd,
                         buffer,
                         sizeof(buffer) - 1);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        printf("Server response: %s\n", buffer);
    }

    close(client_fd);

    unlink(client_fifo);

    return 0;
}
