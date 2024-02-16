#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    time_t now;
    struct tm newyear;
    double seconds;

    // Get current time
    now = time(NULL);

    // Set the target time (e.g., 20:45:00)
    newyear = *localtime(&now);
    newyear.tm_hour = 20;
    newyear.tm_min = 45;
    newyear.tm_sec = 0;

    // Calculate the difference in seconds between current time and target time
    seconds = difftime(mktime(&newyear), now);

    // Countdown loop
    while (seconds > 0) {
        printf("Time remaining: %.0f seconds\n", seconds);
        sleep(1); // Sleep for 1 second
        now = time(NULL);
        seconds = difftime(mktime(&newyear), now);
    }

    // Alarm reached
    printf("Time's up! It's %02d:%02d:%02d\n", newyear.tm_hour, newyear.tm_min, newyear.tm_sec);

    return 0;
}
