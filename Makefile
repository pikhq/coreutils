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

all: $(progs)


src/basename: src/basename.c
src/cat:      src/cat.c
src/cksum:    src/cksum.c
src/dirname:  src/dirname.c
src/dmesg:    src/dmesg.c
src/echo:     src/echo.c
src/env:      src/env.c
src/false:    src/false.c
src/groups:   src/groups.c
src/hostname: src/hostname.c
src/link:     src/link.c
src/logger:   src/logger.c
src/mesg:     src/mesg.c
src/nice:     src/nice.c
src/nohup:    src/nohup.c
src/sleep:    src/sleep.c
src/sum:      src/sum.c
src/sync:     src/sync.c
src/tee:      src/tee.c
src/true:     src/true.c
src/tty:      src/tty.c
src/uname:    src/uname.c
src/unlink:   src/unlink.c

clean:
	-rm -f $(progs)
