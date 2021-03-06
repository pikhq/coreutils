#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "util.h"
#include "noreturn.h"
#include "sethostname.h"

static void noreturn die(char *prog)
{
	write_err(prog, errno, 0);
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
