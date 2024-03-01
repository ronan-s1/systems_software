#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <syslog.h>
#include "daemon_task.h"

#define LOG_FILE "/home/ronan/Desktop/shared_folder/ca1/assignment/log.txt"

int main()
{
    time_t now;
    struct tm backup_time;
    time(&now);
    backup_time = *localtime(&now);
    backup_time.tm_hour = 13;
    backup_time.tm_min = 41;
    backup_time.tm_sec = 0;

    // Fork the parent process
    int pid = fork();

    // Error handling for fork
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // If parent process, terminate
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Elevate the orphan process to session leader, to loose controlling TTY
    // This command runs the process in a new session
    if (setsid() < 0)
    {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Fork again
    int pid_2 = fork();

    // Error handling for fork
    if (pid_2 < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // If parent process, terminate
    if (pid_2 > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Change the current working dir to root.
    // This will eliminate any issues of running on a mounted drive
    if (chdir("/") < 0)
    {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // call umask() to set the file mode creation mask to 0
    umask(0);

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    // Open log file in append mode
    int fd = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    struct tm check_uploads_time;
    time(&now);
    check_uploads_time = *localtime(&now);
    check_uploads_time.tm_hour = 13;
    check_uploads_time.tm_min = 40;
    check_uploads_time.tm_sec = 0;

    // Get current time
    time_t rawtime;
    struct tm *info;
    char timestamp[20];

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", info);

    // Log message to file
    dprintf(fd, "[%s] Daemon is running...\n", timestamp);
    fsync(fd);

    // Main daemon loop
    while (1)
    {
        // Set signal handler for SIGINT, "kill -SIGINT pid" will trigger this
        if (signal(SIGINT, sig_handler) == SIG_ERR)
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
            fprintf(log_file, "[%s] action: TRANSFER_TRIGGER, msg: daemon.c SIG_ERR recieved\n", timestamp);
            fclose(log_file);
        }

        // countdown to 23:30
        time(&now);
        double seconds_to_files_check = difftime(now, mktime(&check_uploads_time));
        if (seconds_to_files_check == 0)
        {
            check_file_uploads();

            // change to tommorow's day
            update_timer(&check_uploads_time);
        }

        time(&now);
        double seconds_to_transfer = difftime(now, mktime(&backup_time));
        if (seconds_to_transfer == 0)
        {
            lock_directories();
			move_reports();	  
			backup();
			sleep(30);
			unlock_directories();
    
			// After tasks are finished, start counting to next day
			update_timer(&backup_time);
        }
        sleep(1);
    }

    // Close log file
    close(fd);
    return EXIT_SUCCESS;
}
