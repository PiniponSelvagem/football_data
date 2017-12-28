#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "fixtures.h"
#include "getdata.h"
#include "common.h"
#include "teams.h"

Fixture *get_fixtures(int competition_id, size_t *size) {
	json_t *jroot, *jfixt;
	jroot = get_jdata_from("http://football-data.org/v1/competitions/", "", competition_id, "/fixtures");
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
	

Fixture *get_team_fixtures(char *team_name, FixtureNode *f_orig, size_t size) {
	Fixture *ret = malloc(size * sizeof(Fixture));
	for (int i=0, n=0; i<size; ++i) {
		if (cmp_teams(team_name, f_orig->fixtures[i].homeTeamName) || cmp_teams(team_name, f_orig->fixtures[i].awayTeamName)) {
			ret[n] = f_orig->fixtures[i];
			++n;
		}
	}
	return ret;
}

size_t count_team_fixtures(FixtureNode *f_orig, char *team_name) {
	size_t count = 0;
	for (int i=0; i<f_orig->size; ++i) {
		count = count + cmp_teams(team_name, f_orig->fixtures[i].homeTeamName) + cmp_teams(team_name, f_orig->fixtures[i].awayTeamName);
	}
	return count;
}

FixtureNode *save_fixtures_tolist(FixtureNode *head, int id) {
	struct fixture_node *node;
	node = (struct fixture_node*)malloc(sizeof(struct fixture_node));
	if (node == NULL) {
		printf("WARN: Unable to allocate memory\n");
	}
	else {
		node->competition_id = id;
		node->fixtures = get_fixtures(node->competition_id, &node->size);
		node->next = head;
		head = node;
	}
	return head;
}

FixtureNode *save_fixtures_to_templist(FixtureNode *head, int id, Fixture *fixtures, size_t size) {
	struct fixture_node *node;
	node = (struct fixture_node*)malloc(sizeof(struct fixture_node));
	if (node == NULL) {
		printf("WARN: Unable to allocate memory\n");
	}
	else {
		node->competition_id = id;
		node->size = size;
		node->fixtures = fixtures;
		node->next = head;
		head = node;
	}
	return head;
}

void free_fixtures(Fixture *fixtures, size_t size) {
	for (int i=0; i<size; ++i) {
		free(fixtures[i].date);
		free(fixtures[i].status);
		free(fixtures[i].homeTeamName);
		free(fixtures[i].awayTeamName);
		free(&fixtures[i].resultHomeTeam);
		free(&fixtures[i].resultAwayTeam);
		free(&fixtures[i]);	
	}
	free(fixtures);
}
