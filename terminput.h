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

extern char* get_input(char* prompt, int show);
