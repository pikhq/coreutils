#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	int c;
	long adj = 10;
	char *endptr;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	while((c = getopt(argc, argv, "n:")) != -1) {
		if(c == 'n') {
			errno = 0;
			adj = strtol(optarg, &endptr, 10);
			if(!errno && *endptr) errno = EINVAL;
			if(!errno && (adj > INT_MAX || adj < INT_MIN)) errno = ERANGE;
			if(errno) { write_err(argv[0], errno, 0); return 1; }
		} else {
			return 1;
		}
	}

	errno = 0;
	nice(adj);
	if(errno) write_err(argv[0], errno, 0);
	if(errno && errno != EPERM) return 1;
	execvp(argv[optind], &argv[optind]);
	write_err(argv[0], errno, 0);
	return errno != ENOENT ? 126 : 127;
}
