#include <wmappauth.h>

/**
 * Internal use function just to separate out code a bit more clearly
 */
char* raw_mk_json_credenials(char* username, char* password, char* device_name){
	if(username==NULL||password==NULL)
		return " \0";
	struct json_object* creds = json_object_new_object();
	json_object_object_add(creds, "username", json_object_new_string(username));
	json_object_object_add(creds, "password", json_object_new_string(password));
	json_object_object_add(creds, "devicename", json_object_new_string(device_name));
	return json_object_to_json_string_ext(creds, JSON_C_TO_STRING_PLAIN);
}

int raw_login(char* username, char* password, char* device_name){
	char* cred_json = raw_mk_json_credenials(username, password, device_name); //make the credential object
	
	/*struct curl_request* request = malloc(sizeof(struct curl_request)); //create it
	request->request_method = HTTP_POST;
	request->url = string_new_with_data(URL_GET_TOKEN, sizeof(char)*strlen(URL_GET_TOKEN) );
	request->body = string_new_with_data(cred_json, sizeof(char)*strlen(cred_json) );
	free(cred_json); //we don't need it anymore since we already made an object string out of it
	request->request_headers = NULL;//*/

	struct curl_request* request = objcurl_new_request();
	request->request_method = HTTP_POST;
	request->url = string_new_with_data(URL_GET_TOKEN, sizeof(char)*strlen(URL_GET_TOKEN) );
	request->body = string_new_with_data(cred_json, sizeof(char)*strlen(cred_json) );
	free(cred_json); //we don't need it anymore since we already made an object string out of it
	objcurl_add_request_header(request, string_new_with_data("Content-Type: application/json", 30));
	objcurl_add_request_header(request, string_new_with_data("Accept: application/json", 24));
	
	struct curl_response* response = objcurl_perform(request);
	
	int return_status = response->status_code;

	if(return_status == 200){ //if the login was successful
		char* token = malloc( sizeof(char)*(response->response_body->len+1) );
		memcpy(token, response->response_body->ptr, response->response_body->len); //create a copy because we're about to free the memory
		token[response->response_body->len] = '\0'; //just in case
		char* signature = objcurl_find_header(response, SIGNATURE_HEADER);
		
		credentials_set_token(current_credentials, token);
		credentials_set_token_signature(current_credentials, signature);
	}else if(return_status == 401){ //they gave the wrong username or password, no error
	}else{
		fprintf(stderr, "wmappauth %d: %s\n", return_status, response->response_body->ptr);
	}
	/*
	 * Free memory here either way
	 */
	objcurl_free_request(request);
	objcurl_free_response(response);

	return return_status; //success?
}

void raw_add_auth_headers(struct curl_request* request){
	ObjString* token_header = string_new_with_data(TOKEN_HEADER, strlen(TOKEN_HEADER));
	cstring_append(token_header, ": ", 2);
	cstring_append(token_header, current_credentials->token, strlen(current_credentials->token));
	objcurl_add_request_header(request, token_header);

	ObjString* signature_header = string_new_with_data(SIGNATURE_HEADER, strlen(SIGNATURE_HEADER));
	cstring_append(signature_header, ": ", 2);
	cstring_append(signature_header, current_credentials->token_signature, strlen(current_credentials->token_signature));
	objcurl_add_request_header(request, signature_header);

	/*
	 * Add headers for content type
	 */
	objcurl_add_request_header(request, string_new_with_data("Content-Type: application/json", 30));
	objcurl_add_request_header(request, string_new_with_data("Accept: application/json", 24));
}

int raw_renew_token(){
	struct curl_request* request = objcurl_new_request();
	request->url = string_new_with_data(URL_RENEW_TOKEN, sizeof(char)*strlen(URL_RENEW_TOKEN) );
	raw_add_auth_headers(request);
	
	struct curl_response* response = objcurl_perform(request);
	int return_status = response->status_code;

	if(return_status == 200){ //good, save it to the cache
		char* token = malloc( sizeof(char)*(response->response_body->len+1) );
		memcpy(token, response->response_body->ptr, response->response_body->len); //create a copy because we're about to free the memory
		token[response->response_body->len] = '\0'; //just in case
		char* signature = objcurl_find_header(response, SIGNATURE_HEADER);
		
		credentials_set_token(current_credentials, token);
		credentials_set_token_signature(current_credentials, signature);
	}else if(return_status == 500){ //error, so keep the old credentials
		fprintf(stderr, "wmappauth %d: %s\n", return_status, response->response_body->ptr);
	}else if(return_status == 401){ //maybe warn the user, but delete their credentials
		credentials_set_token(current_credentials, NULL);
		credentials_set_token_signature(current_credentials, NULL);
		fprintf(stderr, "wmappauth %d: %s\n", return_status, response->response_body->ptr);
	}else{
		fprintf(stderr, "wmappauth %d: %s\n", return_status, response->response_body->ptr);
	}

	objcurl_free_request(request); //problem?
	objcurl_free_response(response);

	return return_status;
}

int login(char* username, char* password, char* device_name){
	if(device_name == NULL){ //If they didn't pass a device name, populate the info
		device_name = malloc(128*sizeof(char)); //allocate space for 127 characters + null terminator
		gethostname(device_name, 127); //get the hostname from the machine and write it into the buffer
		device_name[127] = '\0'; //set last one to null just in case
	}
	return raw_login(username, password, device_name);
}

int renew_token(){
	return raw_renew_token();
}

/**
 * Intended for external use
 */
int terminal_login(){
	//get input
	char* username = get_input("Username: ", SHOW_INPUT);
	char* password = get_input("Password: ", HIDE_INPUT);
	int response_code = login(username, password, NULL);
	if(response_code == 200){
		printf("Login successful!\n");
	}else if(response_code == 401){
		printf("Incorrect username or password\n");
	}else if(response_code == 500){
		printf("There was a problem with the server\n");
	}else{
		printf("Unknown code: %d\n", response_code);
	}
	return response_code;
}

/**
 * Initialize the library
 */
void wmappauth_init(){
	objcurl_init();
	current_credentials = malloc(sizeof(Credentials));
	current_credentials->token = NULL; //in case
	current_credentials->token_signature = NULL; //in case
}


