#define _GNU_SOURCE
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "util.h"

static void die(char *prog)
{
	char *err = strerror(errno);
	write(2, prog, strlen(prog));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
	exit(1);
}

int main(int argc, char **argv)
{
	char name[HOST_NAME_MAX];
	int c = argc - 1;
	char **v = argv + 1;

	if(c && v[0][0] == '-' && v[0][1] == '-' && !v[0][2]) { c--; v++; }

	if(!c) {
		size_t name_len = strnlen(name, HOST_NAME_MAX);
		if(gethostname(name, HOST_NAME_MAX) < 0) die(argv[0]);
		if(write_fd(1, name, name_len) < name_len) die(argv[0]);
		if(write_fd(1, "\n", 1) < 1) die(argv[0]);
	} else {
		if(sethostname(v[0], strlen(v[0])) < 0) die(argv[0]);
	}
}
