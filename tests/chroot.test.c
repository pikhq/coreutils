#define _GNU_SOURCE
#define _ALL_SOURCE
#undef _XOPEN_SOURCE
#undef _POSIX_C_SOURCE
#define _XOPEN_SOURCE 500
#include <unistd.h>

#undef chroot

int main(int argc, char **argv)
{
	int (*p)(const char*) = chroot;
	int (chroot)(const char*);
	return chroot(argv[1]);
}
