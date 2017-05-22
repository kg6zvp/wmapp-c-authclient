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
	if(device_name != NULL)
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

	objcurl_free_request(request);
	objcurl_free_response(response);

	return return_status;
}

int raw_persist_credentials(){
	ObjString* cred_fname = string_new_with_data(data_dir, 0); //'cause I'm lazy
	cstring_append(cred_fname, "/wmappauth.bin", 0); //'cause I'm even lazier :P

	FILE* f = fopen(cred_fname->ptr, "w+"); //yee
	string_free(cred_fname); //at least I did this

	fwrite(current_credentials->token, sizeof(char)*(strlen(current_credentials->token)+1), 1, f);
	fwrite(current_credentials->token_signature, sizeof(char)*(strlen(current_credentials->token_signature)+1), 1, f);
	fclose(f); //don't need it anymore, why keep it open?
	return 1;
}

int raw_load_credentials(){
	ObjString* cred_fname = string_new_with_data(data_dir, 0); //'cause I'm lazy
	cstring_append(cred_fname, "/wmappauth.bin", 0); //'cause I'm even lazier :P

	FILE* f = fopen(cred_fname->ptr, "r"); //not including b 'cause we don't care about Windows :'|
	string_free(cred_fname);
	if(f){ //exists/readable
		//
		struct stat fst;
		fstat(fileno(f), &fst);
		long size = fst.st_size;
		void* temp_store = malloc(size); //allocate enough space to read the whole file (risky, but probably okay!)
		int n = fread(temp_store, size, 1, f);
		fclose(f); //we're done with it now (it being the file we were reading data from)
		
		/*
		 * Populate the token variable
		 */
		int token_len = strlen((char*)temp_store);
		char* token = malloc(token_len+1); //create the thing we're storing the token in
		memcpy(token, temp_store, token_len+1); //we can copy the null terminator because we made the file
		credentials_set_token(current_credentials, token);

		/*
		 * Populate the signature variable
		 */
		char* signature_temp = temp_store+token_len+1; //add the length of the token plus 1 to move past the NULL terminator
		int sig_len = strlen((char*)signature_temp); //cast is redundant, but matches
		char* token_signature = malloc(sig_len+1); //again, copying the null terminator (I know it's risky, shut up)
		memcpy(token_signature, signature_temp, sig_len+1); //copy the null into it
		credentials_set_token_signature(current_credentials, token_signature);

		free(temp_store); //free the memory used to store the file data
	}else{ // :'(
		return 0; //return bad
	}
	return 1;
}

int login(char* username, char* password, char* device_name){
	int devmalloc = 0;
	if(device_name == NULL){ //If they didn't pass a device name, populate the info
		devmalloc = 1;
		device_name = malloc(128*sizeof(char)); //allocate space for 127 characters + null terminator
		gethostname(device_name, 127); //get the hostname from the machine and write it into the buffer
		device_name[127] = '\0'; //set last one to null just in case
	}
	int status_code = raw_login(username, password, device_name);
	if(devmalloc){
		free(device_name);
	}
	return status_code;
}

int renew_token(){
	return raw_renew_token();
}

struct linkedlist* list_tokens(){
	//Get the tokens from server
	struct curl_request* request = objcurl_new_request();
	request->url = string_new_with_data(URL_LIST_TOKENS, 0);
	raw_add_auth_headers(request);
	struct curl_response* response = objcurl_perform(request);
	//TODO: Parse the json and separate the tokens into a list
	return NULL;
}

int persist_credentials(){
	if(current_credentials == NULL)
		return 0;
	if(current_credentials->token == NULL
			|| current_credentials->token_signature == NULL)
		return 0;
	return raw_persist_credentials(); //success?
}

int load_credentials(){
	if(current_credentials == NULL)
		return 0;
	return raw_load_credentials();
}

/**
 * Intended for external use
 */
int terminal_login(){
	//get input
	char* username = get_input("Username: ", SHOW_INPUT);
	char* password = get_input("Password: ", HIDE_INPUT);
	int response_code = login(username, password, NULL);
	free(username); //don't need it anymore
	free(password); //we're done with this, don't need to keep it in memory
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
 * Helper function to get the time in milliseconds
 */
int64_t raw_get_current_time_millis(){
	/*struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return lround( spec.tv_nsec ); //nano to millis//*/
	time_t t;
	time(&t);
	return t*1000; //get milliseconds
}

int64_t system_currenttimemillis(){
	return raw_get_current_time_millis();
}

int credentials_are_expired(){
	json_object* token_object = json_tokener_parse(current_credentials->token);
	//assume it's type is object (come on! It's 2017)
	json_object* expirationDate_object = json_object_object_get(token_object, "expirationDate"); //bad to use "magic string," but need to test it

	long time = raw_get_current_time_millis();
	long expiration_date = json_object_get_int64(expirationDate_object);
	
	json_object_put(token_object); //free memory (no idea why it's called put)

	return (time >= expiration_date);
}

/**
 * Initialize the library
 */
void wmappauth_init(){
	/*
	 * implement XDG Directory specification (specifies where applications should put cache, data and configuration files for each user)
	 */
	char* base_dir = getenv("XDG_DATA_HOME"); //get data directory
	if(base_dir == NULL||strlen(base_dir) < 2){ //if it's not defined, but this really shouldn't happen
		char* home_dir = getenv("HOME");
		char* append_dir = "/.local/share/";
		base_dir = malloc(strlen(home_dir)+strlen(append_dir)+1); //should use sizeof(char)
		memcpy(base_dir, home_dir, strlen(home_dir)); //should use sizeof(char)
		memcpy(&(base_dir[strlen(home_dir)]), append_dir, strlen(append_dir)); //should use sizeof(char)
		base_dir[strlen(home_dir)+strlen(append_dir)] = '\0'; //could be a stored value, but oh well
	}
	wmappauth_init_dir(base_dir);
}

int raw_mkpath(const char *s, mode_t mode){
	char *q, *r = NULL, *path = NULL, *up = NULL;
	int rv;

	rv = -1;
	if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0)
		return (0);
	if ((path = strdup(s)) == NULL)
		goto out;
	if ((q = strdup(s)) == NULL)
		goto out;
	if ((r = dirname(q)) == NULL)
		goto out;
	if ((up = strdup(r)) == NULL)
		goto out;
	if ((raw_mkpath(up, mode) == -1) && (errno != EEXIST))
		goto out;
	if ((mkdir(path, mode) == -1) && (errno != EEXIST))
		rv = -1;
	else
		rv = 0;

out:
	if (up != NULL)
		free(up);
	free(q);
	free(path);
	return (rv);
}

int raw_update_user_info(char* username, char* password){
	char* cred_json = raw_mk_json_credenials(username, password, NULL);

	struct curl_request* request = objcurl_new_request();
	request->request_method = HTTP_POST;
	request->url = string_new_with_data(URL_USER_UPDATE_DATABASE, 0);
	request->body = string_new_with_data(cred_json, 0);
	free(cred_json);
	objcurl_add_request_header(request, string_new_with_data("Content-Type: application/json", 30));
	objcurl_add_request_header(request, string_new_with_data("Acceps: application/json", 24));

	struct curl_response* response = objcurl_perform(request);

	if(response->status_code == 200){
		printf("Received response: %s\n", response->response_body->ptr);
		objcurl_free_request(request);
		objcurl_free_response(response);
		return 1;
	}
	printf("Received status code: %d\nand response: %s\n", response->status_code, response->response_body->ptr);
	return 0;
}

/**
 * Right now it doesn't do anything special other than pass the work along to the function for internal use, but it may become important later
 */
int update_user_info(char* username, char* password){
	return raw_update_user_info(username, password);
}

int terminal_update_user_info(){
	char* username = get_input("Username: ", SHOW_INPUT);
	char* password = get_input("Password: ", HIDE_INPUT);
	int code = update_user_info(username, password);
	free(username);
	free(password);
	return code;
}

void wmappauth_init_dir(char* base_dir){
	/*
	 * figure out which directory to use inside the base directory
	 * (this maybe should go in wmappauth_init() function which takes no args)
	 */
	//char* dir_name = "wmappauth";
	char* dir_name = SERVER_ADDRESS;
	data_dir = malloc(strlen(base_dir)+strlen(dir_name)+1);
	memcpy(data_dir, base_dir, strlen(base_dir));
	memcpy( &(data_dir[strlen(base_dir)]), dir_name, strlen(dir_name));
	data_dir[strlen(base_dir)+strlen(dir_name)] = '\0';

	/*
	 * Create config directory if necessary (just does it anyway and it will silently fail if it already exists, so cool)
	 */
	raw_mkpath(data_dir, 0700);
	
	objcurl_init();
	current_credentials = credentials_new();
}

void wmappauth_deinit(){
	free(data_dir); //don't need to keep this around
	/*
	 * Free credentials
	 */
	credentials_free(current_credentials);
}


