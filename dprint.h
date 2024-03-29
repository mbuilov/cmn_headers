#ifndef DPRINT_H_INCLUDED
#define DPRINT_H_INCLUDED

/**********************************************************************************
* Debug printing/tracing helpers
* Copyright (C) 2012-2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* dprint.h */

/* defines macros:

  DBGPRINT(format, ...)
  DBGPRINTX(file, line, function, format, ...)
*/

/* Note: DPRINT_TO_LOG - name of logging function (for its prototype - see below);
  if defined, then it is used for printing log messages (even in release builds). */

/* Note: DPRINT_TO_STREAM - name of output stream to print log messages to,
  if not defined, then in debug builds - it is defined as stderr. */
#ifndef DPRINT_TO_STREAM
#ifndef NDEBUG
/* define DPRINT_DISABLE to disable logging even in debug builds */
#ifndef DPRINT_DISABLE
#define DPRINT_TO_STREAM stderr
#endif
#endif
#endif

#ifdef DPRINT_TO_STREAM
#include <stdio.h> /* for fprintf() */
#endif

#include "annotations.h" /* for A_Printf_format() */

#ifdef __cplusplus
extern "C" {
#endif

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STREAM

/* define DPRINT_SHOW_THREAD_ID globally to print thread ID in log messages */
#ifdef DPRINT_SHOW_THREAD_ID

/* format specifier for DPRINT_GET_THREAD_ID */
#ifndef DPRINT_THREAD_ID_FORMAT
#define DPRINT_THREAD_ID_FORMAT "%llx"
#endif

#define DPRINT_THREAD_ID_SEP ":"

/* DPRINT_GET_THREAD_ID - function that returns current thread ID */
#ifndef DPRINT_GET_THREAD_ID

/* NOTE: #include <windows.h> or <pthread.h> before this file */
#ifdef _WIN32
#define DPRINT_GET_THREAD_ID ((long long)0 + GetCurrentThreadId())
#elif defined _POSIX_THREADS
#define DPRINT_GET_THREAD_ID ((long long)0 + pthread_self())
#else
#error unable to determine current thread identifier
#endif

#endif /* DPRINT_GET_THREAD_ID */

#else /* !DPRINT_SHOW_THREAD_ID */

#define DPRINT_THREAD_ID_FORMAT "%s"
#define DPRINT_THREAD_ID_SEP ""
#define DPRINT_GET_THREAD_ID ""

#endif /* !DPRINT_SHOW_THREAD_ID */

#ifndef DPRINT_NO_FUNC

/* DPRINT_FUNC - macro to obtain compiled source file name */
#ifndef DPRINT_FUNC

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define DPRINT_FUNC __func__
#elif defined __cplusplus && __cplusplus >= 201103L
#define DPRINT_FUNC __func__
#elif defined __SUNPRO_C || defined __SUNPRO_CC
#define DPRINT_FUNC __func__
#else
#define DPRINT_FUNC __FUNCTION__
#endif

#endif /* !DPRINT_FUNC */

#define DPRINT_FUNC_FORMAT "%s():"

#else /* DPRINT_NO_FUNC */

#define DPRINT_FUNC_FORMAT "%s"
#define DPRINT_FUNC ""

#endif /* DPRINT_NO_FUNC */

/* printf-like format of source file name and line number */
#ifndef DPRINT_FILE_LINE_FORMAT
#define DPRINT_FILE_LINE_FORMAT "%s:%d:"
#endif

/* log messages prefix, e.g.: "%llx:%s:%d:%s(): " */
#define DPRINT_LOCATION_FORMAT DPRINT_THREAD_ID_FORMAT DPRINT_THREAD_ID_SEP DPRINT_FILE_LINE_FORMAT DPRINT_FUNC_FORMAT " "

#endif /* DPRINT_TO_STREAM || DPRINT_TO_LOG */

/*    DPRINT_TO_LOG    -> print debug messages via custom function (to file, socket, etc.)
 else DPRINT_TO_STREAM -> print debug messages to standard stream (stderr or stdout)
 else                  -> don't print anything (in release, when NDEBUG is defined) */

#ifdef DPRINT_TO_LOG

/* prototype of custom logging function - it must be defined elsewhere */
A_Printf_format(1,2)
void DPRINT_TO_LOG(const char *format/*!=NULL,'\0'-terminated*/, ...);

#define DBGPRINT3_1(f) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC)
#define DBGPRINT3_2(f,...) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC, __VA_ARGS__)
#define DBGPRINT3x1(d_file_,d_line_,d_func_,f) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, d_file_, d_line_, d_func_)
#define DBGPRINT3x2(d_file_,d_line_,d_func_,f,...) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, d_file_, d_line_, d_func_, __VA_ARGS__)

#elif defined DPRINT_TO_STREAM

#define DBGPRINT3_1(f) \
	((void)fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC))
#define DBGPRINT3_2(f,...) \
	((void)fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC, __VA_ARGS__))
#define DBGPRINT3x1(d_file_,d_line_,d_func_,f) \
	((void)fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, d_file_, d_line_, d_func_))
#define DBGPRINT3x2(d_file_,d_line_,d_func_,f,...) \
	((void)fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, d_file_, d_line_, d_func_, __VA_ARGS__))

#endif /* DPRINT_TO_STREAM */

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STREAM

/* check if number of arguments is 2 or more, works for maximum 32 arguments */
#define DPRN_ARGS2(a1,a2,a3,a4,a5,a6,a7,a8,a9,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,d0,d1,d2,n,...) n
#define DPRN_ARGS1(args)    DPRN_ARGS2 args
#define DPRN_ARGS(...)      DPRN_ARGS1((__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0))

#define DBGPRINT3(X,N,args) DBGPRINT3##X##N args
#define DBGPRINT2(X,N,args) DBGPRINT3(X,N,args)
#define DBGPRINT1(X,N,args) DBGPRINT2(X,N,args)

/* add '\n' at end of format string, works for maximum 32 arguments */
#define DBGPRINT(...)                          DBGPRINT1(_,DPRN_ARGS(__VA_ARGS__),(__VA_ARGS__))
#define DBGPRINTX(d_file_,d_line_,d_func_,...) DBGPRINT1(x,DPRN_ARGS(__VA_ARGS__),(d_file_,d_line_,d_func_,__VA_ARGS__))

#else /* !DPRINT_TO_LOG && !DPRINT_TO_STREAM */

#define DBGPRINT(...)                          ((void)0)
#define DBGPRINTX(d_file_,d_line_,d_func_,...) ((void)(d_file_),(void)(d_line_),(void)(d_func_))

#endif /* !DPRINT_TO_LOG && !DPRINT_TO_STREAM */

#ifdef __cplusplus
}
#endif

#endif /* DPRINT_H_INCLUDED */
