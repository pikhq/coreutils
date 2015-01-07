#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "util.h"

extern char **environ;

int main(int argc, char **argv)
{
	int i;
	size_t n;

	for(i = 1; i < argc && argv[i][0] == '-' && argv[i][1]; i++) {
		if(argv[i][1] == '-' && !argv[i][2]) break;
		if(argv[i][1] == 'i' && !argv[i][2]) {
			environ = (char*[]){0};
		}
		if(argv[i][1] != 'i' || argv[i][2]) {
			write_err(argv[0], EINVAL, argv[i]);
			return 1;
		}
	}

	for(; i < argc && strchr(argv[i], '='); i++) {
		if(putenv(argv[i]) < 0) {
			write_err(argv[0], errno, 0);
			return 1;
		}
	}

	if(i < argc) {
		execvp(argv[i], &argv[i]);
		write_err(argv[0], errno, 0);
		return errno != ENOENT ? 126 : 127;
	}
	for(n = 0; environ && environ[n]; n++) {
		if(write_fd(1, environ[n], strlen(environ[n]))
		      < strlen(environ[n])
		   || write_fd(1, "\n", 1) < 1) {
			write_err(argv[0], errno, 0);
			return 1;
		}
	}
}
