include build.mk

progs = src/basename \
	src/cat \
	src/cksum \
	src/dirname \
	src/dmesg \
	src/echo \
	src/env \
	src/false \
	src/groups \
	src/hostname \
	src/link \
	src/logger \
	src/mesg \
	src/nice \
	src/nohup \
	src/sleep \
	src/sum \
	src/sync \
	src/tee \
	src/true \
	src/tty \
	src/uname \
	src/unlink

SRCS=$(wildcard src/*.c src/*/*.c)
OBJS=$(SRCS:.c=.o)

UTIL_SRCS=$(wildcard src/util/*.c)
UTIL_OBJS=$(UTIL_SRCS:.c=.o)

all: $(progs)

CPPFLAGS += -Iinclude

src/util.a: $(UTIL_OBJS)

src/%: src/%.o src/util.a
	$(CC) $(LDFLAGS) -o $@ $< src/util.a $(LDLIBS)

clean:
	-rm -f $(progs)
	-rm -f $(OBJS)
	-rm -f src/util.a
