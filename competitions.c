#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "competitions.h"
#include "getdata.h"
#include "common.h"

Competition *get_competitions(int season, size_t *size) {
	json_t *jroot, *jcomp;
	jroot = get_jdata_from("http://football-data.org/v1/competitions/", "?season=", season, "");
	
	//http_get("http://football-data.org/v1/competitions/", "test.txt");
		
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
