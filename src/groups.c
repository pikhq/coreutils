#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	char *err;
	char *args[] = {"id", "-Gn", "--", argc > 1 ? argv[1] : 0, 0 };

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	execvp(args[0], args);
	write_err(argv[0], errno, 0);
	return errno != ENOENT ? 126 : 127;
}
