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

// Signal handler for SIGTERM
void signal_handler(int sig)
{
    if (sig == SIGTERM)
    {
        syslog(LOG_INFO, "Received SIGINT signal, shutting down.");
        exit(EXIT_SUCCESS);
    }
}

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

    // Set signal handler for SIGTERM
    signal(SIGTERM, signal_handler);

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
    time(&now); /* get current time; same as: now = time(NULL)  */
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
    // check_file_uploads();

    while (1)
    {
        move_reports();
        backup();

        // break;
        sleep(1);
        break;
    }

    // Close log file
    close(fd);
    return EXIT_SUCCESS;
}
