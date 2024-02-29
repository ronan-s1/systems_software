#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define UPLOAD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/upload"
#define DASHBOARD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/dashboard"
#define LOG_FILE "/home/ronan/Desktop/shared_folder/ca1/assignment/log.txt"

void move_reports()
{
    // Get the username of the current user
    char *username = getlogin();

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
        snprintf(ls_command, sizeof(ls_command), "ls %s | grep -q %s", UPLOAD_DIR, expected_files[i]);
        int ls_status = system(ls_command);

        // If file exists, move it to the dashboard directory
        if (ls_status == 0)
        {
            char mv_command[1024];
            snprintf(mv_command, sizeof(mv_command), "mv %s/%s %s/", UPLOAD_DIR, expected_files[i], DASHBOARD_DIR);
            int mv_status = system(mv_command);

            // Log move operation
            if (mv_status == 0)
            {
                FILE *log_file = fopen(LOG_FILE, "a");
                if (log_file == NULL)
                {
                    perror("fopen");
                    exit(EXIT_FAILURE);
                }
                fprintf(log_file, "[%s] user: %s, msg: %s has been moved to dashboard\n", timestamp, username, expected_files[i]);
                fclose(log_file);
            }
            else
            {
                fprintf(stderr, "Error moving file %s to dashboard\n", expected_files[i]);
                exit(EXIT_FAILURE);
            }
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
            fprintf(log_file, "[%s] user: %s, msg: %s report is missing\n", timestamp, username, expected_files[i]);
            fclose(log_file);
        }
    }
}
