#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <errno.h>
#include <signal.h>

static int my_write(int fd, const char *str, size_t len)
{
	while(len) {
		ssize_t n = write(fd, str, len);
		if(n < 0 && errno == EINTR) continue;
		if(n < 0) return n;
		str += n; len -= n;
	}
	return 0;
}

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
	char buf[4 * 1024];
	size_t len;
	int ret = 0, n, i;
	char **v;
	int optc;

	int mode = O_WRONLY | O_CREAT | O_TRUNC;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	while((optc = getopt(argc, argv, "ai")) != -1) {
		switch(optc) {
		case 'a':
			mode = O_WRONLY | O_CREAT | O_APPEND;
			break;
		case 'i':
			signal(SIGINT, SIG_IGN);
			break;
		default:
			return 1;
		}
	}

	n = argc - optind;
	v = argv + optind;

	int fds[n+1];

	for(i = 0; i < n; i++) {
		fds[i] = open(v[i], mode, 00666);
		if(fds[i] < 0) {
			my_perror(argv[0], v[i]);
			ret = 1;
		}
	}
	fds[n] = 1;

	while((len = read(0, buf, sizeof buf)) > 0) {
		for(i = 0; i < n + 1; i++) {
			if(fds[i] < 0) continue;
			if(my_write(fds[i], buf, len) < 0) {
				fds[i] = -1;
				my_perror(argv[0], "write error");
				ret = 1;
			}
		}
	}
	if(len) {
		my_perror(argv[0], "read error");
		ret = 1;
	}
	for(i = 0; i < n + 1; i++) {
		if(fds[i] < 0) continue;
		if(close(fds[i]) < 0) {
			my_perror(argv[0], "error closing file");
			ret = 1;
		}
	}

	return ret;
}
