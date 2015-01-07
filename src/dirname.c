#include <string.h>
#include <libgen.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>

#include "util.h"

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
		write_err(argv[0], 0, "Missing operand");
		return 1;
	}

	str = dirname(v[0]);

	if(write_fd(1, str, strlen(str)) < strlen(str)
	   || write_fd(1, "\n", 1) < 1) {
		write_err(argv[0], errno, 0);
		return 1;
	}
}
