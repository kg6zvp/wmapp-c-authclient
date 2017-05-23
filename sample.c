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
	int code = -1;
	/**
	 * If this user hasn't signed in, ask them to login until they are successful (response 200 is the HTTP success response code, indicating login was successful)
	 */
	if(!load_credentials()){
		while(code != 200){
			code = terminal_login(); //prompt the user to login, which will save their token and signature in memory
		}
	}
	//if you want to know what the variable 'int code' is, it's the HTTP status returned by the server
	print_creds();
	/**
	 * Check if the credentials are expired and, if so, ask them to login again
	 */
	if(credentials_are_expired()){
		printf("credentials expired\n");
		code = terminal_login();
	}else{
		printf("credentials not expired\n");
	}
	/**
	 * Renew their token to keep them logged in (do this at least once every two-three weeks to keep it from expiring, but not more often than every couple of days)
	 */
	renew_token();
	print_creds();
	/**
	 * Store their credentials to the disk so that they can be loaded next time
	 */
	persist_credentials();
	/**
	 * Deinitialize the library, prepare for end of program (or at least, end of use of wmappauth in program)
	 */
	wmappauth_deinit();
	return 0;
}
