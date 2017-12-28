#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "teams.h"
#include "getdata.h"
#include "common.h"

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

TeamNode *save_teams_tolist(TeamNode *head, int id) {
	struct team_node *node;
	node = (struct team_node*)malloc(sizeof(struct team_node));
	if (node == NULL) {
		printf("WARN: Unable to allocate memory\n");
	}
	else {
		node->competition_id = id;
		node->teams = get_teams(node->competition_id, &node->size);
		node->next = head;
		head = node;
	}
	return head;
}

int cmp_teams(char *team_name, char *other_team_name) {
	if (strcmp(team_name, other_team_name)==0) {
		return 1;
	}
	return 0;
}

void free_teams(Team *teams, size_t size) {
	for (int i=0; i<size; ++i) {
		free(teams[i].name);
		free(teams[i].code);
		free(teams[i].shortName);
	}
	free(teams);
}

void free_teams_node(TeamNode *head) {
	TeamNode *curr = head;
	while (curr!=NULL) {
		free_teams(curr->teams, curr->size);
		head = curr;
		curr = curr->next;
		free(head);
	}
}
