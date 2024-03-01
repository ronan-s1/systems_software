#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include "daemon_task.h"
#include "constants.h"

void backup()
{
    // Get the current timestamp
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Expected file names
    const char *expected_files[] = {"warehouse.xml", "manufacturing.xml", "sales.xml", "distribution.xml"};
    int num_expected_files = sizeof(expected_files) / sizeof(expected_files[0]);

    // Iterate over expected files
    for (int i = 0; i < num_expected_files; i++)
    {
        // Check if the file exists in the upload directory
        char ls_command[1024];
        snprintf(ls_command, sizeof(ls_command), "ls %s | grep -qw %s", DASHBOARD_DIR, expected_files[i]);
        int ls_status = system(ls_command);

        // If file exists, move it to the dashboard directory
        if (ls_status == 0)
        {
            char cp_command[1024];
            snprintf(cp_command, sizeof(cp_command), "cp %s/%s %s/", DASHBOARD_DIR, expected_files[i], DASHBOARD_BACKUP_DIR);
            int cp_status = system(cp_command);

            // If move is successful, log the event else log the error
            FILE *log_file = fopen(LOG_FILE, "a");
            if (cp_status == 0)
            {
                if (log_file == NULL)
                {
                    perror("fopen");
                    exit(EXIT_FAILURE);
                }
                fprintf(log_file, "[%s] action: BACKUP, msg: %s has been copied to dashboard_backup\n", timestamp, expected_files[i]);
            }
            else
            {
                fprintf(log_file, "[%s] action: BACKUP, msg: Error copying file %s to dashboard_backup\n", timestamp, expected_files[i]);
                exit(EXIT_FAILURE);
            }
            fclose(log_file);
        }
        else
        {
            // Log missing file
            FILE *log_file = fopen(LOG_FILE, "a");
            if (log_file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(log_file, "[%s] action: BACKUP, msg: %s report is missing\n", timestamp, expected_files[i]);
            fclose(log_file);
        }
    }
}
