#include <stdlib.h>
#include <stdio.h> //for error message
#include <string.h>
#include <objstring.h>
#include <curl/curl.h>
#include <linkedlist.h>

#ifndef _objcurl_h_
#define _objcurl_h_

/**
 * Define HTTP methods
 */
#define HTTP_POST "POST"
#define HTTP_GET "GET"
#define HTTP_DELETE "DELETE"

struct curl_request{
	char* request_method; //char* instead of ObjString* because we're using curl and don't need the overhead
	ObjString* url;
	LinkedList* request_headers; //ObjString*
	ObjString* body; //if there is one, may not be printable characters
	//TODO: Add NULLable function pointers for parsing headers and/or reading message body. If these are included, we can disregard returning the headers or body in the response
};

struct curl_response{
	LinkedList* response_headers; //ObjString*
	ObjString* response_body; //may not be ascii string
	long status_code;
};

/**
 * Initialize Objective Curl
 */
extern void objcurl_init();

/**
 * Perform a curl request using the given curl_request object
 *
 * @param request: the request you want to make
 */
extern struct curl_response* objcurl_perform(struct curl_request* request);

/**
 * Find a header in a request, return null if it doesn't exist
 *
 * @param response: response to search in
 * @param header: header to search for
 */
extern char* objcurl_find_header(struct curl_response* response, char* header_name);

/**
 * Add a header to the request
 *
 * NOTE: requires request->request_headers to be set to NULL before adding the first one or there will be an error. No additional requirements after that. It will just work.
 */
extern void objcurl_add_request_header(struct curl_request* request, ObjString* header);

/**
 * Create a new request object, properly initialized to zero
 */
extern struct curl_request* objcurl_new_request();

/**
 * Free all the memory in the given request
 */
extern void objcurl_free_request(struct curl_request* request);

/**
 * Free all the memory in the given response
 */
extern void objcurl_free_response(struct curl_response* response);

#endif


