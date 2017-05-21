#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * json-c
 */
#include <json.h>
#include <json_tokener.h> //for parsing

/*
 * my libraries
 */
#include <terminput.h>
#include <credentials.h>
#include <objstring.h>
#include <objcurl.h>

#include <sys/stat.h> //for XDG Directory specification
#include <sys/types.h> //for XDG Directory specification
#include <errno.h> //for XDG Directory specification

/*
 * Credential expiration, time stuff
 */
#include <time.h>
#include <math.h>

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
#ifndef _mc_auth_h_
#define _mc_auth_h_

Credentials* current_credentials;

char* data_dir;
char* config_dir; //in case
char* cache_dir; //probs not necessary

/**
 * Define urls
 */
#define HTTP_PROTO "https://"
#define SERVER_ADDRESS "wmapp.mccollum.enterprises"
#define AUTH_BASE_URL HTTP_PROTO SERVER_ADDRESS "/loginserver/api"

#define AUTH_TOKEN_URL AUTH_BASE_URL "/token"

#define URL_GET_TOKEN AUTH_TOKEN_URL "/getToken"
#define URL_RENEW_TOKEN AUTH_TOKEN_URL "/renewToken"
#define URL_IS_TOKEN_VALID AUTH_TOKEN_URL "/tokenValid"
#define URL_INVALIDATE_TOKEN AUTH_TOKEN_URL "/invalidateToken/%d"
#define URL_LIST_TOKENS AUTH_TOKEN_URL "/listTokens"
#define URL_SUBSCRIBE_TO_INVALIDATION AUTH_TOKEN_URL "/subscribeToInvalidation"

/**
 * Initialize the library, guessing config directory via XDG
 */
extern void wmappauth_init();

/**
 * Initialize the library with the given directory for storing config files
 */
extern void wmappauth_init_dir(char* base_dir);

/**
 * Free memory and prepare for shutdown
 */
extern void wmappauth_deinit();

/**
 * Login to the system and store the result in the credential cache
 *
 * @param username: username, can include @westmont.edu
 * @param password: password
 * @param device_name: can be NULL, optional name of device
 *
 * @return int: HTTP status code from server
 */
extern int login(char* username, char* password, char* device_name);

/**
 * Renew token
 */
extern int renew_token();

/**
 * Check whether the current token is valid
 */
extern int is_token_valid();

/**
 * Invalidate/logout the current token
 */
extern int invalidate_token();

/**
 * List the user's outstanding tokens
 */
extern struct linkedlist* list_tokens();

/**
 * Present the user with a login prompt in the terminal and store the result in the credential cache if successful
 */
extern int terminal_login();

/**
 * Save the token and signature to the store (probably something like ~/.config/wmapp/auth)
 */
extern int persist_credentials();

/**
 * Load the token and signature from the store and into memory
 *
 * @return int: 0 for unsuccessful, 1 for successful at loading
 */
extern int load_credentials();

/**
 * Checks if the credentials are expired according to the computer's current date/time
 *
 * NOTE: returns 1 if no credentials are present
 *
 * @return int: 1 if they are expired, 0 if they are still valid
 */
extern int credentials_are_expired();

/**
 * Emulate Java's System.currentTimeMillis() call
 *
 * return int64_t: The number of Milliseconds since the Epoch (not millisecond precise, should be used only for approximations)
 */
extern int64_t system_currenttimemillis();

#endif


