#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "common.h"
#include "getdata.h"

static char *my_itoa(int num, char *str)
{
	if(str == NULL) {
		return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

char *json_string_to_ptr(json_t *jcomp, char *field) {
	const char *src = json_string_value(json_object_get(jcomp, field));
	if (src == NULL)
		return NULL;
	char *dest = malloc(strlen(src)+1);
	strcpy(dest, src);
	return dest;
}

json_t *get_jdata_from(const char *u, const char *u_prefix, int input_value, const char *u_suffix) {
	char url[64];
	char s[2];
	if (input_value < 0) {
		strcpy(url, u);
	}
	else {
		my_itoa(input_value, s);
		snprintf(url,sizeof(url) ,"%s%s%s%s",u, u_prefix, s, u_suffix);
	}
	//printf("%s\n", url);
	return http_get_json_data(url);
}

int find_jdata_size(json_t *jroot, json_t *jdata) { //find jroot array size
	int count = -1;
	do {
		jdata = json_array_get(jroot, ++count);
	} while (jdata!=NULL);
	return count;
}
