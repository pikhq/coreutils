#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>

#include "util.h"

int main(int argc, char **argv)
{
	int c = argc-1;
	char **v = argv+1;
	size_t len;
	char *buf;
	char *errstring;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	if(v[0][0] == '-' && v[0][1] == '-' && !v[0][2]) { c--; v++; }

	for(int i = 0; i < c; i++) {
		len += strlen(v[i]) + 1;
	}

	buf = malloc(len);
	if(!buf) goto err;
	buf[0] = 0;

	for(; c; c--, v++) {
		strcat(buf, v[0]);
		if(c != 1) strcat(buf, " ");
	}
	
	syslog(LOG_NOTICE, "%s", buf);
	return 0;

err:
	write_err(argv[0], errno, 0);
	return 1;
}
