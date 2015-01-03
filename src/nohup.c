#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>

#include "util.h"

#if !defined(PATH_MAX)
#define PATH_MAX _POSIX_PATH_MAX
#endif

int main(int argc, char **argv)
{
	int fd;

	setlocale(LC_CTYPE, "");
#ifdef LC_MESSAGES
	setlocale(LC_MESSAGES, "");
#endif

	signal(SIGHUP, SIG_IGN);

	if(isatty(0)) {
		fd = open("/dev/null", O_RDONLY|O_CLOEXEC);
		if(fd < 0) { return 127; }
		dup2(fd, 0);
	}
	if(isatty(1) || isatty(2) || fcntl(1, F_GETFD) == -1 || errno == EBADF) {
		char tmp[PATH_MAX] = "nohup.out";
		mode_t umask_val = umask(~(S_IRUSR|S_IWUSR));
		fd = open(tmp, O_CREAT|O_APPEND|O_WRONLY|O_CLOEXEC, S_IRUSR|S_IWUSR);
		if(fd < 0) {
			char *home = getenv("HOME");
			if(!home) return 127;
			if(strlen(home) + strlen("/nohup.out") + 1 > PATH_MAX) return 127;
			strcpy(tmp, home);
			strcat(tmp, "/nohup.out");
			fd = open(tmp, O_CREAT|O_APPEND|O_WRONLY|O_CLOEXEC, S_IRUSR|S_IWUSR);
			if(fd < 0) return 127;
		}
		umask(umask_val);
		write_fd(2, tmp, strlen(tmp));
		write_fd(2, "\n", 1);
	}
	if(isatty(1))
		if(dup2(fd, 1) < 0) return 127;
	if(isatty(2)) {
		if(fcntl(1, F_GETFD) != -1 && errno != EBADF)
			fd = 1;
		if(dup2(fd, 2) < 0) return 127;
	}

	execvp(argv[1], &argv[1]);
	return errno != ENOENT ? 126 : 127;
}
