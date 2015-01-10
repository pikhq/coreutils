#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <fcntl.h>

#include "getopt_long.h"
#include "util.h"

static uint32_t sysv_sum(size_t len, unsigned char *c, uint32_t sum)
{
	for(; len; len--, c++) {
		sum += *c;
	}
	return sum;
}

static uint32_t bsd_sum(size_t len, unsigned char *c, uint32_t sum)
{
	uint16_t tmp = sum;
	for(; len; len--, c++) {
		tmp = (tmp >> 1) | (tmp << 15);
		tmp += *c;
	}
	return tmp;
}

static int cksum(int f, char *name, int sysv)
{
	off_t len = 0;
	uint32_t sum = 0;
	ssize_t read_len;
	char buf[12 * 1024];

	uint32_t (*func)(size_t,unsigned char*,uint32_t) = sysv ? sysv_sum : bsd_sum;
	int bs = sysv ? 512 : 1024;
	static const char sysv_fmt[] = "%"PRIu32" %jd%s%s\n";
	static const char bsd_fmt[]  = "%05"PRIu32" %5jd%s%s\n";

	while((read_len = read(f, buf, sizeof(buf))) > 0) {
		sum = func(read_len, buf, sum);
		len += read_len;
	}
	if(read_len < 0) return read_len;

	if(sysv) {
		sum = (sum & 0xffff) + (sum >> 16);
		sum = (sum & 0xffff) + (sum >> 16);
	}

	return dprintf(1, sysv ? sysv_fmt : bsd_fmt, sum, (intmax_t)len / bs + !!(len % bs), name ? " " : "", name ? name : "");
}

static const struct option longopts = 
	{"sysv", no_argument, 0, 's' }
;

int main(int argc, char **argv)
{
	int n;
	char **v;
	int ret = 0;
	int optc;

	int sysv = 0;
	int out_names = 0;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	while((optc = getopt_long(argc, argv, "+rs", &longopts, 0)) != -1) {
		switch(optc) {
		case 'r':
			sysv = 0;
			break;
		case 's':
			sysv = 1;
			break;
		default:
			return 1;
		}
	}

	n = argc - optind;
	v = argv + optind;
	out_names = (n > 1) || (n && sysv);

	if(!n) {
		if(cksum(0, 0, sysv) < 0) { write_err(argv[0], errno, 0); ret = 1; }
	}

	for(; n; v++, n--) {
		int f = v[0][0] != '-' || v[0][1]
		      ? open(*v, O_RDONLY)
		      : 0;
		if(f < 0) { write_err(argv[0], errno, 0); ret = 1; continue; }
		if(cksum(f, out_names ? *v : 0, sysv) < 0) { write_err(argv[0], errno, 0); ret = 1; }
		if(f != 0 && close(f) < 0) { write_err(argv[0], errno, 0); ret = 1; }
	}

	if(close(1) < 0) { write_err(argv[0], errno, 0); ret = 1; }

	return ret;
}
