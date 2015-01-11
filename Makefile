include build.mk

PROGS_SRCS=$(wildcard src/*.c)
PROGS=$(PROGS_SRCS:.c=)

SRCS=$(wildcard src/*.c src/*/*.c)
OBJS=$(SRCS:.c=.o)

UTIL_SRCS=$(wildcard src/util/*.c)
UTIL_PORT=$(wildcard src/util/*.inc)
UTIL_PORT_SRCS=$(UTIL_PORT:.inc=.c)
UTIL_SRCS+=$(UTIL_PORT_SRCS)
UTIL_OBJS=$(UTIL_SRCS:.c=.o)

TEST_SRCS=$(wildcard tests/*.c)

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

src/util/%.c: src/util/%.inc src/util/%.fallback tests/%.test.c
	-$(CC) $(CFLAGS) $(CPPFLAGS) -o /dev/null tests/$*.test.c $(LDLIBS) \
		&& cp src/util/$*.inc src/util/$*.c \
		|| cp src/util/$*.fallback src/util/$*.c

clean:
	-rm -f $(PROGS)
	-rm -f $(OBJS)
	-rm -f $(UTIL_PORT_SRCS)
	-rm -f src/util.a
	-rm -f include/noreturn.h
