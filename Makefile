CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = server client

all: $(TARGETS)

server: server.c
		$(CC) $(CFLAGS) -o bin/server server.c

client: client.c
		$(CC) $(CFLAGS) -o bin/client client.c

clean:
		rm -f $(TARGETS)

.PHONY: all clean
