#include <stdio.h>
#include <jansson.h>
#include "libfootball.h"

int main() {
	//int value = http_get("http://imagem.band.com.br/f_198156.jpg", "File.txt");
	
	/*
	json_t *jroot;
	json_t *item, *items, *id;
	const char *url = "http://football-data.org/v1/competitions/";
	
	jroot = http_get_json_data(url);
	int i = 0;
	items = json_array_get(jroot, i);
	do {
		id = json_object_get(items, "league");
		printf("%s\n", json_string_value(id));
		items = json_array_get(jroot, ++i);
	} while (items!=NULL);
	
	json_decref(jroot);
	*/
	
	/*
	size_t size = 0;
	Competition *competitions;
	competitions = get_competitions(2017, &size);
	printf("%d\n", size);
	printf("%d\n", competitions[0].id);
	printf("%s\n", competitions[0].caption);
	free_competitions(competitions, size);
	*/
	
	/*
	size_t size2 = 0;
	Team *teams;
	teams = get_teams(445, &size2);
	printf("%d\n", size2);
	printf("%s\n", teams[0].name);
	printf("%s\n", teams[0].code);
	printf("%s\n", teams[0].shortName);
	free_teams(teams, size2);
	*/
	
	size_t size3 = 0;
	Fixture *fixtures;
	fixtures = get_fixtures(445, &size3);
	printf("%d\n", size3);
	printf("%s\n", fixtures[369].date);
	printf("%s\n", fixtures[369].status);
	printf("%s\n", fixtures[369].homeTeamName);
	printf("%s\n", fixtures[369].awayTeamName);
	printf("%d\n", fixtures[369].resultHomeTeam);
	printf("%d\n", fixtures[369].resultAwayTeam);
	free_fixtures(fixtures, size3);
}
