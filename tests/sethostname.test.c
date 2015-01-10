#define _GNU_SOURCE
#define _ALL_SOURCE
#include <unistd.h>

#undef sethostname

int main()
{
	int (*p)(const char*, size_t) = sethostname;
	int (sethostname)(const char*, size_t);
	return sethostname("gibberish", sizeof("gibberish")-1);
}
