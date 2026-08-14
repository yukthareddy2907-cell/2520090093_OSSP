#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

// Function to enable character-by-character input
void enableRawMode(struct termios *original)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, original);
    raw = *original;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Function to restore normal terminal mode
void disableRawMode(struct termios *original)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}

int main()
{
    char input[BUFFER_SIZE];
    int index;
    char ch;
    struct termios original;

    while (1)
    {
        index = 0;

        printf("myshell> ");
        fflush(stdout);

        enableRawMode(&original);

        while (1)
        {
            read(STDIN_FILENO, &ch, 1);

            // Enter key
            if (ch == '\n' || ch == '\r')
            {
                input[index] = '\0';
                printf("\n");
                break;
            }

            // Backspace key
            else if (ch == 127 || ch == '\b')
            {
                if (index > 0)
                {
                    index--;
                    printf("\b \b");
                    fflush(stdout);
                }
            }

            // Ctrl+D
            else if (ch == 4)
            {
                disableRawMode(&original);
                printf("\n");
                printf("Exiting shell...\n");
                return 0;
            }

            // Normal characters
            else if (index < BUFFER_SIZE - 1)
            {
                input[index++] = ch;
                printf("%c", ch);
                fflush(stdout);
            }
        }

        disableRawMode(&original);

        // Ignore empty input
        if (index == 0)
        {
            continue;
        }

        // Exit condition
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        // Create child process
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            continue;
        }

        if (pid == 0)
        {
            // Convert input into command and arguments
            char *args[20];
            int count = 0;

            char *token = strtok(input, " ");

            while (token != NULL && count < 19)
            {
                args[count++] = token;
                token = strtok(NULL, " ");
            }

            args[count] = NULL;

            // Execute command
            execvp(args[0], args);

            // If execvp fails
            perror("Command execution failed");
            exit(1);
        }
        else
        {
            // Parent waits for child
            wait(NULL);
        }
    }

    return 0;
}
