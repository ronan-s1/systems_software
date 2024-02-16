#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() 
{
    time_t now;
    struct tm newyear;
    struct tm countdown;
    double seconds;

    time(&now);
    newyear = *localtime(&now);

    newyear.tm_hour = 20;
    newyear.tm_min = 45;
    newyear.tm_sec = 0;

    countdown = newyear;

    seconds = difftime(now, mktime(&countdown));

    if (seconds < 0) 
    {
        seconds = -seconds; 

        while (seconds > 0) 
        {
            time(&now);
            seconds = difftime(now, mktime(&newyear));
            printf("%f seconds left\n", seconds);
            sleep(1);
        }
    }

    printf("time up\n");

    return 0;
}