CC = gcc
CFLAGS = -Iinclude
BINDIR = exec

all: $(BINDIR)/client $(BINDIR)/server

$(BINDIR)/client: src/client/client.c src/ciph.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/client src/client/client.c src/ciph.c

$(BINDIR)/server: src/server/server.c src/ciph.c src/server/caesar.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/server src/server/server.c src/ciph.c src/server/caesar.c

clean:
	rm -f $(BINDIR)/client $(BINDIR)/server

