CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: daemon

daemon: daemon.o collect_reports.o backup_dashboard.o check_file_uploads.o lock_directories.o unlock_directories.o update_timer.o generate_reports.o
	$(CC) $(CFLAGS) -o daemon daemon.o collect_reports.o backup_dashboard.o check_file_uploads.o lock_directories.o unlock_directories.o update_timer.o generate_reports.o -lrt

daemon.o: daemon.c daemon_task.h
	$(CC) $(CFLAGS) -c daemon.c

collect_reports.o: collect_reports.c daemon_task.h
	$(CC) $(CFLAGS) -c collect_reports.c

backup_dashboard.o: backup_dashboard.c daemon_task.h
	$(CC) $(CFLAGS) -c backup_dashboard.c

check_file_uploads.o: check_file_uploads.c daemon_task.h
	$(CC) $(CFLAGS) -c check_file_uploads.c

lock_directories.o: lock_directories.c daemon_task.h
	$(CC) $(CFLAGS) -c lock_directories.c

unlock_directories.o: unlock_directories.c daemon_task.h
	$(CC) $(CFLAGS) -c unlock_directories.c

update_timer.o: update_timer.c daemon_task.h
	$(CC) $(CFLAGS) -c update_timer.c

generate_reports.o: generate_reports.c daemon_task.h
	$(CC) $(CFLAGS) -c generate_reports.c

sig_handler.o: sig_handler.c daemon_task.h
	$(CC) $(CFLAGS) -c sig_handler.c

clean:
	rm -f daemon daemon.o collect_reports.o backup_dashboard.o check_file_uploads.o lock_directories.o unlock_directories.o update_timer.o generate_reports.o sig_handler.o
