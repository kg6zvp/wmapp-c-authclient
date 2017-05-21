#include <credentials.h>

void credentials_set_token(Credentials* credentials, char* token){
	if(credentials->token == token)
		return;
	if(credentials->token != NULL){
		free(credentials->token);
	}
	credentials->token = token;
}

void credentials_set_token_signature(Credentials* credentials, char* token_signature){
	if(credentials->token_signature == token_signature)
		return;
	if(credentials->token_signature != NULL){
		free(credentials->token_signature);
	}
	credentials->token_signature = token_signature;
}
