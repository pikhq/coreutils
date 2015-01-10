#define _GNU_SOURCE
#define _ALL_SOURCE
#include <sys/klog.h>

#undef klogctl

int main()
{
	int (*p)(int,char*,int) = klogctl;
	int (klogctl)(int,char*,int);
	char buf[1024];
	return klogctl(2, buf, sizeof(buf));
}
