#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "daemon_task.h"

#define UPLOAD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/upload"
#define LOG_FILE "/home/ronan/Desktop/shared_folder/ca1/assignment/log.txt"
#define DASHBOARD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/dashboard"
#define DASHBOARD_BACKUP_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/dashboard_backup"

void lock_directories()
{
    if (chmod(UPLOAD_DIR, 0555) == -1 || chmod(DASHBOARD_DIR, 0555) == -1 || chmod(DASHBOARD_BACKUP_DIR, 0555) == -1)
    {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}