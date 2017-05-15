#include <wmappauth.h>

void print_creds(){
	printf("%s: %s\n", TOKEN_HEADER, current_credentials->token);
	printf("%s: %s\n", SIGNATURE_HEADER, current_credentials->token_signature);
}

/**
 * Test program
 */
int main(int argc, char** argv){
	wmappauth_init(); //initialize the login system (IMPORTANT!)
	int code = terminal_login(); //prompt the user to login, which will save their token and signature in memory
	//if you want to know what the variable code is, it's the HTTP status returned by the server
	print_creds();
	renew_token();
	print_creds();
	persist_credentials();
	return 0;
}
