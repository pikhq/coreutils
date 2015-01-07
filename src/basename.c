#include <string.h>
#include <libgen.h>
#include <locale.h>
#include <errno.h>

#include "util.h"

int main(int argc, char **argv)
{
	int n = argc - 1;
	char **v = argv + 1;
	char *str;
	char *err_msg = 0;

	setlocale(LC_CTYPE, "");
#ifdef LC_MESSAGES
	setlocale(LC_MESSAGES, "");
#endif
	if(n && strcmp(v[0], "--") == 0) { n--; v++; }

	if(!n) {
		write_err(argv[0], 0, "Missing operand");
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
		write_err(argv[0], errno, "write error");
		return 1;
	}
}
