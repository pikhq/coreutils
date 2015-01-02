#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/klog.h>
#include <limits.h>

static void my_perror(char *prog)
{
	char *err = strerror(errno);
	write(2, prog, strlen(prog));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
}

int main(int argc, char **argv)
{
	int optc;
	int len = 16*1024;
	int level = 0;
	int clear = 0;
	char *buf;
	char last = '\n';

	while((optc = getopt(argc, argv, "n:s:c")) != -1) {
		switch(optc) {
		case 'n':
			if(isdigit(optarg[0]) && optarg[0] != '0' && !optarg[1]) {
				level = '0' - optarg[0];
			} else {
				errno = EINVAL;
				my_perror(argv[0]);
				return 1;
			}
			break;
		case 's':
			{
				long tmp;
				char *endptr;

				errno = 0;
				tmp = strtol(optarg, &endptr, 0);
				if(!errno && (tmp > INT_MAX || tmp < 0)) errno = ERANGE;
				if(!errno && endptr[0]) errno = EINVAL;
				if(errno) { my_perror(argv[0]); return 1; }
				len = tmp;
			}
			break;
		case 'c':
			clear = 1;
			break;
		case '?':
			return 1;
		}
	}

	if(level) {
		if(klogctl(8, 0, level)) { my_perror(argv[0]); return 1; }
		return 0;
	}

	buf = malloc(len);
	if(!buf) { my_perror(argv[0]); return 1; }
	len = klogctl(3 + clear, buf, len);
	if(len < 0) { my_perror(argv[0]); return 1; }

	for(;len;len--,buf++) {
		if(last == '\n' && *buf == '<') { while(last != '>') { last = *buf; len--;buf++; }}
		do {
			errno = 0;
			write(1, buf, 1);
		} while(errno == EINTR);
		last = *buf;
	}
	if(!errno) close(1);
	if(errno) { my_perror(argv[0]); return 1; }
}
