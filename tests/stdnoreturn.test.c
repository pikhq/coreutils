#include <stdnoreturn.h>

static noreturn void donothing(void)
{
	for(;;);
}

int main()
{
	donothing();
}
