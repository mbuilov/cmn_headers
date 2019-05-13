#ifndef STATIC_ASSERTS_H_INCLUDED
#define STATIC_ASSERTS_H_INCLUDED

/**********************************************************************************
* Compile-time assertions
* Copyright (C) 2012-2019 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* static_asserts.h */

/* defines:
  STATIC_EXPR(const_expr)   - disable warnings for a constant compile-time expression,
  EMBED_ASSERT(const_expr)  - evaluates to 0 at compile-time, may be placed inside an expression,
  STATIC_ASSERT(const_expr) - defines typedef, cannot be placed inside expressions.
*/

#ifndef STATIC_EXPR
#ifdef _MSC_VER
#define STATIC_EXPR(const_expr) ( \
	__pragma(warning(push))                                         \
	__pragma(warning(disable:4296)) /* expression is always true */ \
	(const_expr)                                                          \
	__pragma(warning(pop)))
#else /* !_MSC_VER */
#define STATIC_EXPR(const_expr) (const_expr)
#endif /* !_MSC_VER */
#endif

#ifndef EMBED_ASSERT
#define EMBED_ASSERT(const_expr) (0*sizeof(int[1-2*!STATIC_EXPR(const_expr)]))
#endif

/* note: use ##line for the unique name */
#ifndef STATIC_ASSERT
#ifdef __COUNTER__
#ifdef __GNUC__
#define STATIC_ASSERT2(e,l,n) typedef __attribute__ ((unused)) int static_assert_##n##_at_line_##l[1-2*!STATIC_EXPR(e)]
#else /* !__GNUC__ */
#define STATIC_ASSERT2(e,l,n) typedef int static_assert_##n##_at_line_##l[1-2*!STATIC_EXPR(e)]
#endif /* !__GNUC__ */
#define STATIC_ASSERT1(e,l,n) STATIC_ASSERT2(e,l,n)
#define STATIC_ASSERT(cexpr)  STATIC_ASSERT1(cexpr,__LINE__,__COUNTER__)
#else /* !__COUNTER__ */
#ifdef __GNUC__
#define STATIC_ASSERT2(e,l) typedef __attribute__ ((unused)) int static_assert_at_line_##l[1-2*!STATIC_EXPR(e)]
#else /* !__GNUC__ */
#define STATIC_ASSERT2(e,l) typedef int static_assert_at_line_##l[1-2*!STATIC_EXPR(e)]
#endif /* !__GNUC__ */
#define STATIC_ASSERT1(e,l)  STATIC_ASSERT2(e,l)
#define STATIC_ASSERT(cexpr) STATIC_ASSERT1(cexpr,__LINE__)
#endif /* !__COUNTER__ */
#endif

#endif /* STATIC_ASSERTS_H_INCLUDED */
