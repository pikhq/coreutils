#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "util.h"

int main(int argc, char **argv)
{
	int n = argc - 1;
	char **v = argv + 1;
	char *endptr, *err;
	struct timespec timespec = {0};

	if(n && v[0][0] == '-' && v[0][1] == '-' && !v[0][2]) {
		n--; v++;
	}

	if(!n) {
		write_err(argv[0], 0, "Missing operand");
		return 1;
	}
	errno = 0;
	uintmax_t len = strtoumax(v[0], &endptr, 10);
	if(v[0][0] && *endptr) errno = EINVAL;
	timespec.tv_sec = len;
	if(len != timespec.tv_sec) errno = ERANGE;
	if(errno) goto err;
	do {
		errno = 0;
		nanosleep((struct timespec[1]){timespec}, &timespec);
	} while(errno == EINTR);
	if(errno) goto err;

	return 0;
err:
	write_err(argv[0], errno, 0);
	return 1;
}
