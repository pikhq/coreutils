#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

int main(int argc, char **argv)
{
	int put_env = 1;
	int c;
	char *wd = NULL;

	while((c = getopt(argc, argv, "LP")) != -1) {
		if(c == '?') return 1;
		put_env = c == 'L';
	}

	if(put_env) {
		wd = getenv("PWD");
		if(wd) {
			char *s = wd;
			while(*s == '/') {
				char *tmp;
				if(*(++s) == '.') {
					if(s[1] == '/' || s[1] == '\0') break;
					if(s[1] == '.' && (s[2] == '/' || s[2] == '\0')) break;
				}
				while(*s && *s != '/') s++;
			}
			if(*s || s == wd) wd = NULL;
		}
	}
	if(!wd) {
		char *buf;
		size_t alloc;
#ifdef PATH_MAX
		alloc = PATH_MAX;
#else
		long path_max = pathconf(".", _PC_PATH_MAX);
		if(path_max > SIZE_MAX || path_max < 0) alloc = -1;
#endif
		if(alloc < 0) alloc = _POSIX_PATH_MAX;
		buf = malloc(alloc);
		if(!buf) { perror(argv[0]); return 1; }
		do {
			wd = getcwd(buf, alloc);
			if(!wd && errno != ERANGE) {
				perror(argv[0]);
				return 1;
			}
			if(!wd) {
				if(alloc == SIZE_MAX) {
					errno = ENOMEM;
					perror(argv[0]);
					return 1;
				}
				if(alloc + alloc/2 < alloc) {
					alloc = SIZE_MAX;
				}
				buf = realloc(buf, alloc);
				if(!buf) { perror(argv[0]); return 1; }
			}
		} while(!wd);
	}

	if(write_fd(1, wd, strlen(wd)) < strlen(wd)
	   || write_fd(1, "\n", 1) < 1
	   || close(1)) {
		perror(argv[0]);
		return 1;
	}
}
