#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

void change_permissions(const char* path, mode_t mode) {
    int result = chmod(path, mode);
    if (result != 0) {
        perror("Could not change permissions");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("Daemon PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    umask(0);

    if (setsid() < 0) {
        perror("Could not create new session");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("Could not change directory to /");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        time_t now = time(NULL);
        struct tm* time_info = localtime(&now);

        if (time_info->tm_hour == 9) {
            change_permissions("/path/to/your/directory", 0777);
        } else if (time_info->tm_hour == 17) {
            change_permissions("/path/to/your/directory", 0000);
        }

        sleep(3600);
    }

    return EXIT_SUCCESS;
}