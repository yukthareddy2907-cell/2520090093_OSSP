#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int size = 10 * 1024 * 1024;
    int *data = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
        data[i] = 10;

    printf("Parent PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        printf("Child PID: %d\n", getpid());

        printf("Child BEFORE modification\n");
        sleep(30);

        for (int i = 0; i < size; i++)
            data[i] = 99;

        printf("Child AFTER modification\n");
        sleep(30);

        free(data);
    }
    else {
        printf("Parent waiting...\n");
        sleep(60);

        free(data);
        wait(NULL);
    }

    return 0;
}
