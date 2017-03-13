#ifndef ASSERTS_H_INCLUDED
#define ASSERTS_H_INCLUDED

/**********************************************************************************
* Assertions
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* asserts.h */

/* defines: ASSERT, DEBUG_CHECK, EMBED_ASSERT, STATIC_ASSERT */

#include <assert.h>
#include "dprint.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ASSERT(condition)

  if condition is false, then:
  in DEBUG   - abnormal program exit
  in RELEASE - unreachable code
*/

#ifdef _MSC_VER
#ifdef _DEBUG
#define ASSERT(cond) assert(cond)
#endif
#endif

#ifdef __clang_analyzer__
#define ASSERT(cond) ASSUME(cond)
#endif

#ifndef ASSERT

#ifdef _DEBUG

A_Noreturn_function A_Force_inline_function
static void _x__assertion_failed(A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	DBGPRINTX(file, line, function, "assertion failed: %s", cond);
	*(volatile int*)((char*)0 + fflush(stderr)) = 0; /* generate SIGSEGV */
	exit(-1);
}

static inline void _x__assert_(int x, A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	if (x)
		_x__assertion_failed(cond, file, line, function);
}

#define ASSERT(cond) _x__assert_(!(cond), #cond, __FILE__, __LINE__, __func__)

#else /* !_DEBUG */
#define ASSERT(cond) ASSUME(cond)
#endif /* !_DEBUG */

#endif /* !ASSERT */

/* DEBUG_CHECK(condition)

  if condition is false, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _reachable_ code, error must be processed appropriately
*/
#ifdef _DEBUG
#define DEBUG_CHECK(cond) ASSERT(cond)
#else
#define DEBUG_CHECK(cond) ((void)0) /* _must_ process an error on runtime if cond is false */
#endif

/* compile-time asserts:

  EMBED_ASSERT(condition)  - evaluates to 0 at compile-time, may be placed inside expression,
  STATIC_ASSERT(condition) - defines struct, cannot be placed inside expression.
*/

#define EMBED_ASSERT(expr) (0*sizeof(int[1-2*!(expr)]))

#ifdef STATIC_ASSERT
#undef STATIC_ASSERT
#endif

#define ___STATIC_ASSERT(expr,line) struct _static_assert_at_line_##line{int _a[1-2*!(expr)];}
#define __STATIC_ASSERT(expr,line) ___STATIC_ASSERT(expr,line)
#define STATIC_ASSERT(expr) __STATIC_ASSERT(expr,__LINE__)

#ifdef __cplusplus
}
#endif

#endif /* ASSERTS_H_INCLUDED */
