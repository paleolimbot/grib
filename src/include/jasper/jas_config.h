#ifndef JAS_CONFIG_H
#define JAS_CONFIG_H

#include <jasper/jas_dll.h>

/* This preprocessor symbol identifies the version of JasPer. */
#define	JAS_VERSION "unknown"

#define JAS_HAVE_FCNTL_H 1
#define JAS_HAVE_IO_H 1
#define JAS_HAVE_UNISTD_H 1
#define JAS_HAVE_SYS_TIME_H 1
#define JAS_HAVE_SYS_TYPES_H 1

#define JAS_HAVE_GETTIMEOFDAY 1
#define JAS_HAVE_GETRUSAGE 1

#define JAS_HAVE_GL_GLUT_H 1
#define JAS_HAVE_GLUT_H 1

#define JAS_HAVE_SNPRINTF	1

#define ssize_t long long

#if !defined(JAS_DEC_DEFAULT_MAX_SAMPLES)
#define JAS_DEC_DEFAULT_MAX_SAMPLES (64 * ((size_t) 1048576))
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define JAS_ATTRIBUTE_DISABLE_USAN \
  __attribute__((no_sanitize_undefined))
#elif defined(__clang__)
#define JAS_ATTRIBUTE_DISABLE_USAN \
  __attribute__((no_sanitize("undefined")))
#else
#define JAS_ATTRIBUTE_DISABLE_USAN
#endif

#endif
