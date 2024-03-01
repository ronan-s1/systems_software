#ifndef DAEMON_TASKS 
#define DAEMON_TASKS

#include <time.h>

void move_reports(void);

void backup(void);

void check_file_uploads(void);

void sig_handler(int);

void update_timer(struct tm*);

void lock_directories(void);

void unlock_directories(void);
#endif
