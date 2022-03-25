#ifndef ASSERTS_H_INCLUDED
#define ASSERTS_H_INCLUDED

/**********************************************************************************
* Runtime assertions
* Copyright (C) 2012-2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* asserts.h */

/* defines:
  ASSERT(cond)
  ASSERT_PTR(ptr)
  DEBUG_CHECK(cond)
  DEBUG_CHECK_PTR(ptr)
*/

#ifndef NDEBUG
#include <stdlib.h> /* for exit() */
#include <assert.h> /* for assert() */
#endif
#include "dprint.h" /* for ASSUME(), DBGPRINTX(), annotations */

/* ASSERT(condition), ASSERT_PTR(pointer)

  if condition is false or pointer is NULL, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _unreachable_ code

  NOTE: condition must have no side-effects!
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

A_Noreturn_function
A_Force_inline_function
static void asserts_h_assertion_failed(
	const char *const cond,
	const char *const file,
	const int line,
	const char *const function)
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

A_Const_function /* for calling this function from const-expressions */
A_Force_inline_function
static int asserts_h_assert(
	const int x,
	const char *const cond,
	const char *const file,
	const int line,
	const char *const function)
{
	if (x)
		asserts_h_assertion_failed(cond, file, line, function);
	return 0;
}

#define ASSERT(cond) ((void)asserts_h_assert(!(cond), #cond, __FILE__, __LINE__, DPRINT_FUNC))

#ifndef ASSERT_PTR

/* to avoid "-Wnonnull-compare", don't mark ptr as non-null */
#ifdef __cplusplus
template <typename T>
#endif
A_Const_function /* for calling this function from const-expressions */
A_Force_inline_function
static int asserts_h_assert_ptr(
#ifdef __cplusplus
	T const ptr,
#else
	const void *const ptr,
#endif
	const char *const cond,
	const char *const file,
	const int line,
	const char *const function)
{
	if (!ptr)
		asserts_h_assertion_failed(cond, file, line, function);
	return 0;
}

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
/* suppress warning if converting pointer to a function to void* */
#define ASSERT_PTR(ptr) do {                                                \
  _Pragma("GCC diagnostic push")                                            \
  _Pragma("GCC diagnostic ignored \"-Wpedantic\"")                          \
  ((void)asserts_h_assert_ptr(ptr, #ptr, __FILE__, __LINE__, DPRINT_FUNC)); \
  _Pragma("GCC diagnostic pop")                                             \
} while (0)
#endif
#endif

#ifndef ASSERT_PTR
#define ASSERT_PTR(ptr) ((void)asserts_h_assert_ptr(ptr, #ptr, __FILE__, __LINE__, DPRINT_FUNC))
#endif

#endif /* !ASSERT_PTR */

#else /* NDEBUG */

#define ASSERT(cond) ASSUME(cond)

#endif /* NDEBUG */
#endif /* !ASSERT */

#ifndef ASSERT_PTR
#define ASSERT_PTR(ptr) ASSERT(ptr)
#endif

/* DEBUG_CHECK(condition), DEBUG_CHECK_PTR(pointer)

  if condition is false or pointer is NULL, then:
  in DEBUG   - abnormal program exit
  in RELEASE - _reachable_ code, error must be processed appropriately

  NOTE: condition must have no side-effects!
*/
#ifndef DEBUG_CHECK
#ifndef NDEBUG
#define DEBUG_CHECK(cond) ASSERT(cond)
#ifndef DEBUG_CHECK_PTR
#define DEBUG_CHECK_PTR(ptr) ASSERT_PTR(ptr)
#endif
#else /* NDEBUG */
#define DEBUG_CHECK(cond) ((void)0) /* _must_ process an error on runtime if cond is false */
#endif /* NDEBUG */
#endif /* !DEBUG_CHECK */

#ifndef DEBUG_CHECK_PTR
#define DEBUG_CHECK_PTR(ptr) DEBUG_CHECK(ptr)
#endif

#endif /* ASSERTS_H_INCLUDED */
