// This file is responisble for collecting the XML reports in the upload and transferring them the the reporting directory
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdlib.h>
#include "daemon_task.h"

void collect_reports(void) 
{
    // create a child process to handle the file copying
    pid_t pid = fork();

    if (pid == -1) 
    {
        // error log creating child process
        syslog(LOG_ERR, "Error creating child process to collect reports");
        return;
    } 
    else if (pid == 0) 
    {
        // child process
        // use 'cp' command to copy all XML files from upload directory to reporting directory
        execl("/usr/bin/find", "find", "/workspaces/system-software/Assignment/upload/", "-name", "*.xml", "-exec", "/bin/cp", "{}", "/workspaces/system-software/Assignment/reporting/", ";", NULL);



        // if execl returns, an error has occurred
        syslog(LOG_ERR, "Error copying reports from upload to reporting directory");
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
                syslog(LOG_INFO, "Reports collected and moved to reporting directory");
            } 
            else 
            {
                syslog(LOG_ERR, "Error collecting reports from upload to reporting directory");
            }
        } 
        else 
        {
            syslog(LOG_ERR, "Child process to collect reports exited wrong");
        }
    }
}

