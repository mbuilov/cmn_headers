#ifndef ANNOTATIONS_H_INCLUDED
#define ANNOTATIONS_H_INCLUDED

/**********************************************************************************
* Common annotations.
* Copyright (C) 2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* annotations.h */

#if defined _MSC_VER && _MSC_VER >= 1600 && !defined NO_SAL_ANNOTATIONS && defined _PREFAST_

/* printf-like format string:
  f - 1-based index of format string,
  v - 1-based index of va-args (0 for vprintf-family) */
#define A_Printf_format(f,v)          _At_(_Param_(f), _Printf_format_string_)

#else /* !_PREFAST_ */

#if (defined __GNUC__ && __GNUC__ >= 4) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Printf_format(f,v)          __attribute__ ((format(printf, f, v)))
#elif defined __has_attribute
#if __has_attribute(format)
#define A_Printf_format(f,v)          __attribute__ ((format(printf, f, v)))
#endif
#endif

#endif /* !_PREFAST_ */

#ifndef A_Printf_format
#define A_Printf_format(f,v)
#endif

/* A_Restrict - annotates pointer pointing to memory that is not writable via other pointers */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define A_Restrict restrict
#elif defined(_MSC_VER) && (_MSC_VER >= 1600)
#define A_Restrict __restrict
#elif defined(__GNUC__) && (__GNUC__ >= 3)
#define A_Restrict __restrict__
#elif defined(__clang__)
#define A_Restrict __restrict__
#else
#define A_Restrict
#endif

/* A_Const_function - function do not reads or writes memory */

#if defined __cplusplus && __cplusplus >= 201402L
#define A_Const_function                         constexpr
#elif (defined __GNUC__ && __GNUC__ >= 4) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Const_function                         __attribute__ ((const))
#elif defined __has_attribute
#if __has_attribute(const)
#define A_Const_function                         __attribute__ ((const))
#endif
#endif

#ifndef A_Const_function
#define A_Const_function
#endif

/* A_Read_function - function may read, but cannot write memory */

#if (defined __GNUC__ && __GNUC__ >= 4) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Read_function                          __attribute__ ((pure))
#elif defined __has_attribute
#if __has_attribute(pure)
#define A_Read_function                          __attribute__ ((pure))
#endif
#endif

#ifndef A_Read_function
#define A_Read_function
#endif

/* A_Noreturn_function - note: for gcc, must be specified before other attributes */

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#define A_Noreturn_function                      _Noreturn /* standard keyword for c11 */
#elif defined __cplusplus && defined __has_cpp_attribute
#if __has_cpp_attribute(noreturn)
#define A_Noreturn_function [[noreturn]]
#endif
#elif defined __has_c_attribute
#if __has_c_attribute(noreturn)
#define A_Noreturn_function [[noreturn]]
#endif
#endif

#ifndef A_Noreturn_function
#ifdef _MSC_VER
#define A_Noreturn_function                      __declspec(noreturn)
#elif (defined __GNUC__ && __GNUC__ >= 4) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Noreturn_function                      __attribute__ ((noreturn))
#elif defined __has_attribute
#if __has_attribute(noreturn)
#define A_Noreturn_function                      __attribute__ ((noreturn))
#endif
#endif
#endif

#ifndef A_Noreturn_function
#define A_Noreturn_function                      /* function that never returns - calls exit() */
#endif

/* A_Force_inline_function, A_Non_inline_function */

#ifdef _MSC_VER
#define A_Force_inline_function                  __forceinline
#define A_Non_inline_function                    __declspec(noinline)
#elif (defined __GNUC__ && __GNUC__ >= 4) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Force_inline_function                  __attribute__ ((always_inline)) inline
#define A_Non_inline_function                    __attribute__ ((noinline))
#elif defined __has_attribute
#if __has_attribute(always_inline)
#define A_Force_inline_function                  __attribute__ ((always_inline)) inline
#endif
#if __has_attribute(noinline)
#define A_Non_inline_function                    __attribute__ ((noinline))
#endif
#endif

#ifndef A_Force_inline_function
#define A_Force_inline_function inline           /* forcibly inlined function */
#endif

#ifndef A_Non_inline_function
#define A_Non_inline_function                    /* forbid inlining a function */
#endif

/* A_Hot_function */

#if (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 3)) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Hot_function                           __attribute__ ((hot))
#elif defined __has_attribute
#if __has_attribute(hot)
#define A_Hot_function                           __attribute__ ((hot))
#endif
#endif

#ifndef A_Hot_function
#define A_Hot_function                           /* declare 'hot' function, which is called frequently and is optimized for speed */
#endif

/* A_Cold_function */

#if (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 3)) || \
  (defined __clang__ && __clang_major__ > 3 - (__clang_minor__ >= 7))
#define A_Cold_function                          __attribute__ ((hot))
#elif defined __has_attribute
#if __has_attribute(hot)
#define A_Cold_function                          __attribute__ ((hot))
#endif
#endif

#ifndef A_Cold_function
#define A_Cold_function                          /* declare 'cold' function, which is called infrequently and is optimized for size */
#endif

/* ASSUME */
#ifndef ASSUME
#if defined _MSC_VER
#define ASSUME(cond) __assume(!!(cond))
#elif defined __clang_analyzer__
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#elif defined __clang__
#define ASSUME(cond) __builtin_assume(!!(cond))
#elif defined __INTEL_COMPILER
#define ASSUME(cond) ((void)0) /* ICC compiles calls to __builtin_unreachable() as jumps to somewhere... */
#elif defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 5)
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#else
#define ASSUME(cond) ((void)0) /* assume condition is always true */
#endif
#endif

#endif /* ANNOTATIONS_H_INCLUDED */
