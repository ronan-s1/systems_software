#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "daemon_task.h"
#include "constants.h"

void update_timer(struct tm *due_date)
{
    due_date->tm_mday += 1;
    mktime(due_date);

    // logging
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(log_file, "[%s] action: TIME_UPDATE, msg: timer updated, due next day, %d\n", timestamp, due_date->tm_mday);
    fclose(log_file);
}