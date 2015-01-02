#include <unistd.h>
#include "util.h"

size_t write_fd(int fd, const char *s, size_t len)
{
	size_t ret = 0;
	while(len) {
		ssize_t n = write(fd, s, len);
		if(n < 0) break;
		ret += n; s += n; len -= n;
	}
	return ret;
}
