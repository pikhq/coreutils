#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

static void parse_escape(char *str)
{
	for(; *str; str++) {
		unsigned char c = *str;
		if(c == '\\') {
			switch(*++str) {
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 'c':
				exit(0);
			case 'f':
				c = '\f';
				break;
			case 'n':
				c = '\n';
				break;
			case 'r':
				c = '\r';
				break;
			case 't':
				c = '\t';
				break;
			case 'v':
				c = '\v';
				break;
			case '0':
				c = 0;
				for(int n = 0; n < 3; n++) {
					if(str[1] >= '0' && str[1] <= '7') {
						c <<= 3;
						c |= str[1] - '0';
						str++;
					} else {
						break;
					}
				}
				break;
			default:
				c = *str;
			}
		}
		if(write(1, &c, 1) < 0) return;
	}
	errno = 0;
}

int main(int argc, char **argv)
{
	char *err;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	for(int i = 1; i < argc; i++) {
		parse_escape(argv[i]);
		if(errno) goto do_err;
		if(i != argc-1)
			if(write(1, &(char){' '}, 1) < 0) goto do_err;
	}
	if(write(1, &(char){'\n'}, 1) < 0) goto do_err;
	return 0;

do_err:
	err = strerror(errno);
	write(2, argv[0], strlen(argv[0]));
	write(2, ": ", 2);
	write(2, err, strlen(err));
	write(2, "\n", 1);
	return 1;
}
