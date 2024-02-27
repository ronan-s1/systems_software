
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>

void unlock_directories()
{
    printf("unlock directory functionality should go here. fork/chmod will be used here to change permissions");

    char mode[4] = "0777";
    int i = strtol(mode, 0, 8);

    if (chmod(maindir, i) == 0)
    {
        openlog("Daemon unlock", LOG_PID | LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "dir unlock success");
        closelog();
    }
    else
    {
        openlog("Daemon unlock", LOG_PID | LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "dir unlock error");
        closelog();
    }
}