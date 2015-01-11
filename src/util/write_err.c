#include <unistd.h>
#include <string.h>
#include "util.h"

void write_err(const char *prog, int error, const char *msg)
{
	char *err;

	err = error != 0 ? strerror(error) : 0;

	write(2, prog, strlen(prog));
	if(msg) {
		write(2, ": ", 2);
		write(2, msg, strlen(msg));
	}
	if(err) {
		write(2, ": ", 2);
		write(2, err, strlen(err));
	}
	write(2, "\n", 1);
}
