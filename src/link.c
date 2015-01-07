#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	if(argc != 3) {
		write_err(argv[0], 0, "Missing arguments\n");
		return 1;
	}
	if(link(argv[1], argv[2])) {
		write_err(argv[0], errno, 0);
		return 1;
	}
}
