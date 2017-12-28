#ifndef LIBFOOTBALL_H
#define LIBFOOTBALL_H

int http_get(const char *url, const char *filename);
json_t *http_get_json_data(const char *url);

typedef struct competition {
	int  id;
	char *caption;
	char *league;
	char *year;
	int  numberOfTeams;
} Competition;

typedef struct team {
	char *name;
	char *code;
	char *shortName;
} Team;

typedef struct fixture{
	char *date;
	char *status;
	char *homeTeamName;
	char *awayTeamName;
	int  resultHomeTeam;
	int  resultAwayTeam;
} Fixture;

Competition *get_competitions(int season, size_t *size);
void free_competitions(Competition *competitions, size_t size);

Team *get_teams(int competition_id, size_t *size);
void free_teams(Team *teams, size_t size);

Fixture *get_fixtures(int competition_id, size_t *size);
Fixture *get_team_fixtures(int team_id, size_t *size);
void free_fixtures(Fixture *fixtures, size_t size);

#endif
