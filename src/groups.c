#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

int main(int argc, char **argv)
{
	char *err;
	char *args[] = {"id", "-Gn", "--", argc > 1 ? argv[1] : 0, 0 };

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	execvp(args[0], args);
	err = strerror(errno);
	write(2, argv[0], strlen(argv[0]));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
	return errno != ENOENT ? 126 : 127;
}
