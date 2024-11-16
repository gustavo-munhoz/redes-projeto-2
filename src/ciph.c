#include "ciph.h"

void create_auth_request(char *buffer, const char *id, const char *password) {
	sprintf(buffer, "%s %s %s", AUTH_REQUEST, id, password);
}

void create_encrypt_request(char *buffer, const char *text, int shift) {
	sprintf(buffer, "%s %s %d", ENCRYPT_REQUEST, text, shift);
}

void create_logout_request(char *buffer) {
	sprintf(buffer, "%s", LOGOUT_REQUEST);
}

