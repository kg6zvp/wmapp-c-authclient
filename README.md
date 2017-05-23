---
header-includes:
- \usepackage{listings}
- \lstset{breaklines=true}
- \lstset{prebreak=,}
- \lstset{postbreak=,}
---

#C Auth SDK

This library currently can be compiled and linked statically or dynamically

##Usage

See `sample.c`

For documentation of functions, see wmappauth.h

The library needs to be initialized with `wmappauth_init()` and properly closed with `wmappauth_deinit()`.

Most functions which return an integer are returning the HTTP status code returned by the server. The only exceptions are functions which do not directly invoke HTTP calls such as `credentials_are_expired()`, `persist_credentials()`, `load_credentials()`, `is_token_valid` and `terminal_login()` which are using int as a `boolean` value

###Login

Login as `erichtofen` on his favorite device:
``` {.c}
int response = login("erichtofen", "oneStupidLongTestPassword23571113", "myFavoriteDevice");
```

The device name can also be ommited by passing `NULL` and the system's hostname will be used
``` {.c}
int response = login("erichtofen", "oneStupidLongTestPassword23571113", NULL);
```

As usual, the `int` returned from the login function is the HTTP status code that the server responded with. This allows you as the developer a lot more flexibility with regard to presenting error messages, prompting for login information again, etc.

###Renew token

In order to stay logged in, a user's token needs to be renewed before it expires. (This is a normal practice which is readily _Googled_)

``` {.c}
int response = renew_token();
```

As usual, the `int` returned from the renew function is the HTTP status code the server responded with.

###Invalidate token/logout

``` {.c}
int response = invalidate_token();
```

As usual, the `int` returned from the renew function is the HTTP status code the server responded with.

###Initialize library

The library needs to be initialized with one of the following two function calls:

This one sets up the data directory following the XDG directory standard, automatically choosing between the right environment variables if the XDG environment variables aren't present.
``` {.c}
wmappauth_init();
```

or if you wish to specify a root directory for the user data files

``` {.c}
wmappauth_init_dir(".local/share");
```

###Deinitialize (Clean up)

The library should be deinitialized in order to free the memory of all of the data it was storing (credentials, etc.)

Do this by inserting a line in your program (probably just before the `return 0;` in main):

``` {.c}
wmappauth_deinit();
```

###Check Token for validity

Check to see if a token is valid, returns 0 for invalid, 1 for valid and -1 for error

``` {.c}
int code = is_token_valid();
```

###Get a list of the user's tokens

Returns a linkedlist\* containing ObjString\* of the json representation of each of their tokens.

``` {.c}
struct linkedlist* tokens = list_tokens();
```

###Present a login prompt using the terminal

You could write this yourself, but it's provided here as a convenience. It prompts the user to enter their username and password (obscured) and performs login with the server, returning the HTTP status code from the server as the result.

``` {.c}
int response = terminal_login();
```

###Save credentials

Saves the credentials to the storage device

Save the token to the hard drive/SSD/etc. (in the XDG User Data Directory unless a base directory was given in the initialization) and return a boolean value of 0 or 1 to indicate success or failure.

``` {.c}
int success_saving_credentials = persist_credentials();
```

###Load credentials

Loads the credentials from the storage device (Again, loads from the XDG User Data Directory unless a base directory was given in initialization) and returns a boolean value of 0 or 1 to indicate success or failure.

There is a good example of usage in sample.c:

``` {.c .numberLines}
int code = 0;
if(!load_credentials()) //if the credentials couldn't be loaded from the disk (presumably because they don't exist)
	while(code != 200) //if the server's response wasn't a 200, prompt for login again
		code = terminal_login(); //prompt for login
```

###Credential Expiration

Check to see if credentials are expired. A true (or 1) value indicates that the credentials you have are expired and a false (or 0) value indicates that they are still valid

``` {.c .numberLines}
#include <wmappauth.h>
...
void my_func(...){
	if(credentials_are_expired()){ //if the credentials are expired, present a login prompt
		terminal_login();
	}
}
```

###Get the credentials

Use cases include allowing multiple people to use your application and swap out who is signed in currently

In case you want to use the credentials to perform a curl request manually, or to somehow login over a regular UNIX socket with a special server you wrote, use this function:

```{.c}
Credentials* creds = get_current_credentials();
```

###Set the credentials

In case you want to swap out who the library is currently using to perform requests, set the current credentials with this function (Note: you should probably get them first and save them somewhere, otherwise you will create a memory leak)

```{.c}
set_current_credentials(creds);
```

###Perform an HTTP/HTTPS Request

Perform an HTTP/HTTPS request using the objcurl library, adding in the required auth headers and, optionally, specify a set of credentials you wish to use for the request.

####Specify credentials
This is what it looks like to specify credentials

~~~~ {.c .numberLines}
#include <wmappauth.h>
...
void do_something(...){
	/**
	 * Grab the credentials from the auth library
	 */
	Credentials* creds = get_current_credentials();

	/**
	 * Perform login as a new user
	 */
	set_current_credentials(NULL); //we've got a reference to the old user's credentials, don't need to keep these around anymore
	terminal_login(); //do login

	/**
	 * Construct a request using objcurl
	 */
	struct curl_request* request = objcurl_new_request();
	request->request_method = HTTP_POST;
	request->url = string_new_with_data("https://wmapp.mccollum.enterprises/shuttle/api/routes", 0);
	//request->body = string_new_with_data("{\"name\":\"My new Shuttle route\"}", 0);
	objcurl_add_request_header(request, string_new_with_data("Content-Type: application/json", 0));
	objcurl_add_request_header(request, string_new_with_data("Accept: application/json", 24)); //the 24 is specifying the length of the string, but can be replaced with 0 to automatically figure it out

	/**
	 * Perform the request, specifying to use the old credentials of whoever was logged in previously (not really sure why you'd want this)
	 */
	struct curl_response* response = perform_credential_request(credentials, request);
	objcurl_free_request(request); //memory should be freed after
	
	/**
	 * Print out the HTTP response code and the message sent by the server
	 */
	printf("Response code: %d\n", response->status_code);
	printf("Response body: %d\n", response->response_body->ptr); //->ptr is added because the body is of type ObjString*; see objcurl.h for a detailed look at the curl_response struct

	objcurl_free_response(response); //free response now that we don't need it anymore
}
~~~~

####Don't specify Credentials
This will use whatever credentials were used for login

``` {.c .numberLines}
#include <wmappauth.h>
...
void do_something(...){
	/**
	 * Perform login (included for sake of clarity because this should have been done at program start or something)
	 */
	terminal_login(); //do login

	/**
	 * Construct a request using objcurl
	 */
	struct curl_request* request = objcurl_new_request();
	request->request_method = HTTP_POST;
	request->url = string_new_with_data("https://wmapp.mccollum.enterprises/shuttle/api/routes", 0);
	//request->body = string_new_with_data("{\"name\":\"My new Shuttle route\"}", 0); //post json for a new shuttle route with the name 'My new Shuttle route'
	objcurl_add_request_header(request, string_new_with_data("Content-Type: application/json", 0)); //tell the server we're using json
	objcurl_add_request_header(request, string_new_with_data("Accept: application/json", 24)); //Tell the server we want json; the 24 is specifying the length of the string, but can be replaced with 0 to automatically figure it out

	/**
	 * Perform the request and the credentials of the signed in user will be added automagically
	 */
	struct curl_response* response = perform_credential_request(credentials, request);
	objcurl_free_request(request); //memory should be freed as soon as it is no longer needed
	
	/**
	 * Print out the HTTP response code and the message sent by the server
	 */
	printf("Response code: %d\n", response->status_code);
	printf("Response body: %d\n", response->response_body->ptr); //->ptr is added because the body is of type ObjString*; see objcurl.h for a detailed look at the curl_response struct

	objcurl_free_response(response); //free response now that we don't need it anymore
}
```

###Get system time in milliseconds

This is used to check against the millisecond precision timestamp on token expiration. This is just provided as extra help for the programmer.

The function signature is `int64_t system_currenttimemillis()` and just returns an approximate number of milliseconds since the epoch time. Can be used for interoperability with java systems

##Compiling

See `Makefile` under the following targets: `sample_static`, `sample_dynamic`

These provide different ways for you to include the library in your project. All of them still require curl to be installed as a dynamic library on your system.
