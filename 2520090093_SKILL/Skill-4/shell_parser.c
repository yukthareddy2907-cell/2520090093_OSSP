#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_LEN 100

/* ---------------- TOKEN STRUCTURE ---------------- */

typedef struct {
    char value[MAX_LEN];
    char type[30];
} Token;

Token tokens[MAX_TOKENS];
int tokenCount = 0;

/* ---------------- LEXER ---------------- */

void addToken(char *value, char *type)
{
    if (tokenCount >= MAX_TOKENS)
        return;

    strcpy(tokens[tokenCount].value, value);
    strcpy(tokens[tokenCount].type, type);
    tokenCount++;
}

void tokenize(char *input)
{
    int i = 0;
    char buffer[MAX_LEN];

    while (input[i] != '\0')
    {
        /* Ignore whitespace */
        if (isspace(input[i]))
        {
            i++;
            continue;
        }

        /* Word / command */
        if (isalnum(input[i]) || input[i] == '_')
        {
            int j = 0;

            while (isalnum(input[i]) || input[i] == '_')
            {
                buffer[j++] = input[i++];
            }

            buffer[j] = '\0';

            addToken(buffer, "WORD");
        }

        /* Pipe delimiter */
        else if (input[i] == '|')
        {
            addToken("|", "PIPE");
            i++;
        }

        /* Input redirection */
        else if (input[i] == '<')
        {
            addToken("<", "INPUT_REDIRECT");
            i++;
        }

        /* Output redirection */
        else if (input[i] == '>')
        {
            addToken(">", "OUTPUT_REDIRECT");
            i++;
        }

        /* Semicolon */
        else if (input[i] == ';')
        {
            addToken(";", "SEMICOLON");
            i++;
        }

        /* Unknown character */
        else
        {
            buffer[0] = input[i];
            buffer[1] = '\0';

            addToken(buffer, "UNKNOWN");
            i++;
        }
    }
}

/* ---------------- DISPLAY TOKENS ---------------- */

void displayTokens()
{
    int i;

    printf("\n----- TOKEN STREAM -----\n");

    for (i = 0; i < tokenCount; i++)
    {
        printf("Token %d: %-15s Type: %s\n",
               i + 1,
               tokens[i].value,
               tokens[i].type);
    }
}

/* ---------------- PARSE TREE ---------------- */

typedef struct Node {
    char value[MAX_LEN];
    char type[30];

    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(char *value, char *type)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    strcpy(newNode->value, value);
    strcpy(newNode->type, type);

    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* ---------------- PARSER ---------------- */

int current = 0;

Node *parseCommand()
{
    Node *root = NULL;
    Node *currentNode = NULL;

    /* Empty command */
    if (current >= tokenCount)
    {
        return NULL;
    }

    /* First token must be WORD */
    if (strcmp(tokens[current].type, "WORD") != 0)
    {
        printf("\nSyntax Error: Command must start with a WORD.\n");
        return NULL;
    }

    root = createNode("COMMAND", "NON_TERMINAL");

    currentNode = createNode(
        tokens[current].value,
        tokens[current].type
    );

    root->left = currentNode;

    current++;

    /* Process remaining command arguments */
    while (current < tokenCount)
    {
        if (strcmp(tokens[current].type, "WORD") == 0)
        {
            Node *arg = createNode(
                tokens[current].value,
                tokens[current].type
            );

            currentNode->right = arg;
            currentNode = arg;

            current++;
        }

        else if (strcmp(tokens[current].type, "PIPE") == 0)
        {
            printf("\nPIPE detected.\n");
            current++;
            break;
        }

        else if (strcmp(tokens[current].type,
                        "INPUT_REDIRECT") == 0 ||
                 strcmp(tokens[current].type,
                        "OUTPUT_REDIRECT") == 0)
        {
            Node *redirect;

            redirect = createNode(
                tokens[current].value,
                tokens[current].type
            );

            currentNode->right = redirect;
            currentNode = redirect;

            current++;

            /* Redirection must be followed by WORD */
            if (current >= tokenCount ||
                strcmp(tokens[current].type, "WORD") != 0)
            {
                printf("\nSyntax Error: "
                       "Redirection must be followed by a filename.\n");

                return NULL;
            }

            Node *file = createNode(
                tokens[current].value,
                tokens[current].type
            );

            currentNode->right = file;
            currentNode = file;

            current++;
        }

        else
        {
            printf("\nSyntax Error: Unexpected token '%s'\n",
                   tokens[current].value);

            return NULL;
        }
    }

    return root;
}

/* ---------------- PRINT PARSE TREE ---------------- */

void printTree(Node *root, int level)
{
    int i;

    if (root == NULL)
        return;

    for (i = 0; i < level; i++)
        printf("  ");

    printf("|-- %s (%s)\n",
           root->value,
           root->type);

    printTree(root->left, level + 1);
    printTree(root->right, level + 1);
}

/* ---------------- EXECUTION STRUCTURE ---------------- */

void executionStructure()
{
    int i;

    printf("\n----- EXECUTION STRUCTURE -----\n");

    for (i = 0; i < tokenCount; i++)
    {
        if (strcmp(tokens[i].type, "WORD") == 0)
        {
            printf("COMMAND/ARGUMENT : %s\n",
                   tokens[i].value);
        }
        else if (strcmp(tokens[i].type, "PIPE") == 0)
        {
            printf("PIPE             : |\n");
        }
        else if (strcmp(tokens[i].type, "INPUT_REDIRECT") == 0)
        {
            printf("INPUT REDIRECT   : <\n");
        }
        else if (strcmp(tokens[i].type, "OUTPUT_REDIRECT") == 0)
        {
            printf("OUTPUT REDIRECT  : >\n");
        }
    }
}

/* ---------------- MAIN ---------------- */

int main()
{
    char input[500];

    printf("Enter command: ");

    fgets(input, sizeof(input), stdin);

    /* Remove newline */
    input[strcspn(input, "\n")] = '\0';

    /* Empty command */
    if (strlen(input) == 0)
    {
        printf("\nEmpty command entered.\n");
        return 0;
    }

    /* Lexical Analysis */
    tokenize(input);

    displayTokens();

    /* Check unknown tokens */
    for (int i = 0; i < tokenCount; i++)
    {
        if (strcmp(tokens[i].type, "UNKNOWN") == 0)
        {
            printf("\nLexical Error: Unknown token '%s'\n",
                   tokens[i].value);

            return 0;
        }
    }

    /* Parsing */
    current = 0;

    Node *tree = parseCommand();

    if (tree == NULL)
    {
        printf("\nParsing failed.\n");
        return 0;
    }

    if (current < tokenCount)
    {
        printf("\nSyntax Error: "
               "Extra tokens after command.\n");

        return 0;
    }

    printf("\n----- PARSE TREE -----\n");

    printTree(tree, 0);

    /* Execution structure */
    executionStructure();

    printf("\n\nParsing completed successfully.\n");

    return 0;
}
