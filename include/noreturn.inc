#ifndef NORETURN_H
#define NORETURN_H

#warning "Your compiler is missing stdnoreturn.h; providing fallback"

#if __STDC_VERSION < 201112L
# if __GNUC__
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
# else
#  define GCC_VERSION 0
# endif
# ifndef __has_attribute
#  define __has_attribute(x) 0
# endif

# undef _Noreturn
# if __has_attribute(__noreturn__) || GCC_VERSION >= 20500
#  define _Noreturn __attribute__((__noreturn__))
# else
#  define _Noreturn
# endif
#endif

#define noreturn _Noreturn

#endif
