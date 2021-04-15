#ifndef STATIC_ASSERTS_H_INCLUDED
#define STATIC_ASSERTS_H_INCLUDED

/**********************************************************************************
* Compile-time assertions
* Copyright (C) 2012-2019,2021 Michael M. Builov, https://github.com/mbuilov/cmn_headers
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
#define STATIC_EXPR(const_expr) (                                   \
	__pragma(warning(push))                                         \
	__pragma(warning(disable:4296)) /* expression is always true */ \
	(const_expr)                                                    \
	__pragma(warning(pop)))
#else /* !_MSC_VER */
#define STATIC_EXPR(const_expr) (const_expr)
#endif /* !_MSC_VER */
#endif

/* note: use bit-field of negative size instead of array of negative size
  - to make sure that expression is compile-time defined; c99 allows to
  specify arrays with run-time defined bounds */
#ifndef EMBED_ASSERT
#define EMBED_ASSERT3(e,l,n) \
	(0*sizeof(struct embed_assert_##n##_at_line_##l { \
		unsigned int f_##n##_at_line_##l: 1-2*!STATIC_EXPR(e); \
	}*))
#define EMBED_ASSERT2(e,l,n) EMBED_ASSERT3(e,l,n)
#define EMBED_ASSERT1(e,n)   EMBED_ASSERT2(e,__LINE__,n)
#define EMBED_ASSERT(cexpr)  EMBED_ASSERT1(cexpr,0)
#endif

/* note: use bit-field of negative size instead of array of negative size
  - to make sure that expression is compile-time defined; c99 allows to
  specify arrays with run-time defined bounds */
#ifndef STATIC_ASSERT
#ifdef __GNUC__
#define STATIC_ASSERT3(e,l,n) \
	typedef __attribute__ ((unused)) struct static_assert_##n##_at_line_##l { \
		unsigned int f_##n##_at_line_##l: 1-2*!STATIC_EXPR(e); \
	} static_assert_##n##_at_line_##l##_t
#else /* !__GNUC__ */
#define STATIC_ASSERT3(e,l,n) \
	typedef struct static_assert_##n##_at_line_##l { \
		unsigned int f_##n##_at_line_##l: 1-2*!STATIC_EXPR(e); \
	} static_assert_##n##_at_line_##l##_t
#endif /* !__GNUC__ */
#define STATIC_ASSERT2(e,l,n) STATIC_ASSERT3(e,l,n)
#define STATIC_ASSERT1(e,n)   STATIC_ASSERT2(e,__LINE__,n)
#define STATIC_ASSERT(cexpr)  STATIC_ASSERT1(cexpr,0)
#endif

#endif /* STATIC_ASSERTS_H_INCLUDED */
