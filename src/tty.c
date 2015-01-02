#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static int my_write(int fd, char *s, size_t l)
{
	while(l) {
		ssize_t n;
		errno = 0;
		n = write(fd, s, l);
		if(n < 0 && errno == EINTR) continue;
		if(n < 0) return -1;
		s += n;
		l -= n;
	}
	return 0;
}

static void errorout(char *prog)
{
	char *err = strerror(errno);
	my_write(2, prog, strlen(prog));
	my_write(2, ": ", 2);
	my_write(2, err, strlen(err));
	my_write(2, "\n", 1);
	exit(2);
}

int main(int argc, char **argv)
{
	int ret = 0;
	char *tty;

	tty = ttyname(0);
	if(!tty && errno == ENOTTY) {
		ret = 1;
		tty = "not a tty";
	}

	if(!tty) errorout(argv[0]);

	if(my_write(1, tty, strlen(tty)) < 0) errorout(argv[0]);
	if(my_write(1, "\n", 1) < 0) errorout(argv[0]);
	return ret;
}
