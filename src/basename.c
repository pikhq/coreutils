#include <string.h>
#include <libgen.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>

#include "util.h"

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
	int n = argc - 1;
	char **v = argv + 1;
	char *str;

	setlocale(LC_CTYPE, "");
#ifdef LC_MESSAGES
	setlocale(LC_MESSAGES, "");
#endif
	if(n && strcmp(v[0], "--") == 0) { n--; v++; }

	if(!n) {
		write(2, argv[0], strlen(argv[0]));
		write(2, ": Missing operand\n", sizeof(": Missing operand\n")-1);
		return 1;
	}

	str = basename(v[0]);
	if(strcmp(v[0], ".") != 0
	   && strcmp(v[0], "/") != 0
	   && n > 1) {
		for(char *s = str + 1; *s; s++) {
			if(strcmp(s, v[1]) == 0) {
				*s = 0;
				break;
			}
		}
	}

	if(write_fd(1, str, strlen(str)) < strlen(str)
	   || write_fd(1, "\n", 1) < 1) {
		my_perror(argv[0]);
		return 1;
	}
}
