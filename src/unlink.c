#include <unistd.h>
#include <errno.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	if(argc != 2) {
		write_err(argv[0], 0, "Missing operand");
		return 1;
	}
	if(unlink(argv[1])) {
		write_err(argv[0], errno, 0);
		return 1;
	}
}
