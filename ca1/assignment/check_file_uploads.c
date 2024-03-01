#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>
#include "daemon_task.h"

#define UPLOAD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/upload"
#define LOG_FILE "/home/ronan/Desktop/shared_folder/ca1/assignment/log.txt"

void check_file_uploads()
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

        // Check if the file exists
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
            fprintf(log_file, "[%s] action: CHECK, msg: %s report is missing\n", timestamp, expected_files[i]);
            fclose(log_file);
        }
        else
        {
            // Get user name corresponding to file owner
            struct passwd *user_info = getpwuid(file_stat.st_uid);
            const char *file_owner = (user_info != NULL) ? user_info->pw_name : "Unknown";

            // Get last modification time
            struct tm *mod_time_info = localtime(&(file_stat.st_mtime));
            char mod_time_str[20];
            strftime(mod_time_str, sizeof(mod_time_str), "%Y-%m-%d %H:%M:%S", mod_time_info);

            // Log file information
            FILE *log_file = fopen(LOG_FILE, "a");
            if (log_file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fprintf(log_file, "[%s] action: CHECK, msg: %s report is present, modified_by: %s, last_modified_time: %s\n", timestamp, expected_files[i], file_owner, mod_time_str);
            fclose(log_file);
        }
    }
}
