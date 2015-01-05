#ifndef ASPRINTF_H
#define ASPRINTF_H

#undef asprintf
int repl_asprintf(char **, const char*, ...);
#define asprintf repl_asprintf

#endif
