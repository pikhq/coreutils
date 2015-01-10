include build.mk

PROGS_SRCS=$(wildcard src/*.c)
PROGS=$(PROGS_SRCS:.c=)

SRCS=$(wildcard src/*.c src/*/*.c)
OBJS=$(SRCS:.c=.o)

UTIL_SRCS=$(wildcard src/util/*.c)
UTIL_OBJS=$(UTIL_SRCS:.c=.o)
UTIL_PORT=$(wildcard src/util/*.inc)
UTIL_PORT_OBJS=$(UTIL_PORT:.inc=.o)
UTIL_PORT_SRCS=$(UTIL_PORT:.inc=.c)
UTIL_OBJS+=$(UTIL_PORT_OBJS)

TEST_SRCS=$(wildcard tests/*.c)
TEST_OBJS=$(TEST_SRCS:.c=.o) $(TEST_SRCS:.c=)

all: $(PROGS)

CPPFLAGS += -Iinclude -D_XOPEN_SOURCE=700 -D_FILE_OFFSET_BITS=64

$(OBJS): $(wildcard include/*.h) include/noreturn.h

src/util.a: $(UTIL_OBJS)

src/%: src/%.o src/util.a
	$(CC) $(LDFLAGS) -o $@ $< src/util.a $(LDLIBS)

include/noreturn.h: include/noreturn.inc
	-$(CC) $(CFLAGS) $(CPPFLAGS) -o /dev/null tests/stdnoreturn.test.c \
		&& echo "#include <stdnoreturn.h>">include/noreturn.h \
		|| cp include/noreturn.inc include/noreturn.h

src/util/asprintf.c: src/util/asprintf.inc src/util/asprintf.fallback
	-$(CC) $(CFLAGS) $(CPPFLAGS) -o /dev/null tests/asprintf.test.c $(LDLIBS) \
		&& cp src/util/asprintf.inc src/util/asprintf.c \
		|| cp src/util/asprintf.fallback src/util/asprintf.c

clean:
	-rm -f $(PROGS)
	-rm -f $(OBJS) $(TEST_OBJS)
	-rm -f $(UTIL_PORT_SRCS)
	-rm -f src/util.a
	-rm -f include/noreturn.h
