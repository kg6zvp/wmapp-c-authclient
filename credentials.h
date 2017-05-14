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

#endif
