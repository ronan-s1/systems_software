#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

// Structure to hold file information
typedef struct {
    char name[256];
    time_t modified_time;
} FileInfo;

// Function to compare two FileInfo structures based on modification time
int compare_files(const void *a, const void *b) {
    const FileInfo *fileA = (const FileInfo *)a;
    const FileInfo *fileB = (const FileInfo *)b;
    return difftime(fileA->modified_time, fileB->modified_time);
}

// Function to scan directory and return an array of file information
FileInfo* scan_directory(const char *dir_path, int *num_files) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    FileInfo *files = NULL;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            FileInfo file;
            strncpy(file.name, entry->d_name, sizeof(file.name) - 1);
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            struct stat st;
            if (stat(path, &st) == 0)
                file.modified_time = st.st_mtime;
            else
                file.modified_time = 0;
            
            files = (FileInfo *)realloc(files, (count + 1) * sizeof(FileInfo));
            files[count++] = file;
        }
    }

    closedir(dir);

    *num_files = count;
    return files;
}

int main() {
    const char *dir_path = "/home/ronan/Desktop/shared_folder/ca1/assignment/upload";
    FileInfo *prev_files = NULL;
    int prev_num_files = 0;

    while (1) {
        int num_files;
        FileInfo *current_files = scan_directory(dir_path, &num_files);

        // Compare current files with previous files
        if (prev_files != NULL && prev_num_files != 0) {
            // Sort files based on modification time for comparison
            qsort(prev_files, prev_num_files, sizeof(FileInfo), compare_files);
            qsort(current_files, num_files, sizeof(FileInfo), compare_files);

            // Find changes
            int i = 0, j = 0;
            while (i < prev_num_files && j < num_files) {
                if (strcmp(prev_files[i].name, current_files[j].name) == 0) {
                    if (prev_files[i].modified_time != current_files[j].modified_time) {
                        printf("File %s modified\n", current_files[j].name);
                    }
                    i++;
                    j++;
                } else if (strcmp(prev_files[i].name, current_files[j].name) < 0) {
                    printf("File %s deleted\n", prev_files[i].name);
                    i++;
                } else {
                    printf("File %s created\n", current_files[j].name);
                    j++;
                }
            }
            // Handle remaining files
            for (; i < prev_num_files; i++) {
                printf("File %s deleted\n", prev_files[i].name);
            }
            for (; j < num_files; j++) {
                printf("File %s created\n", current_files[j].name);
            }
        }

        // Free memory and update previous files
        free(prev_files);
        prev_files = current_files;
        prev_num_files = num_files;

        sleep(1); // Adjust sleep duration as needed
    }

    return 0;
}























// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <string.h>
// #include <time.h>
// #include <string.h>
// #include <dirent.h>
// #include <time.h>


// #define UPLOAD_DIR "/home/ronan/Desktop/shared_folder/ca1/assignment/upload"
// #define LOG_FILE "/home/ronan/Desktop/shared_folder/ca1/assignment/log.txt"

// // Structure to hold file information
// typedef struct
// {
//     char name[256];
//     time_t modified_time;
// } FileInfo;

// // Function to compare two FileInfo structures based on modification time
// int compare_files(const void *a, const void *b)
// {
//     const FileInfo *fileA = (const FileInfo *)a;
//     const FileInfo *fileB = (const FileInfo *)b;
//     return difftime(fileA->modified_time, fileB->modified_time);
// }

// // Function to scan directory and return an array of file information
// FileInfo *scan_directory(const char *dir_path, int *num_files)
// {
//     DIR *dir = opendir(dir_path);
//     if (!dir)
//     {
//         perror("opendir");
//         exit(EXIT_FAILURE);
//     }

//     struct dirent *entry;
//     FileInfo *files = NULL;
//     int count = 0;

//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (entry->d_type == DT_REG)
//         { // Regular file
//             FileInfo file;
//             strncpy(file.name, entry->d_name, sizeof(file.name) - 1);
//             char path[512];
//             snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
//             struct stat st;
//             if (stat(path, &st) == 0)
//             {
//                 file.modified_time = st.st_mtime;
//             }
//             else
//             {
//                 file.modified_time = 0;
//             }
//             files = (FileInfo *)realloc(files, (count + 1) * sizeof(FileInfo));
//             files[count++] = file;
//         }
//     }

//     closedir(dir);

//     *num_files = count;
//     return files;
// }

// int check_file_uploads()
// {
//     FileInfo *prev_files = NULL;
//     int prev_num_files = 0;
//     int num_files;
//     FileInfo *current_files = scan_directory(UPLOAD_DIR, &num_files);

//     // Compare current files with previous files
//     if (prev_files != NULL && prev_num_files != 0)
//     {
//         // Sort files based on modification time for comparison
//         qsort(prev_files, prev_num_files, sizeof(FileInfo), compare_files);
//         qsort(current_files, num_files, sizeof(FileInfo), compare_files);

//         // Find changes
//         int i = 0, j = 0;
//         while (i < prev_num_files && j < num_files)
//         {
//             if (strcmp(prev_files[i].name, current_files[j].name) == 0)
//             {
//                 if (prev_files[i].modified_time != current_files[j].modified_time)
//                 {
//                     printf("File %s modified\n", current_files[j].name);

//                     time_t t = time(NULL);
//                     struct tm *tm_info = localtime(&t);
//                     char timestamp[20];
//                     strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

//                     FILE *log_file = fopen(LOG_FILE, "a");
//                     if (log_file == NULL)
//                     {
//                         perror("fopen");
//                         exit(EXIT_FAILURE);
//                     }
//                     fprintf(log_file, "[%s] msg: %s has been modified\n", timestamp, current_files[j].name);
//                     fclose(log_file);
//                 }
//                 i++;
//                 j++;
//             }
//             else if (strcmp(prev_files[i].name, current_files[j].name) < 0)
//             {
//                 printf("File %s deleted\n", prev_files[i].name);

//                 time_t t = time(NULL);
//                 struct tm *tm_info = localtime(&t);
//                 char timestamp[20];
//                 strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

//                 FILE *log_file = fopen(LOG_FILE, "a");
//                 if (log_file == NULL)
//                 {
//                     perror("fopen");
//                     exit(EXIT_FAILURE);
//                 }
//                 fprintf(log_file, "[%s] msg: %s has been deleted\n", timestamp, prev_files[i].name);
//                 fclose(log_file);

//                 i++;
//             }
//             else
//             {
//                 printf("File %s created\n", current_files[j].name);

//                 time_t t = time(NULL);
//                 struct tm *tm_info = localtime(&t);
//                 char timestamp[20];
//                 strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

//                 FILE *log_file = fopen(LOG_FILE, "a");
//                 if (log_file == NULL)
//                 {
//                     perror("fopen");
//                     exit(EXIT_FAILURE);
//                 }
//                 fprintf(log_file, "[%s] msg: %s has been created\n", timestamp, current_files[j].name);
//                 fclose(log_file);

//                 j++;
//             }
//         }
//         // Handle remaining files
//         for (; i < prev_num_files; i++)
//         {
//             printf("File %s deleted\n", prev_files[i].name);

//             time_t t = time(NULL);
//             struct tm *tm_info = localtime(&t);
//             char timestamp[20];
//             strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

//             FILE *log_file = fopen(LOG_FILE, "a");
//             if (log_file == NULL)
//             {
//                 perror("fopen");
//                 exit(EXIT_FAILURE);
//             }
//             fprintf(log_file, "[%s] msg: %s has been deleted\n", timestamp, prev_files[i].name);
//             fclose(log_file);
//         }
//         for (; j < num_files; j++)
//         {
//             printf("File %s created\n", current_files[j].name);

//             time_t t = time(NULL);
//             struct tm *tm_info = localtime(&t);
//             char timestamp[20];
//             strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

//             FILE *log_file = fopen(LOG_FILE, "a");
//             if (log_file == NULL)
//             {
//                 perror("fopen");
//                 exit(EXIT_FAILURE);
//             }
//             fprintf(log_file, "[%s] msg: %s has been created\n", timestamp, current_files[j].name);
//             fclose(log_file);
//         }
//     }

//     // Free memory and update previous files
//     free(prev_files);
//     prev_files = current_files;
//     prev_num_files = num_files;

//     sleep(1);
// }

// int main()
// {
//     while (1)
//     {
//         check_file_uploads();
//     }
//     return 0;
// }
