#ifndef DPRINT_H_INCLUDED
#define DPRINT_H_INCLUDED

/**********************************************************************************
* Debugg printing/tracing helpers
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* dprint.h */

/* defines macros:

  DBGPRINT(format, ...)
  DBGPRINTX(file, line, function, format, ...)
*/

#ifdef _DEBUG
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

/* DEBUG_TO_LOG -> print debug messages to log (file)
 else _DEBUG    -> print debug messages to stderr
 else           -> don't print anything */

#ifdef DEBUG_TO_LOG

A_Printf_format_at(1,2)
void DEBUG_TO_LOG(A_Printf_format_string const char *format, ...);

#define ___DBGPRINT_1(f) \
	DEBUG_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __FILE__, __LINE__, __func__)
#define ___DBGPRINT_2(f,...) \
	DEBUG_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ___DBGPRINTx1(__d_file,__d_line,__d_function,f) \
	DEBUG_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __d_file, __d_line, __d_function)
#define ___DBGPRINTx2(__d_file,__d_line,__d_function,f,...) \
	DEBUG_TO_LOG(THREAD_ID_FORMAT ":%s:%d:%s(): " f, CURRENT_THREAD_ID, __d_file, __d_line, __d_function, __VA_ARGS__)

#elif defined _DEBUG

#define ___DBGPRINT_1(f) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __FILE__, __LINE__, __func__)
#define ___DBGPRINT_2(f,...) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ___DBGPRINTx1(__d_file,__d_line,__d_function,f) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __d_file, __d_line, __d_function)
#define ___DBGPRINTx2(__d_file,__d_line,__d_function,f,...) \
	fprintf(stderr, THREAD_ID_FORMAT ":%s:%d:%s(): " f "\n", CURRENT_THREAD_ID, __d_file, __d_line, __d_function, __VA_ARGS__)

#endif /* _DEBUG */

#if defined DEBUG_TO_LOG || defined _DEBUG

#define ___DPRN_ARGS(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,n,...) n
#define __DPRN_ARGS(args)     ___DPRN_ARGS args
#define _DPRN_ARGS(...)       __DPRN_ARGS((__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0))
#define ___DBGPRINT(X,N,args) ___DBGPRINT##X##N args
#define __DBGPRINT(X,N,args)  ___DBGPRINT(X,N,args)
#define _DBGPRINT(X,N,args)   __DBGPRINT(X,N,args)
/* add '\n' at end of format string, works for maximum 30 arguments */
#define DBGPRINT(...)                                 _DBGPRINT(_,_DPRN_ARGS(__VA_ARGS__),(__VA_ARGS__))
#define DBGPRINTX(__d_file,__d_line,__d_function,...) _DBGPRINT(x,_DPRN_ARGS(__VA_ARGS__),(__d_file,__d_line,__d_function,__VA_ARGS__))

#else /* !DEBUG_TO_LOG && !_DEBUG */

#define DBGPRINT(...)                                 ((void)0)
#define DBGPRINTX(__d_file,__d_line,__d_function,...) ((void)(__d_file),(void)(__d_line),(void)(__d_function))

#endif /* !DEBUG_TO_LOG && !_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* DPRINT_H_INCLUDED */
