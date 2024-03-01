#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include "daemon_task.h"
#include "constants.h"

void move_reports()
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
        // Construct the full path to the file
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", UPLOAD_DIR, expected_files[i]);

        // Check if the file exists using stat
        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1)
        {
            // Log missing file
            FILE *log_file = fopen(LOG_FILE, "a");
            if (log_file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(log_file, "[%s] action: MOVE, msg: %s report is missing\n", timestamp, expected_files[i]);
            fclose(log_file);
        }
        else
        {
            // Move the file to the dashboard directory
            char mv_command[1024];
            snprintf(mv_command, sizeof(mv_command), "mv %s/%s %s/", UPLOAD_DIR, expected_files[i], DASHBOARD_DIR);
            int mv_status = system(mv_command);

            // Log the move event or error
            FILE *log_file = fopen(LOG_FILE, "a");
            if (log_file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            if (mv_status == 0)
            {
                fprintf(log_file, "[%s] action: MOVE, msg: %s has been moved to dashboard\n", timestamp, expected_files[i]);
            }
            else
            {
                fprintf(log_file, "[%s] action: MOVE, msg: Error moving file %s to dashboard\n", timestamp, expected_files[i]);
            }
            fclose(log_file);
        }
    }
}
