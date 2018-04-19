#ifndef ASSERTS_H_INCLUDED
#define ASSERTS_H_INCLUDED

/**********************************************************************************
* Assertions
* Copyright (C) 2012-2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* asserts.h */

/* defines: ASSERT, DEBUG_CHECK, EMBED_ASSERT, STATIC_ASSERT, COUNT_OF */

#ifndef NDEBUG
#include <assert.h>
#endif
#include "dprint.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ASSERT(condition)

  if condition is false, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _unreachable_ code
*/

#ifndef ASSERT
#ifndef NDEBUG
#ifdef _MSC_VER
#ifdef _DEBUG
#define ASSERT(cond) assert(cond)
#endif
#endif
#endif
#endif

#ifndef ASSERT
#ifdef __clang_analyzer__
#define ASSERT(cond) ASSUME(cond)
#endif
#endif

#ifndef ASSERT

#ifndef NDEBUG

A_Noreturn_function A_Force_inline_function
static void asserts_h__assertion_failed(A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	DBGPRINTX(file, line, function, "assertion failed: %s", cond);
	*(volatile int*)((char*)0 + fflush(stderr)) = 0; /* generate SIGSEGV */
	exit(-1);
}

static inline void asserts_h__assert(int x, A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	if (x)
		asserts_h__assertion_failed(cond, file, line, function);
}

#define ASSERT(cond) asserts_h__assert(!(cond), #cond, __FILE__, __LINE__, __func__)

#else /* !NDEBUG */

#define ASSERT(cond) ASSUME(cond)

#endif /* !NDEBUG */

#endif /* !ASSERT */

/* DEBUG_CHECK(condition)

  if condition is false, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _reachable_ code, error must be processed appropriately
*/
#ifndef DEBUG_CHECK
#ifndef NDEBUG
#define DEBUG_CHECK(cond) ASSERT(cond)
#else
#define DEBUG_CHECK(cond) ((void)0) /* _must_ process an error on runtime if cond is false */
#endif
#endif

/* compile-time asserts:

  EMBED_ASSERT(condition)  - evaluates to 0 at compile-time, may be placed inside an expression,
  STATIC_ASSERT(condition) - defines struct, cannot be placed inside expressions.
*/

#ifndef EMBED_ASSERT
#define EMBED_ASSERT(expr) (0*sizeof(int[1-2*!(expr)]))
#endif

/* note: use ##line for the unique structure name */
#ifndef STATIC_ASSERT
#define STATIC_ASSERT2(expr,line) struct asserts_h__static_assert_at_line_##line{int a_[1-2*!(expr)];}
#define STATIC_ASSERT1(expr,line) STATIC_ASSERT2(expr,line)
#define STATIC_ASSERT(expr)       STATIC_ASSERT1(expr,__LINE__)
#endif

/* number of elements in static array:

  int arr[10];
  size_t count = COUNT_OF(arr); // 10
*/

#ifndef COUNT_OF
#ifdef __GNUC__
#define COUNT_OF_(arr) (sizeof(arr)/sizeof((arr)[0]))
/* check that arr - is an array, not a pointer */
#define COUNT_OF(arr)  (COUNT_OF_(arr) + 0*sizeof(&(arr) - (__typeof__((arr)[0])(*)[COUNT_OF_(arr)])0))
#elif defined __cplusplus
template <typename T, size_t N> char (&COUNT_OF_(T (&array)[N]))[N];
#define COUNT_OF(arr) (sizeof(COUNT_OF_(arr)))
#else
#define COUNT_OF(arr) (sizeof(arr)/sizeof((arr)[0]))
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* ASSERTS_H_INCLUDED */
