#include <objcurl.h>

int raw_begins_with(char* str, char* pattern){
	int i = 0;
	int len = strlen(pattern);
	for(; i < len; ++i){
		if(str[i] != pattern[i])
			return 0;
	}
	return 1;
}

size_t raw_header_callback(char* buffer, size_t size, size_t nitems, struct curl_response* response){
	ObjString* header = string_new_with_data(buffer, size*nitems-2); //-2 to get rid of the newline characters
	
	if(response->response_headers == NULL){
		response->response_headers = linkedlist_new();
		response->response_headers->ptr = header;
	}else{
		linkedlist_append(response->response_headers, header);
	}
	
	return size*nitems;
}

size_t raw_write_string(void* ptr, size_t size, size_t nmemb, struct string* s){
	size_t rSize = size*nmemb; //we need it later

	if(!cstring_append(s, ptr, rSize)){
		fprintf(stderr, "Problem doing cstring_append!\n");
	}

	return rSize;
}

/**
 * Initialize Objective Curl
 */
void objcurl_init(){
	//nothing
}

struct curl_slist* raw_get_curl_headers(struct curl_request* request){
	struct curl_slist* curl_headers = NULL;
	LinkedList* current = request->request_headers;
	while(current != NULL){
		ObjString* header_str = (ObjString*)(current->ptr);
		curl_headers = curl_slist_append(curl_headers, header_str->ptr); //append it to the list
		current = current->next; //finally, update pointer
	}
	return curl_headers;
}

/**
 * perform a curl operation based on a request object
 */
struct curl_response* objcurl_perform(struct curl_request* request){
	/*
	 * Create response object for population
	 */
	struct curl_response* response = malloc(sizeof(struct curl_response));
	response->response_headers = NULL;
	response->response_body = string_new();
	response->status_code = -1; //nothing yet
	
	CURL* hnd = curl_easy_init();
	
	curl_easy_setopt(hnd, CURLOPT_URL, request->url->ptr);
	if(request->request_method != NULL){
		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, request->request_method); //set the method to post
	}
	if(request->body != NULL){
		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, request->body->ptr);
	}
	
	/**
	 * Tell curl to write the data in appropriately
	 */
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, raw_write_string);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, response->response_body);
	curl_easy_setopt(hnd, CURLOPT_HEADERFUNCTION, raw_header_callback);
	curl_easy_setopt(hnd, CURLOPT_HEADERDATA, response);

	/**
	 * Put the headers in the request with curl
	 */
	struct curl_slist* curl_headers = NULL;
	if(request->request_headers != NULL){
		curl_headers = raw_get_curl_headers(request);

		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, curl_headers); //actually add the headers
	}

	int ret = curl_easy_perform(hnd);
	

	if(request->request_headers != NULL){
		curl_slist_free_all(curl_headers);
	}
	curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &(response->status_code)); //set the response code

	curl_easy_cleanup(hnd);
	return response;
}

/**
 * Find a header in a request, return null if it doesn't exist
 *
 * @param response: response to search in
 * @param header: header to search for
 */
char* objcurl_find_header(struct curl_response* response, char* header_name){
	LinkedList* current = response->response_headers;
	//printf("objcurl_find_header()\n");
	while(current != NULL){
		int cmpstr_len = strlen(header_name)+2; //+1 to accommodate null terminator, +2 to accommodate ": " in between
		char* current_header = ((ObjString*)(current->ptr))->ptr;
		//char* cmpstr = malloc(cmpstr_len); //not necessary right now
		if(raw_begins_with(current_header, header_name)){ //if it's the same header, return it
			char* data_location = &( current_header[cmpstr_len] );
			size_t data_len = (size_t)(strlen(data_location) * sizeof(char));
			
			char* header_data = malloc(data_len+1); //add null pointer
			memcpy(header_data, data_location, data_len); //copy data
			header_data[data_len] = '\0'; //set null terminator
			return header_data; //now just return
		}
		current = current->next; //iterate to the next one
	}
	return NULL;
}

void objcurl_add_request_header(struct curl_request* request, ObjString* header){
	if(request->request_headers == NULL){
		request->request_headers = linkedlist_new();
		request->request_headers->ptr = header;
	}else{
		linkedlist_append(request->request_headers, header);
	}
}

struct curl_request* objcurl_new_request(){
	struct curl_request* request = malloc(sizeof(struct curl_request)); //create it
	request->request_method = NULL;
	request->url = NULL;
	request->body = NULL;
	request->request_headers = NULL;
	return request;
}

void objcurl_free_request(struct curl_request* request){
	string_free(request->url);
	if(request->body != NULL){
		string_free(request->body);
	}
	if(request->request_headers != NULL){
		linkedlist_free(request->request_headers, string_free);
	}
	free(request);
}

void objcurl_free_response(struct curl_response* response){
	if(response->response_headers != NULL)
		linkedlist_free(response->response_headers, string_free);
	if(response->response_body != NULL)
		string_free(response->response_body);
	free(response);
}



