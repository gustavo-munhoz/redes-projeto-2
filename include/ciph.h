#ifndef CIPH_H
#define CIPH_H

#include <stdio.h>
#include <string.h>

#define AUTH_REQUEST "AUTH_REQUEST"
#define AUTH_OK "AUTH_OK"
#define AUTH_ERROR "AUTH_ERROR"
#define NOT_AUTHENTICATED "NOT_AUTHENTICATED"
#define ENCRYPT_REQUEST "ENCRYPT"
#define ENCRYPTED_TEXT "ENCRYPTED_TEXT"
#define ENCRYPT_ERROR "ENCRYPT_ERROR"
#define LOGOUT_REQUEST "LOGOUT"
#define LOGOUT_OK "LOGOUT_OK"
#define UNKNOWN_COMMAND "UNKNOWN_COMMAND"

void create_auth_request(char *buffer, const char *id, const char *password);
void create_encrypt_request(char *buffer, const char *text, int shift);
void create_logout_request(char *buffer);

#endif
