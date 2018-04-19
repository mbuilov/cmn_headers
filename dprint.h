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

#ifndef DPRINT_TO_STREAM
#ifndef NDEBUG
#define DPRINT_TO_STREAM stderr
#endif
#endif

#ifdef DPRINT_TO_STREAM
#include <stdio.h> /* for fprintf() */
#endif

#include "sal_defs.h" /* for A_Printf_format_at()/A_Printf_format_string */

#ifdef __cplusplus
extern "C" {
#endif

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STREAM

#ifdef DPRINT_SHOW_THREAD_ID

/* format specifier for DPRINT_GET_THREAD_ID */
#ifndef DPRINT_THREAD_ID_FORMAT
#define DPRINT_THREAD_ID_FORMAT "%llx"
#endif

#define DPRINT_THREAD_ID_SEP ":"

#ifndef DPRINT_GET_THREAD_ID

/* NOTE: #include <windows.h> or <pthread.h> before this file */
#ifdef WIN32
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

#ifndef DPRINT_FILE_LINE_FORMAT
#define DPRINT_FILE_LINE_FORMAT "%s:%d:"
#endif

/* "%llx:%s:%d:%s(): " */
#define DPRINT_LOCATION_FORMAT DPRINT_THREAD_ID_FORMAT DPRINT_THREAD_ID_SEP DPRINT_FILE_LINE_FORMAT DPRINT_FUNC_FORMAT " "

#endif /* DPRINT_TO_STREAM || DPRINT_TO_LOG */

/*    DPRINT_TO_LOG    -> print debug messages to log (file)
 else DPRINT_TO_STREAM -> print debug messages to standard stream (stderr or stdout)
 else                  -> don't print anything */

#ifdef DPRINT_TO_LOG

A_Printf_format_at(1,2)
void DPRINT_TO_LOG(A_In_z A_Printf_format_string const char *format, ...);

#define DBGPRINT3_1(f) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC)
#define DBGPRINT3_2(f,...) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC, __VA_ARGS__)
#define DBGPRINT3x1(d_file__,d_line__,d_func__,f) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, d_file__, d_line__, d_func__)
#define DBGPRINT3x2(d_file__,d_line__,d_func__,f,...) \
	DPRINT_TO_LOG(DPRINT_LOCATION_FORMAT f, DPRINT_GET_THREAD_ID, d_file__, d_line__, d_func__, __VA_ARGS__)

#elif defined DPRINT_TO_STREAM

#define DBGPRINT3_1(f) \
	fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC)
#define DBGPRINT3_2(f,...) \
	fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, __FILE__, __LINE__, DPRINT_FUNC, __VA_ARGS__)
#define DBGPRINT3x1(d_file__,d_line__,d_func__,f) \
	fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, d_file__, d_line__, d_func__)
#define DBGPRINT3x2(d_file__,d_line__,d_func__,f,...) \
	fprintf(DPRINT_TO_STREAM, DPRINT_LOCATION_FORMAT f "\n", DPRINT_GET_THREAD_ID, d_file__, d_line__, d_func__, __VA_ARGS__)

#endif /* DPRINT_TO_STREAM */

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STREAM

#define DPRN_ARGS2(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,n,...) n
#define DPRN_ARGS1(args)    DPRN_ARGS2 args
#define DPRN_ARGS(...)      DPRN_ARGS1((__VA_ARGS__,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0))
#define DBGPRINT3(X,N,args) DBGPRINT3##X##N args
#define DBGPRINT2(X,N,args) DBGPRINT3(X,N,args)
#define DBGPRINT1(X,N,args) DBGPRINT2(X,N,args)
/* add '\n' at end of format string, works for maximum 30 arguments */
#define DBGPRINT(...)                             DBGPRINT1(_,DPRN_ARGS(__VA_ARGS__),(__VA_ARGS__))
#define DBGPRINTX(d_file__,d_line__,d_func__,...) DBGPRINT1(x,DPRN_ARGS(__VA_ARGS__),(d_file__,d_line__,d_func__,__VA_ARGS__))

#else /* !DPRINT_TO_LOG && !DPRINT_TO_STREAM */

#define DBGPRINT(...)                             ((void)0)
#define DBGPRINTX(d_file__,d_line__,d_func__,...) ((void)(d_file__),(void)(d_line__),(void)(d_func__))

#endif /* !DPRINT_TO_LOG && !DPRINT_TO_STREAM */

#ifdef __cplusplus
}
#endif

#endif /* DPRINT_H_INCLUDED */
