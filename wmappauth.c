#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <json.h>
#include "terminput.c"
#include <credentials.h>
#include <objstring.h>
//#include <json-c/json.h> //json-c

/**
 * Necessary to use gethostname(char* destination, int max_string_length)
 */
#ifdef _WIN32
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif

#define HTTP_PROTO "https://"
#define AUTH_BASE_URL HTTP_PROTO "wmapp.mccollum.enterprises/loginserver/api"

#define AUTH_TOKEN_URL AUTH_BASE_URL "/token"
#define URL_GET_TOKEN AUTH_TOKEN_URL "/getToken"

#define HTTP_POST "POST"

//#define ecurl(a, b, c) curl_easy_setopt(a, b, c)

Credentials* current_credentials;

size_t raw_write_string(void* ptr, size_t size, size_t nmemb, struct string* s){
	size_t rSize = size*nmemb; //we need it later

	if(!cstring_append(s, ptr, rSize)){
		fprintf(stderr, "Problem doing cstring_append!\n");
	}
	/*size_t new_len = s->len + rSize;
	s->ptr = realloc(s->ptr, new_len+1);

	if(s->ptr == NULL){
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}

	memcpy(s->ptr+s->len, ptr, rSize);
	s->ptr[new_len] = '\0';
	s->len = new_len;//*/

	return rSize;
}

int raw_begins_with(char* str, char* pattern){
	int i = 0;
	int len = strlen(pattern);
	for(; i < len; ++i){
		if(str[i] != pattern[i])
			return 0;
	}
	return 1;
}

/**
 * As defined by libcurl
 */
size_t raw_signature_header_callback(char* buffer, size_t size, size_t nitems, ObjString* token_signature){
	//printf("size: %d\nnitems: %d\n%s\n", size, nitems, buffer);
	if(raw_begins_with(buffer, SIGNATURE_HEADER)){
		if(token_signature != NULL){
			string_free(token_signature);
		}
		char* mbuff = &(buffer[16]);
		token_signature = string_new_with_data(mbuff, size*nitems-16 - 2); //-2 to get rid of the new line characters
		//printf("Signature found: %s\n", token_signature->ptr);
		/*credential_obj->token_signature = malloc(size*nitems-15); // = string_new_with_data(mbuff, size*nitems-16);
		memcpy(credential_obj->token_signature, &(buffer[16]), size*nitems-16);
		credential_obj->token_signature[size*nitems-16] = '\0';
		printf("tokenSignature: %s\n", credential_obj->token_signature);//*/
	} //*/
	return size*nitems;
}

CURL* curl_hnd;

/**
 * Initialize the library
 */
void wmappauth_init(){
	curl_hnd = curl_easy_init();
}

/**
 * Internal use function just to separate out code a bit more clearly
 */
char* int_mk_json_credenials(char* username, char* password, char* device_name){
	if(username==NULL||password==NULL)
		return " ";
	struct json_object* creds = json_object_new_object();
	json_object_object_add(creds, "username", json_object_new_string(username));
	json_object_object_add(creds, "password", json_object_new_string(password));
	json_object_object_add(creds, "devicename", json_object_new_string(device_name));
	return json_object_to_json_string_ext(creds, JSON_C_TO_STRING_PLAIN);
}

int raw_login(char* username, char* password, char* device_name){
	CURLcode result;

	if(device_name == NULL){ //If they didn't pass a device name, populate the info
		device_name = malloc(128*sizeof(char)); //allocate space for 128 characters
		gethostname(device_name, 127); //get the hostname from the machine and write it into the buffer
	}

	char* cred_json = int_mk_json_credenials(username, password, device_name); //malloc(256*sizeof(char)); //if it's longer than 256 characters, there's something really weird going on here

	//printf("Logging in with: %s\n", cred_json);
	
	/**
	 * Initialize headers with data for json
	 */
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Accept: application/json");
	
	/*TODO: Replace with Macro*/
	struct string* response_body = malloc(sizeof(struct string));
	string_init(response_body);

	struct string* signature_header = malloc(sizeof(struct string));
	string_init(signature_header);

	curl_easy_setopt(curl_hnd, CURLOPT_URL, URL_GET_TOKEN); //set login url
	curl_easy_setopt(curl_hnd, CURLOPT_CUSTOMREQUEST, HTTP_POST); //make it a post request
	curl_easy_setopt(curl_hnd, CURLOPT_POSTFIELDS, cred_json); //set post body
	curl_easy_setopt(curl_hnd, CURLOPT_USERAGENT, "wmapp-desktop-c/1.0"); //set the user agent
	curl_easy_setopt(curl_hnd, CURLOPT_HTTPHEADER, headers); //add the headers

	curl_easy_setopt(curl_hnd, CURLOPT_WRITEFUNCTION, raw_write_string);
	curl_easy_setopt(curl_hnd, CURLOPT_WRITEDATA, response_body);

	curl_easy_setopt(curl_hnd, CURLOPT_HEADERFUNCTION, raw_signature_header_callback);
	curl_easy_setopt(curl_hnd, CURLOPT_HEADERDATA, signature_header);

	int ret = curl_easy_perform(curl_hnd);
	curl_slist_free_all(headers);
	headers = NULL;

	printf("%s: %s\n", TOKEN_HEADER, response_body->ptr);
	printf("%s: %s\n", SIGNATURE_HEADER, signature_header->ptr);
	string_free(response_body);
	string_free(signature_header);
	/*free(response_body->ptr);
	free(response_body);//*/

	return 200; //success
}

/**
 * Intended for external use
 */
int terminal_login(){
	//get input
	char* username = get_input("Username: ", SHOW_INPUT);
	char* password = get_input("Password: ", HIDE_INPUT);

	int login_code = raw_login(username, password, NULL);
	return login_code;
}

/**
 * Test program
 */
int main(int argc, char** argv){
	wmappauth_init();
	terminal_login(); //try login, I dare ya

	return 0;
}


