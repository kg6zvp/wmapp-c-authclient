#include <wmappauth.h>

/**
 * Test program
 */
int main(int argc, char** argv){
	wmappauth_init(); //initialize the login system (IMPORTANT!)
	int code = terminal_login(); //prompt the user to login, which will save their token and signature in memory
	//if you want to know what the variable code is, it's the HTTP status returned by the server
	return 0;
}
