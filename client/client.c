#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in server;
	char *message, *server_reply;

	message = malloc(2000);
	server_reply = malloc(2000);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1) {
		perror("Critical error");
		return 1;
	}

	puts("Socket created successfully.");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);

	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Critical error");
		return 1;
	}

	puts("Connection established successfully.");

	while(1) {
		memset(message, '\0', 2000);

		printf("\nEnter a message: ");
		fgets(message, 2000, stdin);

		if ((strlen(message) > 0) && (message[strlen(message) - 1] == '\n')) {
			message[strlen(message) - 1] = '\0';
		}

		if (send(sock, message, strlen(message), 0) < 0) {
			perror("Critical error");
			return 1;
		}

		if (recv(sock, server_reply, 2000, 0) < 0) {
			perror("Error");
			break;
		}

		printf("Server response: %s\n", server_reply);

		memset(server_reply, '\0', 2000);
	}
	
	free(message);
	free(server_reply);

	close(sock);
	return 0;
}
