#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

//StackOverflow
struct string {
	char* ptr;
	size_t len;
};

struct string* s;

void init(){
	s = malloc(sizeof(struct string));
	s->len = 0;
	s->ptr = malloc(s->len+1);
	s->ptr[0] = '\0';
}

size_t writeString(void* ptr, size_t size, size_t nmemb, struct string* s){
	size_t rSize = size*nmemb; //we need it later

	size_t new_len = s->len + rSize;
	s->ptr = realloc(s->ptr, new_len+1);

	if(s->ptr == NULL){
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}

	memcpy(s->ptr+s->len, ptr, rSize);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return rSize;
}


int main(int argc, char** argv){
	init();
	CURL* curl;

	CURLcode result; //static for some reason
	
	curl = curl_easy_init();
	if(!curl){ //if curl is initialized
		exit(EXIT_FAILURE);
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://canhazip.com");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);

	result = curl_easy_perform(curl);
	
	printf("output: %s", s->ptr);
	free(s->ptr);
	free(s);

	curl_easy_cleanup(curl);

	return 0;
}


