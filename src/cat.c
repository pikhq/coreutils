#define _XOPEN_SOURCE 700
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void my_perror(char *prog, char *msg)
{
	char *err = strerror(errno);
	write(2, prog, strlen(prog));
	write(2, ": ", 2);
	write(2, msg, strlen(msg));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
}

int main(int argc, char **argv)
{
	char buf[1024];
	size_t len;
	int ret = 0, n = argc - 1;
	char **v = argv+1;

	setlocale(LC_CTYPE, "");
#ifdef LC_MESSAGES
	setlocale(LC_MESSAGES, "");
#endif

	for(; n && v[0][0] == '-' && v[0][1]; v++, n--) {
		if(v[0][1] == '-' && !v[0][2]) { // --
			v++; n--;
			break;
		}
		if(v[0][1] != 'u' || v[0][2]) {
			errno = EINVAL;
			my_perror(argv[0], v[0]);
			return 1;
		}
	}
	if(!n) {
		n = 1;
		v = (char*[]){"-"};
	}
	for(; n; v++, n--) {
		int fd = v[0][0] == '-' && !v[0][1]
			? 0
			: open(v[0], O_RDONLY);
		if(fd < 0) {
			my_perror(argv[0], v[0]);
			ret = 1;
			continue;
		}
		while((len = read(fd, buf, sizeof buf)) > 0) {
			if(write_fd(fd, buf, len) < len) {
				my_perror(argv[0], "write error");
				return 1;
			}
		}
		if(len) {
			my_perror(argv[0], v[0]);
			ret = 1;
		}
		if(fd) close(fd);
	}
	if(close(1) < 0) {
		my_perror(argv[0], "write error");
		return 1;
	}
	return ret;
}
