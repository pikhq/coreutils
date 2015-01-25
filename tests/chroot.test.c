#define _GNU_SOURCE
#define _ALL_SOURCE
#include <unistd.h>

#undef chroot

int main(int argc, char **argv)
{
	int (*p)(const char*) = chroot;
	int (chroot)(const char*);
	return chroot(argv[1]);
}
