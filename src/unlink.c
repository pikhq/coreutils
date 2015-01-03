#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

#define CSTR(s) (s), sizeof(s)-1

int main(int argc, char **argv)
{
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	if(argc != 2) {
		write(2, argv[0], strlen(argv[0]));
		write(2, CSTR(": Missing operand\n"));
		return 1;
	}
	if(unlink(argv[1])) {
		char *err = strerror(errno);
		write(2, argv[0], strlen(argv[0]));
		write(2, CSTR(": "));
		write(2, err, strlen(err));
		write(2, CSTR("\n"));
		return 1;
	}
}
