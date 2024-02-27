// This code is resposinble for checking the changes in the XML files and then calls log_file_changes to the log the changes in a log file
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#define UPLOAD_DIR "/workspaces/system-software/Assignment/upload"
#define LOG_FILE "/workspaces/system-software/Assignment/reporting/logs/file_changes.log"

// Function to log file changes (name, modified/deleted and the users id)
void log_file_change(const char *file_name, const char *action, int user_id)
{
    // Open the log file in append mode
    FILE *fp = fopen(LOG_FILE, "a");
    // Check if the log file could not be opened and write an error message to the system log if so
    if (fp == NULL)
    {
        syslog(LOG_ERR, "Failed to open log file for writing");
        return;
    }

    // Get the current time
    time_t now;
    time(&now);
    char time_str[20];
    // Converted the current time to a string of the format "YYYY-MM-DD HH:MM:SS"
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Write the file change to the log file(logs the user the id, modified/deleted and the users ID in the reporting dir)
    fprintf(fp, " %s %s %s (user ID: %d)\n", time_str, action, file_name, user_id);
    // Close the log file
    fclose(fp);
}

// Function to check for new or modified xml files in upload directory
void check_file_uploads(void)
{
    DIR *dir;             // Directory pointer object
    struct dirent *entry; // Directory pointer entry object
    struct stat filestat; // File status object(store information about the file)

    // Open the upload directory(opendir)
    dir = opendir(UPLOAD_DIR);
    // Check if the upload directory could be opened and in not write message
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open directory");
        exit(EXIT_FAILURE);
    }

    // Loop through all the files in the upload directory(returns pointer to dirent object)
    while ((entry = readdir(dir)) != NULL)
    {
        char filepath[512];
        // Construct the full file path by concatenating the upload directory path and the name of the file
        sprintf(filepath, "%s/%s", UPLOAD_DIR, entry->d_name);

        // Get the file status
        if (stat(filepath, &filestat))
        {
            // If the file status could not be retrieved, skip to the next entry
            continue;
        }

        // logging changes to the syslog if any changes by a person has been made
        if (S_ISREG(filestat.st_mode))
        {
            // Log that the file has been modified and by the user to the sys log
            syslog(LOG_INFO, "File %s modified by user %d at %s", entry->d_name, filestat.st_uid, ctime(&filestat.st_mtime));
            // Log the file change(passing to the log function)
            log_file_change(entry->d_name, "modified", filestat.st_uid);
        }
    }

    // Close the upload directory
    closedir(dir);
}