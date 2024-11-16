#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[2000];

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc == -1) {
		printf("Critical error: Unable to create socket!");
		return 1;
	}

	puts("Socket created successfully.\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(10000);

	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Critical error: Binding failed.\n");
		return 1;
	}

	puts("Binding completed successfully.\n");

	listen(socket_desc, 3);

	puts("Waiting for client connections...");

	c = sizeof(struct sockaddr_in);
	client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);

	if (client_sock < 0) {
		perror("Critical error: Accept failed.\n");
		return 1;
	}

	puts("Client connection accepted.\n");

	while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
		printf("Client sent %s\n", client_message);
		fflush(stdout);

		write(client_sock, client_message, strlen(client_message));

		memset(client_message, '\0', sizeof(client_message));
	}

	if (read_size == 0) {
		puts("Client disconnected.\n");
		fflush(stdout);
	}

	else if (read_size == -1) {
		perror("recv failed.\n");
	}

	return 0;
}
