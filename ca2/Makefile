CC = gcc
CFLAGS = -Wall -Wextra -pthread -Wformat-truncation

all: server client

server: server.o
	$(CC) $(CFLAGS) server.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f server client *.o
