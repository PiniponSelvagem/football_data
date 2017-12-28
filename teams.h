#ifndef TEAMS_H
#define TEAMS_H

typedef struct team {
	char *name;
	char *code;
	char *shortName;
} Team;

typedef struct team_node {
	int competition_id;
	size_t size;
	Team *teams;
	struct team_node *next;
} TeamNode;

Team *get_teams(int competition_id, size_t *size);
TeamNode *save_teams_tolist(TeamNode *head, int id);
int cmp_teams(char *team_name, char *other_team_name);
void free_teams(Team *teams, size_t size);
void free_teams_node(TeamNode *head);

#endif
