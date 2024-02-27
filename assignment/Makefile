CC=gcc
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-lm

SRC=daemon.c check_file_uploads.c vars.c
OBJ=$(SRC:.c=.o)
EXEC=daemon

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

daemon.o: daemon.c vars.h
	$(CC) $(CFLAGS) -c $<

check_upload_files.o: check_file_uploads.c  vars.h
	$(CC) $(CFLAGS) -c $<

vars.o: vars.c vars.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)