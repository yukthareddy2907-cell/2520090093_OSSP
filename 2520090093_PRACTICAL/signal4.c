#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alarmhandler(int a)
{
    printf("ALARM: WAKE UP\n");
}

int main()
{
    signal(SIGALRM, alarmhandler);

    int duration;

    printf("Enter the duration of alarm: ");
    scanf("%d", &duration);

    alarm(duration);

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep(1);
    }

    printf("END OF PROGRAM\n");

    return 0;
}
