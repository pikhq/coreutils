include build.mk

PROGS_SRCS=$(wildcard src/*.c)
PROGS=$(PROGS_SRCS:.c=)

SRCS=$(wildcard src/*.c src/*/*.c)
OBJS=$(SRCS:.c=.o)

UTIL_SRCS=$(wildcard src/util/*.c)
UTIL_OBJS=$(UTIL_SRCS:.c=.o)

all: $(PROGS)

CPPFLAGS += -Iinclude -D_XOPEN_SOURCE=700 -D_FILE_OFFSET_BITS=64

$(OBJS): $(wildcard include/*.h)

src/util.a: $(UTIL_OBJS)

src/%: src/%.o src/util.a
	$(CC) $(LDFLAGS) -o $@ $< src/util.a $(LDLIBS)

clean:
	-rm -f $(PROGS)
	-rm -f $(OBJS)
	-rm -f src/util.a
