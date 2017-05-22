#include <stdlib.h>
#include <string.h>

#ifndef _mc_credentials_h_

#define _mc_credentials_h_

#define SIGNATURE_HEADER "TokenSignature"
#define TOKEN_HEADER "Token"

/**
 * This struct is designed to hold credentials for user accounts
 */
struct Credentials{
	char* token;
	char* token_signature;
};

typedef struct Credentials Credentials;

/**
 * Set the token, freeing any old memory if necessary
 */
extern void credentials_set_token(Credentials* credentials, char* token);

/**
 * Set the token signature, freeing any old memory if necessary
 */
extern void credentials_set_token_signature(Credentials* credentials, char* token_signature);

extern void credentials_init(Credentials* creds);

extern Credentials* credentials_new();

extern void credentials_free(Credentials* creds);

#endif
