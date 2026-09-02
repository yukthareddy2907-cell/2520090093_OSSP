#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myhandler(int signo)
{
    printf("\nSignal Received\n");
    printf("Signal Number = %d\n", signo);

    if (signo == SIGFPE)
        printf("Floating point exception\n");

    exit(1);
}

int main()
{
    signal(SIGFPE, myhandler);

    int x = 0, y = 0, res;

    printf("Enter the numerator: ");
    scanf("%d", &x);

    printf("Enter the denominator: ");
    scanf("%d", &y);

    printf("WE ARE PERFORMING DIVISION\n");

    res = x / y;

    printf("\nResult = %d\n", res);

    return 0;
}
