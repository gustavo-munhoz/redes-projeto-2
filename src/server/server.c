#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "ciph.h"
#include "caesar.h"

typedef struct {
	char id[50];
	char password[50];
} User;

User valid_users[] = {
	{"user1", "password1"},
	{"user2", "password2"},
	{"user3", "password3"}
};

int num_valid_users = sizeof(valid_users) / sizeof(User);

int authenticate_user(const char *id, const char *password);
void process_client_message(int client_sock, char *message, int *is_authenticated);
void handle_authentication(int client_sock, char *id, char *password, int *is_authenticated);
void handle_encryption(int client_sock, char *text, int shift);
void handle_logout(int client_sock, int *is_authenticated);

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

	int is_authenticated = 0;

	while(1) {
		memset(client_message, '\0', sizeof(client_message));
		
		int read_size = recv(client_sock, client_message, sizeof(client_message) - 1, 0);
		if (read_size <= 0) {
			if (read_size == 0) puts("Client disconnected.");
			else perror("Error receiving data");
			break;
		}

		printf("Message received: %s\n", client_message);

		fflush(stdout);
		
		process_client_message(client_sock, client_message, &is_authenticated);
	}

	close(client_sock);
	close(socket_desc);

	return 0;
}

int authenticate_user(const char *id, const char *password) {
    for (int i = 0; i < num_valid_users; i++) {
        if (strcmp(id, valid_users[i].id) == 0 && strcmp(password, valid_users[i].password) == 0) {
            return 1;
        }
    }
    return 0;
}

void process_client_message(int client_sock, char *message, int *is_authenticated) {
    char *message_copy = strdup(message);
    char *message_type = strtok(message_copy, " ");

    if (strcmp(message_type, AUTH_REQUEST) == 0) {
        char *id = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        handle_authentication(client_sock, id, password, is_authenticated);
        
    } else if (strcmp(message_type, ENCRYPT_REQUEST) == 0) {
        if (*is_authenticated) {
            char *text = strtok(NULL, " ");
            char *shift_str = strtok(NULL, " ");
            if (text != NULL && shift_str != NULL) {
                int shift = atoi(shift_str);
                handle_encryption(client_sock, text, shift);

            } else {
                char response[2000];
                strcpy(response, ENCRYPT_ERROR);
                write(client_sock, response, strlen(response));
            }

        } else {
            char response[2000];
            strcpy(response, NOT_AUTHENTICATED);
            write(client_sock, response, strlen(response));
        }

    } else if (strcmp(message_type, LOGOUT_REQUEST) == 0) {
        handle_logout(client_sock, is_authenticated);

    } else {
        char response[2000];
        strcpy(response, UNKNOWN_COMMAND);
        write(client_sock, response, strlen(response));
    }

    free(message_copy);
}

void handle_authentication(int client_sock, char *id, char *password, int *is_authenticated) {
    char response[2000];
    if (id != NULL && password != NULL) {
        if (authenticate_user(id, password)) {
            strcpy(response, AUTH_OK);
            *is_authenticated = 1;
        } else {
            strcpy(response, AUTH_ERROR);
        }
    } else {
        strcpy(response, AUTH_ERROR);
    }
    write(client_sock, response, strlen(response));
}

void handle_encryption(int client_sock, char *text, int shift) {
    char encrypted_text[2000];
    caesar_cipher(text, shift, encrypted_text);

    char response[2000];
    sprintf(response, "%s %s", ENCRYPTED_TEXT, encrypted_text);
    write(client_sock, response, strlen(response));
}

void handle_logout(int client_sock, int *is_authenticated) {
    *is_authenticated = 0;
    char response[2000];
    strcpy(response, LOGOUT_OK);
    write(client_sock, response, strlen(response));
}
