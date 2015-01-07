#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "util.h"

int main(int argc, char **argv)
{
	int ret = 0;
	char *tty;

	tty = ttyname(0);
	if(!tty && errno == ENOTTY) {
		ret = 1;
		tty = "not a tty";
	}

	if(!tty) { write_err(argv[0], errno, 0); return 2; }

	if(write_fd(1, tty, strlen(tty)) < strlen(tty)) { write_err(argv[0], errno, 0); return 2; }
	if(write_fd(1, "\n", 1) < 1) { write_err(argv[0], errno, 0); return 2; }
	return ret;
}
