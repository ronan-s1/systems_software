#ifndef DAEMON_TASKS 
#define DAEMON_TASKS

#include <time.h>

void move_reports(void);

void backup_dashboard(void);

void generate_reports(void);

void check_file_uploads(void);

void sig_handler(int);

void update_timer(struct tm*);
#endif
