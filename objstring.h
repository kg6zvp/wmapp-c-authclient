#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct string {
	char* ptr;
	size_t len;
};

typedef struct string ObjString;

extern void string_init(ObjString* s);

extern ObjString* string_new();

extern void string_init_with_data(ObjString* s, char* data, size_t len);

extern ObjString* string_new_with_data(char* data, size_t len);

extern void string_free(ObjString* s);

extern int string_append(ObjString* s, ObjString* new_data);

extern int cstring_append(ObjString* s, char* append_data, size_t append_len);
