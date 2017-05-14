#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#define SHOW_INPUT 1
#define HIDE_INPUT 0

#define INPUT_BUFFER_SZ 16

void err_exit(char* msg){
	fprintf(stderr, msg);
	exit(1);
}

char* raw_get_noshow_input(char* prompt){
	#ifdef _WIN32
		//do windows things
		Handle handle;
		DWORD mode;
		
		printf(prompt); //print to console

		/**
		 * Disable echo to read password
		 */
		h = GetStdHandle(STD_INPUT_HANDLE);
		if(GetConsoleMode(h, &mode)){
			mode &= ~ENABLE_ECHO_INPUT;
			SetConsoleMode(h, mode);
		}
		/**
		 * Actually get password
		 */
		//actually get input

		/**
		 * Enable echo so that characters typed into CLI can be read by user again
		 */
		h = GetStdHandle(STD_INPUT_HANDLE);
		if(GetConsoleMode(h, &mode)){
			mode |= ENABLE_ECHO_INPUT;
			SetConsoleMode(h, mode);
		}

		fprintf(stderr, "terminput.c doesn't care about Windows support right now.\n");
		return NULL;
	#else
		return getpass(prompt);
	#endif
}

char* get_input(char* prompt, int show){
	if(show == HIDE_INPUT){ //if we're supposed to suppress output
		return raw_get_noshow_input(prompt);
	}
	int max_len = INPUT_BUFFER_SZ;

	printf(prompt);
	char* data = malloc(max_len*sizeof(char));

	if(data == 0) //something went very wrong
		err_exit("Couldn't allocate memory in terminput.c Exiting...\n");

	int i = 0;
	while(1){ //forever
		int c = getchar();
		if(isspace(c) || c == EOF){
			data[i] = 0; //done
			break;
		}

		data[i] = c; //add the next character
		
		if(i >=max_len-1){
			max_len += INPUT_BUFFER_SZ;
			data = realloc(data, max_len*sizeof(char));
			if(data == 0)
				err_exit("Couldn't allocate buffer");
		}
		++i;
	}
	return data;
}
