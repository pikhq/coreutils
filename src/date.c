#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#include "util.h"

/* The standard strftime function is somewhat irritating to use *correctly*;
 * wrap it all up in here.
 *
 * Irritating issues handled:
 *   strftime provides no indication of how long the output string should be
 *   when the format results in a zero length string, strftime returns 0
 * which is the same value it returns when the buffer was not long enough
 * 
 */
static ssize_t astrftime(char **buf, const char *fmt, const struct tm *tm)
{
	char *tmp;
	size_t alloc;
	size_t len;
	size_t fmt_len;
	size_t new_fmt_len;
	char *new_fmt = 0;

	if(!buf) return -1;
	*buf = 0;

	fmt_len = strlen(fmt) + 1;
	// strlen(fmt) + null delimiter + " "
	new_fmt_len = fmt_len + 1 + 1;
	new_fmt = malloc(new_fmt_len);
	if(!new_fmt) goto error;

	// Like snprintf(new_fmt, new_fmt_len, "%s ", fmt);, but manual
	memcpy(new_fmt, fmt, fmt_len);
	new_fmt[fmt_len - 1] = ' ';
	new_fmt[fmt_len] = '\0';

	/* Start with a very small buffer; the common case is going to be
	 * fairly small strings, so don't rely on the heap having more than
	 * a trivial amount of space except when necessary.
	 */
	alloc = 128;
	*buf = malloc(alloc);
	if(!*buf) goto error;

	do {
		len = strftime(*buf, alloc, new_fmt, tm);
		
		if(len == 0) {
			if(alloc == SIZE_MAX) {
				errno = ENOMEM;
				goto error;
			}
			if(alloc + alloc/2 < alloc) {
				alloc = SIZE_MAX;
			} else {
				alloc += alloc/2;
			}
			
			tmp = realloc(*buf, alloc);
			if(!tmp) goto error;
			*buf = tmp;
		}
	} while(len == 0);

	// Overwrite the space that we left there with a null delimiter
	len--;
	buf[len] = '\0';

	free(new_fmt);

	/* Try and shrink the result to match the actual size of the string.
	 * On failure, just return the old, larger-than-necessary buffer.
	 */
	tmp = realloc(*buf, len);
	if(tmp) {
		*buf = tmp;
		errno = 0;
	}
	return len;

error:
	free(new_fmt);
	free(*buf);
	return -1;
}

int main(int argc, char **argv)
{
	int n = argc - 1;
	char **v = argv+1;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	setlocale(LC_TIME, "");

	for(; n && v[0][0] == '-' && v[0][1]; v++, n--) {
		if(v[0][1] == '-' && !v[0][2]) { // --
			v++; n--;
			break;
		}
		if(v[0][1] == 'u' && !v[0][2]) {
			if(setenv("TZ", "UTC0", 1)) {
				write_err(argv[0], errno, 0);
				return 1;
			}
			continue;
		}
		write_err(argv[0], EINVAL, v[0]);
		return 1;
	}

	if(!n) {
		char *time_locale;
		char *fmt = 0;

		/* The output of date(1) is localized. Unfortunately, the
		 * required output in the POSIX locale does not have a standard
		 * format string for strftime to get at it. So, in the POSIX
		 * locale we use the specified string and in all others we use
		 * the %c format, which is the "locale's appropriate date and
		 * time representation".
		 * (%c almost, but doesn't quite, match the expected output in
		 * that locale: it misses the time zone. Some testing suggests
		 * most other locales do include the time zone, though.)
		 */

		n = 1;

		time_locale = setlocale(LC_TIME, 0);

		if(strcmp(time_locale, "POSIX") == 0
		  || strcmp(time_locale, "C") == 0
		  || strcmp(time_locale, "C.UTF-8") == 0)
			fmt = "+%a %b %e %H:%M:%S %Z %Y";
		else
			fmt = "+%c";

		v = (char*[]){ fmt, 0 };
	}

	if(n > 1) {
		write_err(argv[0], 0, "Too many operands");
		return 1;
	}

	if(v[0][0] == '+') {
		ssize_t len;
		struct tm tm;

		char *buf;

		if(!localtime_r(&(time_t){time(0)}, &tm)) {
			write_err(argv[0], errno, 0);
			return 1;
		}

		len = astrftime(&buf, v[0] + 1, &tm);
		if(len < 0) {
			write_err(argv[0], errno, 0);
			return 1;
		}

		if(write_fd(1, buf, len) < len
		  || write_fd(1, "\n", 1) < 1
		  || close(1) == -1) {
			write_err(argv[0], errno, 0);
			return 1;
		}
	} else {
		char *end = 0;
		struct tm tm;
		if(parse_posixdate(v[0], &tm)) {
			if(!errno)
				errno = EINVAL;
			write_err(argv[0], errno, 0);
			return 1;
		}

		time_t conv_time = mktime(&tm);
		if(conv_time == -1) {
			write_err(argv[0], errno, 0);
			return 1;
		}

		if(clock_settime(CLOCK_REALTIME, &(struct timespec){ conv_time, 0 }) == -1) {
			write_err(argv[0], errno, 0);
			return 1;
		}
	}
	return 0;
}
