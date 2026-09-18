#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_LEN 500

/* ---------------- TOKEN STRUCTURE ---------------- */

typedef struct
{
    char value[MAX_LEN];
    char type[30];
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;

/* ---------------- ADD TOKEN ---------------- */

void addToken(char *value, char *type)
{
    if (tokenCount >= MAX_TOKENS)
    {
        printf("Too many tokens!\n");
        return;
    }

    strcpy(tokens[tokenCount].value, value);
    strcpy(tokens[tokenCount].type, type);

    tokenCount++;
}

/* ---------------- VARIABLE EXPANSION ---------------- */

void expandVariable(char *input, int *i, char *output)
{
    char variable[100];
    char value[200];

    int j = 0;

    /* Skip '$' */
    (*i)++;

    /* Read variable name */
    while (isalnum(input[*i]) || input[*i] == '_')
    {
        variable[j++] = input[*i];
        (*i)++;
    }

    variable[j] = '\0';

    /* Get environment variable */
    char *envValue = getenv(variable);

    if (envValue != NULL)
    {
        strcpy(value, envValue);
    }
    else
    {
        value[0] = '\0';
    }

    strcat(output, value);
}

/* ---------------- TOKENIZER ---------------- */

int tokenize(char *input)
{
    int i = 0;

    while (input[i] != '\0')
    {
        char buffer[MAX_LEN];
        int j = 0;

        /* Ignore whitespace outside quotes */
        if (isspace(input[i]))
        {
            i++;
            continue;
        }

        /* ---------------- SINGLE QUOTES ---------------- */

        if (input[i] == '\'')
        {
            i++;

            while (input[i] != '\0' && input[i] != '\'')
            {
                buffer[j++] = input[i++];
            }

            /* Unmatched single quote */
            if (input[i] == '\0')
            {
                printf("\nSyntax Error: Unmatched single quote.\n");
                return 0;
            }

            buffer[j] = '\0';

            /* Skip closing quote */
            i++;

            addToken(buffer, "SINGLE_QUOTED");

            continue;
        }

        /* ---------------- DOUBLE QUOTES ---------------- */

        if (input[i] == '"')
        {
            i++;

            while (input[i] != '\0' && input[i] != '"')
            {
                /* Variable expansion inside double quotes */

                if (input[i] == '$')
                {
                    expandVariable(input, &i, buffer);
                    j = strlen(buffer);
                }
                else
                {
                    buffer[j++] = input[i++];
                    buffer[j] = '\0';
                }
            }

            /* Unmatched double quote */
            if (input[i] == '\0')
            {
                printf("\nSyntax Error: Unmatched double quote.\n");
                return 0;
            }

            buffer[j] = '\0';

            /* Skip closing quote */
            i++;

            addToken(buffer, "DOUBLE_QUOTED");

            continue;
        }

        /* ---------------- NORMAL TEXT ---------------- */

        while (input[i] != '\0' &&
               !isspace(input[i]) &&
               input[i] != '\'' &&
               input[i] != '"')
        {
            buffer[j++] = input[i++];
        }

        buffer[j] = '\0';

        if (j > 0)
        {
            addToken(buffer, "WORD");
        }
    }

    return 1;
}

/* ---------------- DISPLAY TOKENS ---------------- */

void displayTokens()
{
    printf("\n========== TOKEN STREAM ==========\n");

    for (int i = 0; i < tokenCount; i++)
    {
        printf("Token %d\n", i + 1);
        printf("  Value : %s\n", tokens[i].value);
        printf("  Type  : %s\n\n",
               tokens[i].type);
    }
}

/* ---------------- VALIDATE RESULTS ---------------- */

void validateResults()
{
    printf("========== VALIDATION ==========\n");

    if (tokenCount == 0)
    {
        printf("No tokens found.\n");
        return;
    }

    printf("Parsing successful.\n");
    printf("Total tokens: %d\n", tokenCount);

    for (int i = 0; i < tokenCount; i++)
    {
        if (strcmp(tokens[i].type, "SINGLE_QUOTED") == 0)
        {
            printf("Token %d: Single quote detected.\n",
                   i + 1);

            printf("        Variable expansion ignored.\n");
        }

        else if (strcmp(tokens[i].type, "DOUBLE_QUOTED") == 0)
        {
            printf("Token %d: Double quote detected.\n",
                   i + 1);

            printf("        Variable expansion allowed.\n");
        }
    }
}

/* ---------------- MAIN ---------------- */

int main()
{
    char input[MAX_LEN];

    printf("====================================\n");
    printf("   QUOTE PARSER\n");
    printf("====================================\n");

    printf("\nEnter a command: ");

    fgets(input, sizeof(input), stdin);

    /* Remove newline */
    input[strcspn(input, "\n")] = '\0';

    /* Empty input */
    if (strlen(input) == 0)
    {
        printf("\nEmpty command entered.\n");
        return 0;
    }

    /* Tokenize */
    if (!tokenize(input))
    {
        printf("\nParsing failed.\n");
        return 0;
    }

    /* Display tokens */
    displayTokens();

    /* Validate */
    validateResults();

    return 0;
}
