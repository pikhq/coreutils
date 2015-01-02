#define _XOPEN_SOURCE 700
#include <string.h>
#include <libgen.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>

static int my_write(int fd, const char *str, size_t len)
{
        while(len) {
                ssize_t n = write(fd, str, len);
                if(n < 0 && errno != EINTR) continue;
                if(n < 0) return -1;
                str += n; len -= n;
        }
        return 0;
}

static void my_perror(char *prog)
{
        char *err = strerror(errno);
        write(2, prog, strlen(prog));
        write(2, ": ", 2);
        write(2, err, strlen(err));
        write(2, "\n", 1);
}

int main(int argc, char **argv)
{
	int n = argc - 1;
	char **v = argv + 1;
	char *str;

	setlocale(LC_CTYPE, "");
#ifdef LC_MESSAGES
	setlocale(LC_MESSAGES, "");
#endif
	if(n && strcmp(v[0], "--") == 0) { n--; v++; }

	if(!n) {
		write(2, argv[0], strlen(argv[0]));
		write(2, ": Missing operand\n", sizeof(": Missing operand\n")-1);
		return 1;
	}

	str = dirname(v[0]);

	if(   my_write(1, str, strlen(str)) < 0
	   || my_write(1, "\n", 1) < 0) {
		my_perror(argv[0]);
		return 1;
	}
}
