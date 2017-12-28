#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "libfootball.h"

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



/**
 *  The buffer type to hold the HTTP GET response body.
 */
typedef struct _Buffer {
    size_t size;
    char *memory;
} Buffer;

/**
 * A write_function that copies data to a user buffer.
 */
static size_t write_to_memory(char *data, size_t size, size_t nmemb, void *context)
{
    size_t byteCount = size * nmemb;
    Buffer *bufptr = (Buffer *)context;

    /* grow the buffer in order to hold more byteCount bytes */
    bufptr->memory = realloc(bufptr->memory, bufptr->size + byteCount + 1);
    if (bufptr->memory == NULL) {
        /* out of memory! */
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

static char *my_itoa(int num, char *str)
{
	if(str == NULL) {
		return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

json_t *http_get_json_data(const char *url) {
	Buffer buffer = { .size = 0, .memory = NULL };
	
	CURL *curl = curl_easy_init();							//initialize CURL
	if (!curl) {
		curl_global_cleanup();								//cleanup before returning
		return NULL;
	}
	
	curl_easy_setopt(curl, CURLOPT_PROXY, "http://211.127.160.240:8080"); //JP proxy
	
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


static char *json_string_to_ptr(json_t *jcomp, char *field) {
	const char *src = json_string_value(json_object_get(jcomp, field));
	if (src == NULL)
		return NULL;
	char *dest = malloc(strlen(src)+1);
	strcpy(dest, src);
	return dest;
}

static json_t *get_jdata_from(const char *u, const char *u_prefix, int input_value, const char *u_suffix) {
	char url[64];
	char s[2];
	if (input_value < 0) {
		strcpy(url, u);
	}
	else {
		my_itoa(input_value, s);
		snprintf(url,sizeof(url) ,"%s%s%s%s",u, u_prefix, s, u_suffix);
	}
	printf("%s\n", url);
	return http_get_json_data(url);
}

static int find_jdata_size(json_t *jroot, json_t *jdata) {
	int count = -1;
	do {
		jdata = json_array_get(jroot, ++count);
	} while (jdata!=NULL);
	return count;
}

Competition *get_competitions(int season, size_t *size) {
	json_t *jroot, *jcomp;
	jroot = get_jdata_from("http://football-data.org/v1/competitions/", "?season=", season, "");
	jcomp = jroot;
	
	*size = find_jdata_size(jroot, jcomp);
	
	Competition *competitions = malloc(*size * sizeof(Competition));
	
	for (int i=0; i<*size; ++i) {
		jcomp = json_array_get(jroot, i);
		competitions[i].id = json_integer_value(json_object_get(jcomp, "id"));
		competitions[i].caption = json_string_to_ptr(jcomp, "caption");
		competitions[i].league  = json_string_to_ptr(jcomp, "league");
		competitions[i].year    = json_string_to_ptr(jcomp, "year");
		competitions[i].numberOfTeams = json_integer_value(json_object_get(jcomp, "numberOfTeams"));
		//printf("%d\n", competitions[i].id);
	}
		
	json_decref(jroot);
	json_decref(jcomp);
	
	return competitions;
}

void free_competitions(Competition *competitions, size_t size) {
	for (int i=0; i<size; ++i) {
		free(competitions[i].caption);
		free(competitions[i].league);
		free(competitions[i].year);
	}
	free(competitions);
}

Team *get_teams(int competition_id, size_t *size) {
	json_t *jroot, *jteams;
	jroot = get_jdata_from("http://football-data.org/v1/competitions/", "", competition_id, "/teams");
	jroot = json_object_get(jroot, "teams");
	jteams = jroot;
	
	*size = find_jdata_size(jroot, jteams);
	
	Team *teams = malloc(*size * sizeof(Team));
	
	for (int i=0; i<*size; ++i) {
		jteams = json_array_get(jroot, i);		
		teams[i].name      = json_string_to_ptr(jteams, "name");
		teams[i].code      = json_string_to_ptr(jteams, "code");
		teams[i].shortName = json_string_to_ptr(jteams, "shortName");
	}
		
	json_decref(jroot);
	json_decref(jteams);
	
	return teams;
}

void free_teams(Team *teams, size_t size) {
	for (int i=0; i<size; ++i) {
		free(teams[i].name);
		free(teams[i].code);
		free(teams[i].shortName);
	}
	free(teams);
}


Fixture *get_fixtures(int competition_id, size_t *size) {
	json_t *jroot, *jfixt;
	jroot = get_jdata_from("http://football-data.org/v1/competitions/", "", competition_id, "/fistures");
	jroot = json_object_get(jroot, "fixtures");
	jfixt = jroot;
	
	*size = find_jdata_size(jroot, jfixt);
	
	Fixture *fixtures = malloc(*size * sizeof(Fixture));
	
	for (int i=0; i<*size; ++i) {
		jfixt = json_array_get(jroot, i);
		fixtures[i].date           = json_string_to_ptr(jfixt, "date");
		fixtures[i].status         = json_string_to_ptr(jfixt, "status");
		fixtures[i].homeTeamName   = json_string_to_ptr(jfixt, "homeTeamName");
		fixtures[i].awayTeamName   = json_string_to_ptr(jfixt, "awayTeamName");
		fixtures[i].resultHomeTeam = json_integer_value(json_object_get(json_object_get(jfixt, "result"), "goalsHomeTeam"));
		fixtures[i].resultAwayTeam = json_integer_value(json_object_get(json_object_get(jfixt, "result"), "goalsAwayTeam"));
	}
		
	json_decref(jroot);
	json_decref(jfixt);
	
	return fixtures;
}

Fixture *get_team_fixtures(int team_id, size_t *size) {
	
	return NULL;
}

void free_fixtures(Fixture *fixtures, size_t size) {
	for (int i=0; i<size; ++i) {
		free(fixtures[i].date);
		free(fixtures[i].status);
		free(fixtures[i].homeTeamName);
		free(fixtures[i].awayTeamName);
	}
	free(fixtures);
}
