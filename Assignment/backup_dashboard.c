#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "daemon_task.h"

void backup_dashboard(void)
{

    // create a child process to handle the backup
    pid_t pid = fork();

    if (pid == -1)
    {
        // error creating child process
        syslog(LOG_ERR, "Error creating child process to backup dashboard");
        return;
    }
    else if (pid == 0)
    {
        // child process
        // use 'rsync' command to copy all files from reporting directory to dashboard directory
        execl("/usr/bin/rsync", "rsync", "-a", "--delete", "--no-perms", "/workspaces/system-software/Assignment/reporting/", "/workspaces/system-software/Assignment/backup_dashboard/", NULL);

        // if execl returns, an error has occurred
        syslog(LOG_ERR, "Error backing up dashboard");
        exit(EXIT_FAILURE);
    }
    else
    {
        // parent process
        // wait for child process to complete
        int status;
        waitpid(pid, &status, 0);

        // check child process exit status
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status == EXIT_SUCCESS)
            {
                syslog(LOG_INFO, "Dashboard backed up successfully");
            }
            else
            {
                syslog(LOG_ERR, "Error backing up dashboard");
            }
        }
        else
        {
            syslog(LOG_ERR, "Child process to backup dashboard exited wrong");
        }
    }
}
