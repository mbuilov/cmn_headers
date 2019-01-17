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
#include <stdlib.h> /* for assert() */
#include <assert.h> /* for exit() */
#endif
#include "dprint.h" /* for DBGPRINTX() */

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

#ifdef __cplusplus
extern "C" {
#endif

A_Noreturn_function A_Force_inline_function
static void asserts_h_assertion_failed(A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	volatile int *arr[1] = {NULL};
	DBGPRINTX(file, line, function, "assertion failed: %s", cond);
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable:6011) /* Dereferencing NULL pointer 'arr[0]' */
#elif defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
	*arr[0] = fflush(stderr); /* generate SIGSEGV */
#if defined _MSC_VER
#pragma warning(pop)
#elif defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
	exit(-1);
}

static inline void asserts_h_assert(int x, A_In_z const char *cond, A_In_z const char *file, int line, A_In_z const char *function)
{
	if (x)
		asserts_h_assertion_failed(cond, file, line, function);
}

#ifdef __cplusplus
}
#endif

#define ASSERT(cond) asserts_h_assert(!(cond), #cond, __FILE__, __LINE__, DPRINT_FUNC)

#else /* NDEBUG */

#define ASSERT(cond) ASSUME(cond)

#endif /* NDEBUG */

#endif /* !ASSERT */

/* DEBUG_CHECK(condition)

  if condition is false, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _reachable_ code, error must be processed appropriately
*/
#ifndef DEBUG_CHECK
#ifndef NDEBUG
#define DEBUG_CHECK(cond) ASSERT(cond)
#else /* NDEBUG */
#define DEBUG_CHECK(cond) ((void)0) /* _must_ process an error on runtime if cond is false */
#endif /* NDEBUG */
#endif

/* compile-time asserts:

  EMBED_ASSERT(condition)  - evaluates to 0 at compile-time, may be placed inside an expression,
  STATIC_ASSERT(condition) - defines struct, cannot be placed inside expressions.
*/

#ifndef EMBED_ASSERT
#define EMBED_ASSERT(expr) (0*sizeof(int[1-2*!(expr)]))
#endif

/* note: use ##line for the unique name */
#ifndef STATIC_ASSERT
#ifdef __COUNTER__
#ifdef __GNUC__
#define STATIC_ASSERT2(expr,line,counter) typedef __attribute__ ((unused)) int static_assert_##counter##_at_line_##line[1-2*!(expr)]
#else /* !__GNUC__ */
#define STATIC_ASSERT2(expr,line,counter) typedef int static_assert_##counter##_at_line_##line[1-2*!(expr)]
#endif /* !__GNUC__ */
#define STATIC_ASSERT1(expr,line,counter) STATIC_ASSERT2(expr,line,counter)
#define STATIC_ASSERT(expr)               STATIC_ASSERT1(expr,__LINE__,__COUNTER__)
#else /* !__COUNTER__ */
#ifdef __GNUC__
#define STATIC_ASSERT2(expr,line) typedef __attribute__ ((unused)) int static_assert_at_line_##line[1-2*!(expr)]
#else /* !__GNUC__ */
#define STATIC_ASSERT2(expr,line) typedef int static_assert_at_line_##line[1-2*!(expr)]
#endif /* !__GNUC__ */
#define STATIC_ASSERT1(expr,line) STATIC_ASSERT2(expr,line)
#define STATIC_ASSERT(expr)       STATIC_ASSERT1(expr,__LINE__)
#endif /* !__COUNTER__ */
#endif

/* number of elements in static array:

  int arr[10];
  size_t count = COUNT_OF(arr); // 10
*/

#ifndef COUNT_OF
#ifdef __cplusplus
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtemplates"
#endif
template <typename T, size_t N> char (&COUNT_OF_(T (&arr)[N]))[N];
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
#define COUNT_OF(arr) sizeof(COUNT_OF_(arr))
#elif defined __GNUC__
#define COUNT_OF_(arr) (sizeof(arr)/sizeof((arr)[0]))
/* check that arr - is an array, not just a pointer */
#define COUNT_OF(arr)  (COUNT_OF_(arr) + 0*sizeof(&(arr) - (__typeof__((arr)[0])(*)[COUNT_OF_(arr)])0))
#else /* fallback */
#define COUNT_OF(arr) (sizeof(arr)/sizeof((arr)[0]))
#endif
#endif /* !COUNT_OF */

#endif /* ASSERTS_H_INCLUDED */
