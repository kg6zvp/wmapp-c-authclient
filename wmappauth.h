#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json.h> //json-c
#include <terminput.h>
#include <credentials.h>
#include <objstring.h>
#include <objcurl.h>

/**
 * Necessary to use gethostname(char* destination, int max_string_length)
 */
#ifdef _WIN32
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif


/**
 * Begin actual library stuff
 */
#ifndef _wmappauth_h_
#define _wmappauth_h_

Credentials* current_credentials;

/**
 * Define urls
 */
#define HTTP_PROTO "https://"
#define AUTH_BASE_URL HTTP_PROTO "wmapp.mccollum.enterprises/loginserver/api"

#define AUTH_TOKEN_URL AUTH_BASE_URL "/token"
#define URL_GET_TOKEN AUTH_TOKEN_URL "/getToken"

/**
 * Initialize the library
 */
extern void wmappauth_init();

extern int login(char* username, char* password, char* device_name);

extern int terminal_login();

#endif
