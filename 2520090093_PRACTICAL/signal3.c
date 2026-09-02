#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int flag = 0;

void myfunc(int a)
{
    printf("\nENTERED HANDLER\n");
    printf("Signal received: %d\n", a);
    flag = 1;
}

int main()
{
    signal(SIGINT, myfunc);

    printf("Press Ctrl+C in 5 seconds\n");

    sleep(5);

    if (flag == 0)
        printf("You never pressed Ctrl+C\n");

    printf("END OF PROGRAM\n");

    return 0;
}
