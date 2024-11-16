CC = gcc
CFLAGS = -Iinclude

all: client server

client: src/client/client.c src/ciph.c
	$(CC) $(CFLAGS) -o client src/client/client.c src/ciph.c

server: src/server/server.c src/ciph.c
	$(CC) $(CFLAGS) -o server src/server/server.c src/ciph.c

clean:
	rm -f client server
