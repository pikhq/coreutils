#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

struct tm;

size_t write_fd(int fd, const char *s, size_t len);
int parse_posixdate(char*, struct tm*);

#endif
