#include <unistd.h>
#include <errno.h>

#include "util.h"
#include "chroot.h"

int main(int argc, char **argv)
{
	char **v = argv+1;
	int c = argc-1;

	for(; c; c--, v++) {
		if(v[0] == '-') {
			if(v[1] == '-' && v[2] == 0) break;
			write_err(argv[0], EINVAL, 0);
			return 1;
		}
		break;
	}

	if(chroot(v[0]) < 0) {
		write_err(argv[0], errno, 0);
		return 1;
	}
	if(v[1])
		execvp(v[1], &v[1]);
	else
		execlp("/bin/sh", "/bin/sh", "-i", (void*)0);
	int err = errno;
	write_err(argv[0], errno, 0);
	return err != ENOENT ? 126 : 127;
}
