#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	char *logname;
	size_t logname_len;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	logname = getlogin();
	if(!logname) goto error;
	logname_len = strlen(logname);

	if(write_fd(1, logname, logname_len) < logname_len
	   || write_fd(1, "\n", 1) < 1) {
		goto error;
	}

	return 0;

error:
	write_err(argv[0], errno, 0);
	return 1;
}
