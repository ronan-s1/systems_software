#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include "daemon_task.h"
#include "constants.h"

void unlock_directories()
{
    if (chmod(UPLOAD_DIR, 0777) == -1 || chmod(DASHBOARD_DIR, 0777) == -1 || chmod(DASHBOARD_BACKUP_DIR, 0777) == -1)
    {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}