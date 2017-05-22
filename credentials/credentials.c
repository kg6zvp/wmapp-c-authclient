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

void credentials_init(Credentials* creds){
	creds->token = NULL;
	creds->token_signature = NULL;
}

Credentials* credentials_new(){
	Credentials* creds = malloc(sizeof(Credentials));
	credentials_init(creds);
	return creds;
}

void credentials_free(Credentials* creds){
	free(creds->token);
	free(creds->token_signature);
	free(creds);
}
