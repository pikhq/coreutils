#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

static void my_perror(char *prog)
{
	char *err = strerror(errno);
	write(2, prog, strlen(prog));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
}

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
			if(errno) { my_perror(argv[0]); return 1; }
		} else {
			return 1;
		}
	}

	errno = 0;
	nice(adj);
	if(errno) my_perror(argv[0]);
	if(errno && errno != EPERM) return 1;
	execvp(argv[optind], &argv[optind]);
	my_perror(argv[0]);
	return errno != ENOENT ? 126 : 127;
}
