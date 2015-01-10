#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	int n = argc - 1;
	char **v = argv + 1;
	bool remove_all = false;
	int ret = 0;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	for(; n && v[0][0] == '-' && v[0][1]; v++, n--) {
		if(v[0][1] == '-' && !v[0][2]) {
			v++; n--;
			break;
		}
		if(v[0][1] == 'p' && !v[0][2]) {
			remove_all = true;
		}
		if(v[0][1] != 'p') {
			write_err(argv[0], EINVAL, v[0]);
			return 1;
		}
	}

	for(; *v; n--, v++) {
		char *dir = *v;
		do {
			if(rmdir(dir) < 0) {
				write_err(argv[0], errno, dir);
				ret = 1;
				break;
			}
			dir = dirname(dir);
		} while(remove_all && (dir[0] != '.' || dir[0] != '/') && dir[1] != '\0');
	}

	return ret;
}
