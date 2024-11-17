#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include "ciph.h"

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
		int option;

		printf("\nChoose an option:\n");
		printf("1. Authenticate\n");
		printf("2. Encrypt string\n");
		printf("3. Log out\n");
		printf("4. Exit\n");
		printf("Option: ");
		scanf("%d", &option);
		getchar();

		memset(message, '\0', 2000);

		switch (option) {
			case 1: {
				char id[50], password[20];
				printf("Insert an id: ");
				fgets(id, sizeof(id), stdin);
				id[strcspn(id, "\n")] = '\0';

				printf("Insert a password: ");
				fgets(password, sizeof(password), stdin);
				password[strcspn(password, "\n")] = '\0';

				create_auth_request(message, id, password);
				break;
			}
			case 2: {
				char text[256];
				int shift;
				printf("Insert a message to encrypt: ");
				fgets(text, sizeof(text), stdin);
				text[strcspn(text, "\n")] = '\0';

				printf("Insert the desired shift: ");
				scanf("%d", &shift);
				getchar();

				create_encrypt_request(message, text, shift);

				break;
			}
			case 3: 
				create_logout_request(message);
				break;

			case 4:
				free(message);
				free(server_reply);
				close(sock);
				return 0;
			default:
				printf("Invalid option.\n");
				continue;
		}

		// success sending
		if (send(sock, message, strlen(message), 0) < 0) {
			perror("Critical error");
			return 1;
		}

		// receive
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
