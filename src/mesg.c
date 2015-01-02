#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>

#include "util.h"

int main(int argc, char **argv)
{
	int ret = 0;
	char tty[TTY_NAME_MAX];
	struct stat st;
	int i;
	char *err;

	int n = argc-1;
	char **v = argv+1;

	for(; n; n--, v++) {
		if(v[0][0] == '-' && v[0][1] == '-' && !v[0][2]) { n--; v++; break; }
		if(v[0][0] == '-') { errno = EINVAL; goto do_error; }
		break;
	}

	for(i = 0; i < 3; i++) {
		if(ttyname_r(i, tty, sizeof tty) == 0) break;
	}

	if(i == 3) goto do_error;

	if(stat(tty, &st) < 0) goto do_error;

	if(!n) {
		if(write_fd(1, (char[]){'i', 's', ' ', st.st_mode & 0020 ? 'y' : 'n', '\n'}, 5) < 5)
			goto do_error;
	} else {
		mode_t mode = v[0][0] == 'n' ? st.st_mode & ~(mode_t)0020 : st.st_mode | 0020;
		if(chmod(tty, mode) < 0) goto do_error;
	}

	return 0;

do_error:
	err = strerror(errno);
	write(2, argv[0], strlen(argv[0]));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
	return 1;

}
