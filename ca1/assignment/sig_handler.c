#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "daemon_task.h"
#include "constants.h"

void sig_handler(int sigNum)
{
    if (sigNum == SIGINT)
    {
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
        fprintf(log_file, "[%s] action: TRANSFER_TRIGGER, msg: initiating MOVE and BACKUP\n", timestamp);
        fclose(log_file);
        
        check_file_uploads();
        lock_directories();
        move_reports();	  
        backup();
        sleep(30);
        unlock_directories();
    }
}