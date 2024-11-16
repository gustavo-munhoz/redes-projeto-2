#ifndef CIPH_H
#define CIPH_H

#include <stdio.h>
#include <string.h>

#define AUTH_REQUEST "AUTH_REQUEST"
#define AUTH_RESPONSE_OK "AUTH_OK"
#define AUTH_RESPONSE_ERROR "AUTH_ERROR"
#define ENCRYPT_REQUEST "ENCRYPT"
#define LOGOUT_REQUEST "LOGOUT"
#define LOGOUT_RESPONSE "LOGOUT_OK"

void create_auth_request(char *buffer, const char *id, const char *password);
void create_encrypt_request(char *buffer, const char *text, int shift);
void create_logout_request(char *buffer);

#endif
