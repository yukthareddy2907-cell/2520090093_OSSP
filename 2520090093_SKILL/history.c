#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    char *input;

    printf("\033[1;32m=== Mini Shell History Test ===\033[0m\n");
    printf("Type commands. Type 'exit' to quit.\n\n");

    while (1)
    {
        input = readline("myshell> ");

        if (input == NULL)
            break;

        if (strlen(input) > 0)
            add_history(input);

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        printf("Command entered: %s\n", input);

        free(input);
    }

    printf("\nCommand History:\n");

    HIST_ENTRY **hist = history_list();

    if (hist != NULL)
    {
        for (int i = 0; hist[i] != NULL; i++)
        {
            printf("%d: %s\n", i + 1, hist[i]->line);
        }
    }

    return 0;
}
