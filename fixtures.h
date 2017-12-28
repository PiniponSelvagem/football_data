#ifndef FIXTURES_H
#define FIXTURES_H

typedef struct fixture{
	char *date;
	char *status;
	char *homeTeamName;
	char *awayTeamName;
	int  resultHomeTeam;
	int  resultAwayTeam;
} Fixture;

typedef struct fixture_node {
	int competition_id;
	size_t size;
	Fixture *fixtures;
	struct fixture_node *next;
} FixtureNode;

Fixture *get_fixtures(int competition_id, size_t *size);
Fixture *get_team_fixtures(char *team_name, FixtureNode *f_orig, size_t size);
size_t count_team_fixtures(FixtureNode *f_orig, char *team_name);
FixtureNode *save_fixtures_tolist(FixtureNode *head, int id);
FixtureNode *save_fixtures_to_templist(FixtureNode *head, int id, Fixture *fixtures, size_t size);
void free_fixtures(Fixture *fixtures, size_t size);

#endif
