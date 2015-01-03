#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "util.h"

int main(int argc, char **argv)
{
	bool def_time = true;
	bool atime = false;
	bool mtime = false;
	bool create_file = true;
	char *date_string = NULL;
	char *ref_file = NULL;
	char *time_string = NULL;

	struct timespec timespec[2] = {};

	int c;

	int n;
	char **v;

	while((c = getopt(argc, argv, "acd:mr:t:")) != -1) {
		switch(c) {
		case 'a':
			def_time = false;
			atime = true;
			break;
		case 'c':
			create_file = false;
			break;
		case 'd':
			date_string = optarg;
			ref_file = time_string = 0;
			break;
		case 'm':
			def_time = false;
			mtime = true;
			break;
		case 'r':
			ref_file = optarg;
			date_string = time_string = 0;
			break;
		case 't':
			time_string = optarg;
			date_string = ref_file = 0;
			break;
		default:
			return 1;
		}
	}

	if(date_string) {
		char *end;
		struct tm tm;

		end = strptime(date_string, "%Y-%m-%dT%H:%M:%S", &tm);
		if(!end) {
			perror(argv[0]);
			return 1;
		}

		if(*end == '.' || *end == ',') {
			size_t digits;

			end++;
			digits = strlen(end);

			errno = 0;
			timespec[0].tv_nsec = strtoul(end, &end, 10);
			if(timespec[0].tv_nsec == ULONG_MAX && errno != 0) {
				perror(argv[0]);
				return 1;
			}
			
			while(digits < 9) {
				timespec[0].tv_nsec *= 10;
				digits++;
			}
			while(digits > 9) {
				timespec[0].tv_nsec /= 10;
				digits--;
			}
		}

		if(*end == 'Z') {
			end++;
			setenv("TZ", "UTC0", 1);
			tzset();
		}

		if(*end) {
			errno = EINVAL;
			perror(argv[0]);
			return 1;
		}

		timespec[0].tv_sec = mktime(&tm);
		if(timespec[0].tv_sec == -1) {
			perror(argv[0]);
			return 1;
		}
		timespec[1] = timespec[0];
	} else if(time_string) {
		struct tm tm;
		if(parse_posixdate(time_string, &tm)) {
			perror(argv[0]);
			return 1;
		}
		timespec[0].tv_sec = mktime(&tm);
		if(timespec[0].tv_sec == -1) {
			perror(argv[0]);
			return 1;
		}
		timespec[1] = timespec[0];
	} else if(ref_file) {
		struct stat buf;
		if(stat(ref_file, &buf) == -1) {
			perror(argv[0]);
			return 1;
		}
		timespec[0] = buf.st_atim;
		timespec[1] = buf.st_mtim;
	} else {
		if(clock_gettime(CLOCK_REALTIME, &timespec[0]) == -1) {
			perror(argv[0]);
			return 1;
		}
		timespec[1] = timespec[0];
	}

	if(!atime && !def_time) {
		timespec[0].tv_nsec = UTIME_OMIT;
	}
	if(!mtime && !def_time) {
		timespec[1].tv_nsec = UTIME_OMIT;
	}

	n = argc - optind;
	v = argv + optind;

	for(; n; v++, n--) {

		int fd = open(*v, create_file ? O_CREAT : 0|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
		if(fd != -1) {
			if(futimens(fd, timespec) == -1) {
				perror(argv[0]);
				return 1;
			}
		} else if(errno == EACCES) {
			if(utimensat(AT_FDCWD, *v, timespec, 0) == -1) {
				perror(argv[0]);
				return 1;
			}
		} else if(errno != ENOENT || create_file) {
			perror(argv[0]);
			return 1;
		}
	}

	return 0;
}
