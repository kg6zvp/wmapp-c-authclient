#include "objstring.h"

void string_init(ObjString* s){
	s->len = 0;
	s->ptr = malloc(s->len+1);
	s->ptr[0] = '\0';
}

ObjString* string_new(){
	ObjString* s = malloc(sizeof(ObjString));
	string_init(s);
	return s;
}

void string_init_with_data(ObjString* s, char* data, size_t len){
	if(len == 0){ //if no length given, count for it
		s->len = strlen(data);
	}else{
		s->len = len;
	}
	s->ptr = malloc(s->len+1);
	memcpy(s->ptr, data, s->len);
	s->ptr[s->len] = '\0'; //manually zero
}

ObjString* string_new_with_data(char* data, size_t len){
	ObjString* s = malloc(sizeof(ObjString));
	string_init_with_data(s, data, len);
	return s;
}

void string_free(ObjString* s){
	free(s->ptr);
	free(s);
}

int string_append(ObjString* s, ObjString* new_data){
	size_t new_len = s->len + new_data->len;
	s->ptr = realloc(s->ptr, new_len+1);

	if(s->ptr == NULL){
		fprintf(stderr, "realloc() of string failed\n");
		return 0; //error
	}

	memcpy(s->ptr+s->len, new_data->ptr, new_data->len);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return 1; //success
}

int cstring_append(ObjString* s, char* append_data, size_t append_len){
	ObjString* new_data = string_new_with_data(append_data, append_len);
	int ret = string_append(s, new_data);
	string_free(new_data); //we did something inefficient here, but that's okay, we'll fix it later
	return ret;
}

