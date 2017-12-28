#ifndef COMPETITIONS_H
#define COMPETITIONS_H

typedef struct competition {
	int  id;
	char *caption;
	char *league;
	char *year;
	int  numberOfTeams;
} Competition;

Competition *get_competitions(int season, size_t *size);
void free_competitions(Competition *competitions, size_t size);

#endif
