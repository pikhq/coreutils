#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <locale.h>

#include "util.h"

int main(int argc, char **argv)
{
	off_t index = 1;
	off_t line_num = 1;
	FILE *f1, *f2;
	int byte1, byte2;
	int c;
	char **v;
	int n;
	int ret = 0;

	enum {
		FIRST_DIFF,
		ALL_DIFF,
		SILENT
	} mode = FIRST_DIFF;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	while((c = getopt(argc, argv, "ls")) != -1) {
		switch(c) {
		case 'l':
			mode = ALL_DIFF;
			break;
		case 's':
			mode = SILENT;
			break;
		default:
			return 2;
		}
	}

	n = argc - optind;
	v = argv + optind;

	if(n < 2) {
		fprintf(stderr, "%s: Missing operands.\n", argv[0]);
		return 2;
	} else if(n > 2) {
		fprintf(stderr, "%s: Too many operands.\n", argv[0]);
		return 2;
	}

	f1 = fopen(v[0], "r");
	if(!f1) goto error;
	f2 = fopen(v[1], "r");
	if(!f2) goto error;

	do {
		byte1 = fgetc(f1);
		if(ferror(f1)) goto error;
		byte2 = fgetc(f2);
		if(ferror(f2)) goto error;

		if((byte1 == EOF || byte2 == EOF) && byte1 != byte2
		   && mode != SILENT) {
			fprintf(stderr, "cmp: EOF on %s\n",
			        byte1 == EOF ? v[0] : v[1]);
			if(ferror(stderr)) goto error;
			return 1;
		}

		if(byte1 != byte2) {
			ret = 1;
			if(mode == FIRST_DIFF) {
				printf("%s %s differ: char %jd, line %jd\n",
				       v[0], v[1], (intmax_t)index,
				       (intmax_t)line_num);
				if(ferror(stdout)) goto error;
				return ret;
			} else if(mode == ALL_DIFF) {
				printf("%jd %o %o\n", (intmax_t)index,
				       byte1, byte2);
				if(ferror(stdout)) goto error;
			} else {
				return ret;
			}
		}

		index++;
		if(byte1 == '\n' && byte2 == '\n')
			line_num++;
	} while(!feof(f1) && !feof(f2));

	return ret;
error:
	write_err(argv[0], errno, 0);
	return 2;
}
