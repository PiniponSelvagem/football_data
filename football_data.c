#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <jansson.h>
#include "competitions.h"
#include "teams.h"
#include "fixtures.h"

static void print_competitions(Competition *comp, size_t size) {
	printf("\nCompetitions:\n");
	printf("  ID | year | nT | lea |        caption\n");
	printf("-----+------+----+-----+------------------------\n");
	for (int i=0; i<size; ++i) {
		printf("%4d | %s | %d | %3s | %s\n", comp[i].id, comp[i].year, comp[i].numberOfTeams, comp[i].league, comp[i].caption);
	}
	printf("\n");
}

static void print_teams(TeamNode *tn, int id) {
	printf("\nTeams for competition: %d\n", tn->competition_id);
	printf("     ID     |     shortName     |        name\n");
	printf("------------+-------------------+--------------------\n");
	for (int i=0; i<tn->size; ++i) {
		printf("%11s | %17s | %s\n", tn->teams[i].code, tn->teams[i].shortName, tn->teams[i].name);
	}
	printf("\n");
}

static void print_fixtures(FixtureNode *fn, int id) {
	printf("\nFixtures for competition: %d\n", fn->competition_id);
	printf("   STATUS  |           HomeTeam         | HR - AR |           AwayTeam         |         Date\n");
	printf("-----------+----------------------------+---------+----------------------------+----------------------\n");
	for (size_t i=0; i<fn->size; ++i) {
		printf("%10s | %26s | %2d -%2d  | %26s | %s\n", fn->fixtures[i].status, fn->fixtures[i].homeTeamName, fn->fixtures[i].resultHomeTeam, fn->fixtures[i].resultAwayTeam, fn->fixtures[i].awayTeamName, fn->fixtures[i].date);
	}
	printf("\n");
}

static void print_fixtures_not_found(char *team_code) {
	printf("\nNo fixtures nor team ID found for team %s! Try running command 't' and then command 'x'.\n", team_code);
}

static void print_help() {
	printf("\nUnrecognized command!\n");
	printf("Available commands:\n");
	printf(" List competitions\n > c\n");
	printf(" List teams from a competition\n > t {competition_id}\n");
	printf(" List games from a competition\n > x {competition_id}\n");
	printf(" List games of a team from all the competitons\n > j {team_id}\n\n");
}

static int get_int(char *input) {
	if (!isspace(input[1]) || isspace(input[2]) || input[2]==0)
		return -1;	//return if command not valid
	return atoi(input+2);	//do atoi after 1st "space"
}

static char *get_team_code(char *input) {
	if (!isspace(input[1])) return NULL;	//return if command not valid
	char *code = (char*)malloc(16);
	strcpy(code, input+2);
	int i=0;
	while(!isspace(code[i])) {
		++i;
	}
	code[i]=0;
	//printf("%s\n", code);
	return code;
}

static int check_teams_exists(TeamNode *head, int id, int print_found) {
	TeamNode *curr = head;
	while (curr!=NULL) {
		if (curr->competition_id == id && print_found) {
			print_teams(curr, curr->competition_id);
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

static int check_fixtures_exists(FixtureNode *head, int id, int print_found) {
	FixtureNode *curr = head;
	while (curr!=NULL) {
		if (curr->competition_id == id && print_found) {
			print_fixtures(curr, curr->competition_id);
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

static char *search_teamname_fixtures(TeamNode *teams_head, char *team_code, int *found) {
	char *team_name;
	if(team_code!=NULL) {
		TeamNode *temp_teams = teams_head;
		while (temp_teams!=NULL) {
			for (int i=0; i<temp_teams->size; ++i) {
				if (teams_head->teams[i].code!=NULL) {
					if (cmp_teams(team_code, temp_teams->teams[i].code)) {
						team_name = temp_teams->teams[i].name;
						*found = 1;
						break;
					}
				}
			}
			if (found) break;
			temp_teams = temp_teams->next;
		}
	}
	return team_name;
}


int main(int nargs, char *args[]) {
	int season = -1;
	if (args[1] != NULL) {
		season = atoi(args[1]);
	}
	char input[128];
	char *q = "q\n";
	char *c = "c";
	char *t = "t";
	char *x = "x";
	char *j = "j";
	
	size_t size_competitions = 0;
	Competition *competitions = NULL;
	
	TeamNode *teams_head = NULL;
	FixtureNode *fixtures_head = NULL;
	
	while (strcmp(q, input)) {
		printf("> ");
		fgets(input, 128, stdin);
		
		if (input[0] == c[0]) {	//Competitions
			if (competitions == NULL) {
				competitions = get_competitions(season, &size_competitions);
			}
			print_competitions(competitions, size_competitions);
		}
		else if (input[0] == t[0]) {  //Teams
			int id = get_int(input);
			int exits = check_teams_exists(teams_head, id, 1);
			if (exits==0 && id>=0) { //3nd param, print if found
				teams_head = save_teams_tolist(teams_head, id);
				print_teams(teams_head, teams_head->competition_id);	//print last teams data, 1st position in teams list
			}
			else {
				if (exits==0) print_help();
			}
		}
		else if (input[0] == x[0]) {  //Games
			int id = get_int(input);
			int exits = check_fixtures_exists(fixtures_head, id, 1);
			if (exits==0 && id>=0) { //3nd param, print if found
				fixtures_head = save_fixtures_tolist(fixtures_head, id);
				print_fixtures(fixtures_head, fixtures_head->competition_id);	//print last fixtures data, 1st position in fixtures list
			}
			else {
				if (exits==0) print_help();
			}
		}
		else if (input[0] == j[0]) {  //Team Games
			char *team_code = get_team_code(input);
			int found = 0;
			char *team_name = search_teamname_fixtures(teams_head, team_code, &found);
			if (found) {
				FixtureNode *f_orig = fixtures_head;
				while (f_orig != NULL) {
					size_t count = count_team_fixtures(f_orig, team_name);
					FixtureNode *temp_fixtures_head = NULL;
					Fixture *temp = get_team_fixtures(team_name, f_orig, f_orig->size);
					temp_fixtures_head = save_fixtures_to_templist(temp_fixtures_head, f_orig->competition_id, temp, count);				
					print_fixtures(temp_fixtures_head, temp_fixtures_head->competition_id);
					free(temp);
					f_orig = f_orig->next;
				}
			}
			else {
				print_fixtures_not_found(team_code);
			}
			free(team_code);
		}
		else if (input[0] != q[0]){
			print_help();
		}
		
		//printf("INPUT: %s\n", input);
	}
	
	printf("Closing...\n");
	if (competitions != NULL)
		free_competitions(competitions, size_competitions);
	if (teams_head != NULL)
		free_teams_node(teams_head);
	
	return 0;
}
