#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <unistd.h>

#include "util.h"

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
			write_err(argv[0], EINVAL, v[0]);
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
			write_err(argv[0], errno, v[0]);
			ret = 1;
			continue;
		}
		while((len = read(fd, buf, sizeof buf)) > 0) {
			if(write_fd(1, buf, len) < len) {
				write_err(argv[0], errno, "write error");
				return 1;
			}
		}
		if(len) {
			write_err(argv[0], errno, v[0]);
			ret = 1;
		}
		if(fd) close(fd);
	}
	if(close(1) < 0) {
		write_err(argv[0], errno, "write error");
		return 1;
	}
	return ret;
}
