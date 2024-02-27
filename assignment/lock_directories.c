#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>

// Function to lock directories
void lock_directories()
{
    printf("lock directory functionality should go here. fork/chmod will be used here to change permissions");

    // Define mode for directory permissions (0555 means read and execute for all)
    char mode[4] = "0555";
    int i = strtol(mode, 0, 8); // Convert mode string to integer

    // Change directory permissions using chmod
    if (chmod(maindir, i) == 0) 
    {
        // Open syslog for logging
        openlog("Daemon upload lock", LOG_PID | LOG_CONS, LOG_USER);
        // Log a message indicating directory is now locked
        syslog(LOG_INFO, "Directory now locked");
        // Close syslog
        closelog();
    }
    else
    {
        // Open syslog for logging
        openlog("lock err", LOG_PID | LOG_CONS, LOG_USER);
        // Log an error message if chmod fails
        syslog(LOG_INFO, "lock err");
        // Close syslog
        closelog();
    }
}
