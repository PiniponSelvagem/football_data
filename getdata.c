#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "getdata.h"

int http_get(const char *url, const char *filename) {
	CURL *curl = curl_easy_init();							//initialize CURL
	if (!curl) return -1;									//if not init, exit

	FILE *file = fopen(filename, "w");						//create file, write enable
	if (!file) return -1;									//if failed to create file exit

	curl_easy_setopt(curl, CURLOPT_URL, url);				//set URL to get info from
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);		//write date to file
	curl_easy_perform(curl);								//get data from URL

	long status;												//HTTP status code
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);	//get status code to &status

	curl_easy_cleanup(curl);								//close CURL
	fclose(file);											//close FILE

	return (200 == status) ? 0 : -1;						//if "200 OK", we got good data
}


typedef struct _Buffer {
    size_t size;
    char *memory;
} Buffer;

static size_t write_to_memory(char *data, size_t size, size_t nmemb, void *context)
{
    size_t byteCount = size * nmemb;
    Buffer *bufptr = (Buffer *)context;

    /* grow the buffer in order to hold more byteCount bytes */
    bufptr->memory = realloc(bufptr->memory, bufptr->size + byteCount + 1);
    if (bufptr->memory == NULL) {
        fprintf(stderr, "***error: not enough memory\n");
        return 0;
    }
	/* copy the new data block */
    memcpy(bufptr->memory + bufptr->size, data, byteCount);

    /* update size and close the C string */
    bufptr->size += byteCount;
    bufptr->memory[bufptr->size] = '\0';
    return byteCount;
}


json_t *http_get_json_data(const char *url) {
	Buffer buffer = { .size = 0, .memory = NULL };
	
	CURL *curl = curl_easy_init();							//initialize CURL
	if (!curl) {
		curl_global_cleanup();								//cleanup before returning
		return NULL;
	}
	
	//curl_easy_setopt(curl, CURLOPT_PROXY, "http://45.76.106.62:8080");   //Proxy: Japan (Heiwajima)
	
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);		//follow HTTP 3xx redirects
	curl_easy_setopt(curl, CURLOPT_URL, url);				//set URL to get info from
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_memory);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_perform(curl);								//get data from URL

	long status;												//HTTP status code
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);	//get status code to &status

	curl_easy_cleanup(curl);								//close CURL

	json_error_t error;
	return json_loads(buffer.memory, 0, &error);			//load json from JSON text
}
