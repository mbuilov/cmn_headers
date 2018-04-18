#ifndef DPRINT_H_INCLUDED
#define DPRINT_H_INCLUDED

/**********************************************************************************
* Debugg printing/tracing helpers
* Copyright (C) 2012-2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* dprint.h */

/* defines macros:

  DBGPRINT(format, ...)
  DBGPRINTX(file, line, function, format, ...)
*/

#ifdef DPRINT_TO_STDERR
#include <stdio.h> /* for fprintf() */
#endif

#include "sal_defs.h" /* for A_Printf_format_at()/A_Printf_format_string */

#ifdef __cplusplus
extern "C" {
#endif

/* format specifier for CURRENT_THREAD_ID() */
#ifndef THREAD_ID_FORMAT
#define THREAD_ID_FORMAT "%llx"
#endif

#ifdef CURRENT_THREAD_ID
/* NOTE: #include <windows.h> or <pthread.h> before this file */
#ifdef WIN32
#define CURRENT_THREAD_ID ((long long)0 + GetCurrentThreadId())
#elif defined _POSIX_THREADS
#define CURRENT_THREAD_ID ((long long)0 + pthread_self())
#else
#define CURRENT_THREAD_ID ((long long)0)
#endif
#endif /* CURRENT_THREAD_ID */

#if defined _MSC_VER && !defined __func__
#define __func__ __FUNCTION__
#endif

#if defined __SUNPRO_CC && !defined __func__
#define __func__ ""
#endif

/*    DPRINT_TO_LOG    -> print debug messages to log (file)
 else DPRINT_TO_STDERR -> print debug messages to stderr
 else                  -> don't print anything */

#ifdef DPRINT_TO_LOG

A_Printf_format_at(1,2)
void DPRINT_TO_LOG(A_Printf_format_string const char *format, ...);

#define DBGPRINT3_1(f) \
	DPRINT_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __FILE__, __LINE__, __func__)
#define DBGPRINT3_2(f,...) \
	DPRINT_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define DBGPRINT3x1(__d_file,__d_line,__d_function,f) \
	DPRINT_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __d_file, __d_line, __d_function)
#define DBGPRINT3x2(__d_file,__d_line,__d_function,f,...) \
	DPRINT_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __d_file, __d_line, __d_function, __VA_ARGS__)

#elif defined DPRINT_TO_STDERR

#define DBGPRINT3_1(f) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __FILE__, __LINE__, __func__)
#define DBGPRINT3_2(f,...) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define DBGPRINT3x1(__d_file,__d_line,__d_function,f) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __d_file, __d_line, __d_function)
#define DBGPRINT3x2(__d_file,__d_line,__d_function,f,...) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __d_file, __d_line, __d_function, __VA_ARGS__)

#endif /* DPRINT_TO_STDERR */

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STDERR

#define DPRN_ARGS2(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,n,...) n
#define DPRN_ARGS1(args)    DPRN_ARGS2 args
#define DPRN_ARGS(...)      DPRN_ARGS1((__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0))
#define DBGPRINT3(X,N,args) DBGPRINT3##X##N args
#define DBGPRINT2(X,N,args) DBGPRINT3(X,N,args)
#define DBGPRINT1(X,N,args) DBGPRINT2(X,N,args)
/* add '\n' at end of format string, works for maximum 30 arguments */
#define DBGPRINT(...)                                 DBGPRINT1(_,DPRN_ARGS(__VA_ARGS__),(__VA_ARGS__))
#define DBGPRINTX(__d_file,__d_line,__d_function,...) DBGPRINT1(x,DPRN_ARGS(__VA_ARGS__),(__d_file,__d_line,__d_function,__VA_ARGS__))

#else /* !DPRINT_TO_LOG && !DPRINT_TO_STDERR */

#define DBGPRINT(...)                                 ((void)0)
#define DBGPRINTX(__d_file,__d_line,__d_function,...) ((void)(__d_file),(void)(__d_line),(void)(__d_function))

#endif /* !DPRINT_TO_LOG && !DPRINT_TO_STDERR */

#ifdef __cplusplus
}
#endif

#endif /* DPRINT_H_INCLUDED */
