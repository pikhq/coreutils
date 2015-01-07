#include <sys/utsname.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"

static void uname_entry(char *prog, int *any_wrote, char *entry)
{
	if(*any_wrote)
		if(write_fd(1, " ", 1) < 1) { write_err(prog, errno, 0); exit(1); }
	*any_wrote = 1;
	if(write_fd(1, entry, strlen(entry)) < strlen(entry)) { write_err(prog, errno, 0); exit(1); }
}

int main(int argc, char **argv)
{
	int any_opts = 0;
	int any_wrote = 0;
	int opt_a = 0;
	int opt_m = 0;
	int opt_n = 0;
	int opt_r = 0;
	int opt_s = 0;
	int opt_v = 0;
	int c;
	struct utsname name;
	
	while((c = getopt(argc, argv, "amnrsv")) != -1) {
		any_opts = 1;
		switch(c) {
		case 'a': opt_a = 1; break;
		case 'm': opt_m = 1; break;
		case 'n': opt_n = 1; break;
		case 'r': opt_r = 1; break;
		case 's': opt_s = 1; break;
		case 'v': opt_v = 1; break;
		default:  return 1;
		}
	}

	if(uname(&name) < 0) { write_err(argv[0], errno, 0); return 1; }
	if(!any_opts) opt_s = 1;

	if(opt_s || opt_a)
		uname_entry(argv[0], &any_wrote, name.sysname);
	if(opt_n || opt_a)
		uname_entry(argv[0], &any_wrote, name.nodename);
	if(opt_r || opt_a)
		uname_entry(argv[0], &any_wrote, name.release);
	if(opt_v || opt_a)
		uname_entry(argv[0], &any_wrote, name.version);
	if(opt_m || opt_a)
		uname_entry(argv[0], &any_wrote, name.machine);
	if(write_fd(1, "\n", 1) < 1) { write_err(argv[0], errno, 0); return 1; }
}
