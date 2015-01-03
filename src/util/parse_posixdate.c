#include <time.h>

#include "util.h"

struct fmts {
	const char * const fmt;
	const char min_len;
};

static const struct fmts fmts[] = {
	{ "%m%d%H%M%Y.%S", 15},
	{ "%m%d%H%M%y.%S", 13 },
	{ "%m%d%H%M.%S", 11 },
	{ "%m%d%H%M%Y", 12 },
	{ "%m%d%H%M%y", 10 },
	{ "%m%d%H%M", 8 },
	{}
};

int parse_posixdate(char *str, struct tm *tm)
{
	char *end;

	const struct fmts *fmt;

	time_t time_val = time(0);

	for(fmt = fmts; fmt->fmt; fmt++) {
		if(!localtime_r(&time_val, tm)) return 1;
		end = strptime(str, fmt->fmt, tm);
		if(end && !*end && end - str >= fmt->min_len) return 0;
	}
	return 1;
}
