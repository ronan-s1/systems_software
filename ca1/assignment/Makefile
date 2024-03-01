# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Source files
SRC_FILES = daemon.c move_reports.c check_file_uploads.c backup.c update_timer.c unlock_directories.c lock_directories.c sig_handler.c

# Object files
OBJ_FILES = $(SRC_FILES:%.c=%.o)

# Executable
EXECUTABLE = daemon

# Build executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
