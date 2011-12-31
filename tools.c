#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tools.h"

const char *get_date(void)
{
	char *cur_date;
	time_t now;
	struct tm *t;

	time(&now);
	t = localtime(&now);
	cur_date = (char *) malloc(sizeof(char)*21);

	snprintf(cur_date, 20, "%d/%02d/%02d %02d:%02d:%02d", (t->tm_year+1900), t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return cur_date;
}

