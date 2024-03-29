#ifndef CCASTS_H_INCLUDED
#define CCASTS_H_INCLUDED

/**********************************************************************************
* C-casts
* Copyright (C) 2012-2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* ccasts.h */

/* defines macros:

  "pointer->pointer casts"

  CAST          | A*        -> B*
  CAST_CONSTANT | const A*  -> const B*
  CONST_CAST    | const A*  -> A*
  CAST_PPC      | const A** -> const B**
  CAST_PP       | const A** -> B*, const A*** -> B*, const A*const ** -> B*, ...

  "member->container casts"

  CONTAINER_OF:
   NULL     -> assert
   M*       -> C*
   const M* -> const C*

  OPT_CONTAINER_OF:
   NULL     -> NULL
   M*       -> C*
   const M* -> const C*
*/

#include <stddef.h>
#include "asserts.h"

#ifdef __cplusplus
extern "C" {
#endif

/* helper type - used in place of void* */
struct c_casts_void_;

/* helper for casting:
  A* -> B*
*/
A_Const_function
A_Force_inline_function
static struct c_casts_void_ *c_cast_void_(void *const p)
{
	return (struct c_casts_void_*)p;
}

/* helper for casting:
  const A** -> B*
*/
/* note: this function is needed for MSVC */
A_Const_function
A_Force_inline_function
static struct c_casts_void_ *c_cast_ppc_void_(const void **const p)
{
	void *const v = (void*)p;
	return (struct c_casts_void_*)v;
}

/* helper for casting:
  const A* -> const B*
*/
A_Const_function
A_Force_inline_function
static const struct c_casts_void_ *c_cast_constant_void_(const void *const p)
{
	const void *const v = (void*)p;
	return (const struct c_casts_void_*)v;
}

/* helper for casting:
  const A* -> B*
*/
/* note: remove 'constness' of the pointed type */
A_Const_function
A_Force_inline_function
static struct c_casts_void_ *c_const_cast_void_(const void *const p)
{
#if defined __clang__ || (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual" /* cast from type 'const void*' to type 'void*' casts away qualifiers */
#endif
	return (struct c_casts_void_*)p;
#if defined __clang__ || (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#endif
}

/* helper for obtaining constant version of a given (const?) type:
  c_casts_const_type<A>       -> const A
  c_casts_const_type<const A> -> const A
*/
#ifdef __cplusplus
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtemplates"
#endif
extern "C++" template <class T> struct c_casts_const_type {typedef const T XT;};
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
#endif

/* CAST macro
  cast pointer to one NON-CONST Type -> pointer to another const/non-const type:

  struct A *p;
  struct B *q = CAST(struct B, p);

  !note: casted pointer must not point to a constant:

  const struct A *p;
  CAST(?, p);        - error: 'p' points to CONST
*/
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#define CAST(type_, ptr_) __extension__({                    \
  _Pragma("GCC diagnostic push")                             \
  _Pragma("GCC diagnostic error \"-Wdiscarded-qualifiers\"") \
  type_ *const p_ = (type_*)c_cast_void_(ptr_);              \
  _Pragma("GCC diagnostic pop")                              \
  p_;                                                        \
})
#endif
#endif

#ifndef CAST
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#ifdef __clang__
#define CAST(type_, ptr_) __extension__({                            \
  _Pragma("clang diagnostic push")                                   \
  _Pragma("clang diagnostic error \"-Wincompatible-pointer-types\"") \
  type_ *const p_ = (type_*)c_cast_void_(ptr_);                      \
  _Pragma("clang diagnostic pop")                                    \
  p_;                                                                \
})
#endif
#endif
#endif

#ifndef CAST
#if !defined __cplusplus && defined _MSC_VER
#define CAST(type_, ptr_) (                                                 \
  __pragma(warning(push))                                                   \
  __pragma(warning(error:4090))/*'function': different 'const' qualifiers*/ \
  (type_*)c_cast_void_(/*check that 'ptr' points to non-const*/ptr_)        \
  __pragma(warning(pop))                                                    \
)
#endif
#endif

#ifndef CAST
#define CAST(type_, ptr_) ((type_*)c_cast_void_(/*check that 'ptr' points to non-const*/ptr_))
#endif

/* CAST_CONSTANT macro
  cast pointer to one const/non-const type -> pointer to another CONST type:

  const struct A *p;
  const struct B *q = CAST_CONSTANT(const struct B, p);

  !note: the type to cast to must be const:

  const struct A *p;
  CAST_CONSTANT(struct B, p);  - error: 'struct B' is NON-CONST
*/
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __clang__ || (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6))
#define CAST_CONSTANT(type_, ptr_) __extension__({       \
  _Pragma("GCC diagnostic push")                         \
  _Pragma("GCC diagnostic error \"-Wcast-qual\"")        \
  type_ *const p_ = (type_*)c_cast_constant_void_(ptr_); \
  _Pragma("GCC diagnostic pop")                          \
  p_;                                                    \
})
#endif
#endif

#ifdef __cplusplus
#define CAST_CONSTANT(type_, ptr_) (                               \
  /*check that 'type' is const*/                                   \
  (void)sizeof(*(type_**)0 = *(c_casts_const_type<type_>::XT**)0), \
  ((type_*)c_cast_constant_void_(ptr_))                            \
)
#endif

#ifndef CAST_CONSTANT
#ifdef _MSC_VER
#define CAST_CONSTANT(type_, ptr_) (                                         \
  __pragma(warning(push))                                                    \
  __pragma(warning(disable:4114))/*same type qualifier used more than once*/ \
  __pragma(warning(error:4090))/*'=': different 'const' qualifiers*/         \
  (void)sizeof(*(type_**)0 = /*check that 'type' is const*/(const type_*)    \
    c_cast_constant_void_((void*)0))                                         \
  __pragma(warning(pop)),                                                    \
  (type_*)c_cast_constant_void_(ptr_))
#endif
#endif

#ifndef CAST_CONSTANT
#define CAST_CONSTANT(type_, ptr_) (          \
  (void)sizeof(/*check that 'type' is const*/ \
    (type_*)c_cast_constant_void_((void*)0)), \
  (type_*)c_cast_constant_void_(ptr_)         \
)
#endif

/* CONST_CAST macro
  remove 'constness' of the pointed type:

  const struct A *p;
  struct A *q = CONST_CAST(struct A, p);

  !note: the type to cast to:
  1) must be compatible with the type of the casted pointer,
  2) must be non-const:

  const struct A *p;
  CONST_CAST(struct B, p);        - error: 'struct B' is not compatible with 'struct A'
  CONST_CAST(const struct A, p);  - error: 'const struct A' is CONST
*/
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#define CONST_CAST(type_, ptr_) __extension__({                    \
  _Pragma("GCC diagnostic push")                                   \
  _Pragma("GCC diagnostic error \"-Wincompatible-pointer-types\"") \
  _Pragma("GCC diagnostic error \"-Wdiscarded-qualifiers\"")       \
  void *const x_ = c_const_cast_void_(ptr_);                       \
  type_ *const p_ = (type_*)x_;                                    \
  (void)sizeof(*(const type_**)0 = (ptr_));                        \
  (void)sizeof(c_cast_void_((type_*)0));                           \
  _Pragma("GCC diagnostic pop")                                    \
  p_;                                                              \
})
#endif
#endif

#ifndef CONST_CAST
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#ifdef __clang__
#define CONST_CAST(type_, ptr_) __extension__({                      \
  _Pragma("clang diagnostic push")                                   \
  _Pragma("clang diagnostic ignored \"-Wduplicate-decl-specifier\"") \
  _Pragma("clang diagnostic error \"-Wincompatible-pointer-types\"") \
  void *const x_ = c_const_cast_void_(ptr_);                         \
  type_ *const p_ = (type_*)x_;                                      \
  (void)sizeof(*(const type_**)0 = (ptr_));                          \
  (void)sizeof(c_cast_void_((type_*)0));                             \
  _Pragma("clang diagnostic pop")                                    \
  p_;                                                                \
})
#endif
#endif
#endif

#ifndef CONST_CAST
#if !defined __cplusplus && defined _MSC_VER
#define CONST_CAST(type_, ptr_) (                                                  \
  __pragma(warning(push))                                                          \
  __pragma(warning(disable:4114))/*same type qualifier used more than once*/       \
  __pragma(warning(error:4133))/*'=': incompatible types - from '... *' to '... */ \
  __pragma(warning(error:4090))/*'=': different 'const' qualifiers*/               \
  /*check that 'type' is compatible with the type of 'ptr'*/                       \
  (void)sizeof(*(const type_**)0 = (ptr_)),                                        \
  /*check that 'type' is non-const*/                                               \
  (void)sizeof(c_cast_void_((type_*)0))                                            \
  __pragma(warning(pop)),                                                          \
  (type_*)c_const_cast_void_(ptr_)                                                 \
)
#endif
#endif

#ifdef __cplusplus
#define CONST_CAST(type_, ptr_) (                                          \
  /*check that 'type' is compatible with the type of 'ptr'*/               \
  (void)sizeof(*(c_casts_const_type<type_>::XT**)0 = (ptr_)),              \
  /*check that 'type' is non-const*/(void)sizeof(c_cast_void_((type_*)0)), \
  (type_*)c_const_cast_void_(ptr_)                                         \
)
#endif

#ifndef CONST_CAST
#define CONST_CAST(type_, ptr_) (                                          \
  /*check that 'type' is compatible with the type of 'ptr'*/               \
  (void)sizeof(*(const type_**)0 = (ptr_)),                                \
  /*check that 'type' is non-const*/(void)sizeof(c_cast_void_((type_*)0)), \
  (type_*)c_const_cast_void_(ptr_)                                         \
)
#endif

/* CAST_PPC macro
  cast pointer to non-const pointer to one const/non-const type ->
    pointer to non-const pointer to another CONST type:

  const struct A **pp;
  const struct B **qq = CAST_PPC(const struct B, pp);

  !notes:
  1) casted pointer must be a pointer-to-non-const-pointer,
  2) the type to cast to must be const:

  const struct A *p;
  CAST_PPC(?, p);  - error: 'p' is not a pointer-to-pointer

  struct A *const *pp;
  CAST_PPC(?, pp);  - error: 'pp' is not a pointer-to-non-const-pointer

  struct A **pp;
  CAST_PPC(struct B, pp);  - error: 'struct B' is NON-CONST
*/
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#define CAST_PPC(type_, pp_) __extension__({                 \
  _Pragma("GCC diagnostic push")                             \
  _Pragma("GCC diagnostic error \"-Wcast-qual\"")            \
  _Pragma("GCC diagnostic error \"-Wdiscarded-qualifiers\"") \
  type_ **const ppc_ = (type_**)c_cast_void_(pp_);           \
  (void)sizeof((type_*)c_cast_constant_void_(*(pp_)));       \
  _Pragma("GCC diagnostic pop")                              \
  ppc_;                                                      \
})
#endif
#endif

#ifndef CAST_PPC
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#ifdef __clang__
#define CAST_PPC(type_, pp_) __extension__({                               \
  _Pragma("clang diagnostic push")                                         \
  _Pragma("clang diagnostic ignored \"-Wduplicate-decl-specifier\"")       \
  _Pragma("clang diagnostic error \"-Wincompatible-pointer-types\"")       \
  type_ **const ppc_ = (type_**)c_cast_void_(pp_);                         \
  (void)sizeof(*(type_**)0 = (const type_*)c_cast_constant_void_(*(pp_))); \
  _Pragma("clang diagnostic pop")                                          \
  ppc_;                                                                    \
})
#endif
#endif
#endif

#ifdef __cplusplus
#define CAST_PPC(type_, pp_) (                                                                   \
  /*check that 'pp' is a pointer-to-pointer*/(void)sizeof(c_cast_constant_void_(*(pp_))),        \
  /*check that 'type' is const*/(void)sizeof(*(type_**)0 = *(c_casts_const_type<type_>::XT**)0), \
  /*check that 'pp' is a pointer-to-non-const-pointer*/((type_**)c_cast_void_(pp_))              \
)
#endif

#ifndef CAST_PPC
#ifdef _MSC_VER
#define CAST_PPC(type_, pp_) (                                                                \
  __pragma(warning(push))                                                                     \
  __pragma(warning(disable:4114))/*same type qualifier used more than once*/                  \
  __pragma(warning(error:4090))/*'='/'function': different 'const' qualifiers*/               \
  /*check that 'pp' is a pointer-to-pointer*/(void)sizeof(                                    \
    *(type_**)0 = /*check that 'type' is const*/(const type_*)c_cast_constant_void_(*(pp_))), \
  /*check that 'pp' is a pointer-to-non-const-pointer*/(type_**)c_cast_ppc_void_(pp_)         \
  __pragma(warning(pop))                                                                      \
)
#endif
#endif

#ifndef CAST_PPC
#define CAST_PPC(type_, pp_) (                                                        \
  /*check that 'pp' is a pointer-to-pointer*/(void)sizeof(                            \
    /*check that 'type' is const*/(type_*)c_cast_constant_void_(*(pp_))),             \
  /*check that 'pp' is a pointer-to-non-const-pointer*/(type_**)c_cast_ppc_void_(pp_) \
)
#endif

/* CAST_PP (generic) macro
  cast pointer to non-const pointer to some const/non-const type -> pointer to a given const/non-const type:

  const struct A **pp;
  struct B *q = CAST_PP(struct B, pp);

  !note: casted pointer must be a pointer-to-non-const-pointer:

  const struct A *p;
  CAST_PP(?, p);  - error: 'p' is not a pointer-to-pointer

  struct A *const *pp;
  CAST_PP(?, pp);  - error: 'pp' is not a pointer-to-non-const-pointer
*/
#ifdef __cplusplus
#define CAST_PP(type_, pp_) (                                                             \
  /*check that 'pp' is a pointer-to-pointer*/(void)sizeof(c_cast_constant_void_(*(pp_))), \
  /*check that 'pp' is a pointer-to-non-const-pointer*/(type_*)c_cast_void_(pp_)          \
)
#endif

#ifndef CAST_PP
#ifdef _MSC_VER
#define CAST_PP(type_, pp_) (                                                                    \
  /*check that 'pp' is a pointer-to-pointer*/(void)sizeof(c_cast_constant_void_(*(pp_))),        \
  __pragma(warning(push))                                                                        \
  __pragma(warning(disable:4047))/*'const void **' differs in levels of indirection from '...'*/ \
  __pragma(warning(disable:4022))/*pointer mismatch for actual parameter 1*/                     \
  __pragma(warning(error:4090))/*'function': different 'const' qualifiers*/                      \
  /*check that 'pp' is a pointer-to-non-const-pointer*/(type_*)c_cast_ppc_void_(pp_)             \
  __pragma(warning(pop))                                                                         \
)
#endif
#endif

#ifndef CAST_PP
#define CAST_PP(type_, pp_) (                           \
  /*check that 'pp' is a pointer-to-pointer*/           \
  (void)sizeof(c_cast_constant_void_(*(pp_))),          \
  /*check that 'pp' is a pointer-to-non-const-pointer*/ \
  CAST(type_, pp_)                                      \
)
#endif

A_Const_function
A_Force_inline_function
static struct c_casts_void_ *c_container_of_(
	const void *const p/*!=NULL*/, const size_t offset/*0?*/)
{
	DEBUG_CHECK_PTR(p); /* p must be != NULL */
	{
		void *const x = c_const_cast_void_(p);
		void *const v = (char*)x - offset;
		return (struct c_casts_void_*)v;
	}
}

A_Const_function
A_Force_inline_function
static struct c_casts_void_ *c_opt_container_of_(
	const void *const p/*NULL?*/, const size_t offset/*0?*/)
{
	if (p) {
		void *const x = c_const_cast_void_(p);
		void *const v = (char*)x - offset;
		return (struct c_casts_void_*)v;
	}
	return (struct c_casts_void_*)0;
}

/* check that CONTAINER_OF() doesn't discard const qualifier */
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#define c_check_constness_(ptr_, member_, x_) __extension__({ \
  _Pragma("GCC diagnostic push")                              \
  _Pragma("GCC diagnostic error \"-Wdiscarded-qualifiers\"")  \
  (void)sizeof(*(__typeof__(&(x_)->member_)*)0 = (ptr_));     \
  _Pragma("GCC diagnostic pop")                               \
  x_;                                                         \
})
#endif
#endif

#ifndef c_check_constness_
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#ifdef __clang__
#define c_check_constness_(ptr_, member_, x_) __extension__({        \
  _Pragma("clang diagnostic push")                                   \
  _Pragma("clang diagnostic error \"-Wincompatible-pointer-types\"") \
  (void)sizeof(*(__typeof__(&(x_)->member_)*)0 = (ptr_));            \
  _Pragma("clang diagnostic pop")                                    \
  x_;                                                                \
})
#endif
#endif
#endif

#if defined __cplusplus && __cplusplus >= 201103L
#define c_check_constness_(ptr_, member_, x_) \
  ((void)sizeof(*(decltype(&(x_)->member_)*)0 = (ptr_)), (x_))
#endif

#ifndef c_check_constness_
#if defined __cplusplus && defined _MSC_VER && _MSC_VER >= 1900
#define c_check_constness_(ptr_, member_, x_) \
  ((void)sizeof(*(decltype(&(x_)->member_)*)0 = (ptr_)), (x_))
#endif
#endif

/* fallback: no checks */
#ifndef c_check_constness_
#define c_check_constness_(ptr_, member_, x_) (x_)
#endif

/* if type is constant, msvc2017 C++ compiler crashes while evaluating standard offsetof() from <stddef.h>,
  use template for a workaround */
#if defined _MSC_VER && defined __cplusplus
#define c_casts_offsetof_(type_, member_) offsetof(c_casts_const_type<type_>::XT, member_)
#endif

/* use standard offsetof macro */
#ifndef c_casts_offsetof_
#define c_casts_offsetof_(type_, member_) offsetof(type_, member_)
#endif

/* CONTAINER_OF macro
  get container by a non-NULL pointer to member:

  struct Container {
    struct Member memb;
  };
  struct Member *m;
  struct Container *c = CONTAINER_OF(m, struct Container, memb);

  !note: if pointer points to CONST member, passed container type must also be CONST:

  const struct Member *m;
  CONTAINER_OF(m, struct Container, memb);  - error: 'struct Container' is non-const while 'm' points to a const
*/
#define CONTAINER_OF1(ptr_/*!=NULL*/, type_, member_) \
	c_check_constness_(ptr_, member_, \
		((type_*)c_container_of_(ptr_, c_casts_offsetof_(type_, member_) + \
			0*sizeof((&((type_*)c_const_cast_void_(ptr_))->member_ - (ptr_))))))

/* OPT_CONTAINER_OF macro
  like CONTAINER_OF, but returns NULL if pointer is NULL */
#define OPT_CONTAINER_OF1(ptr_/*NULL?*/, type_, member_) \
	c_check_constness_(ptr_, member_, \
		((type_*)c_opt_container_of_(ptr_, c_casts_offsetof_(type_, member_) + \
			0*sizeof((&((type_*)c_const_cast_void_(ptr_))->member_ - (ptr_))))))

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#if defined __clang__ || (defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6))
#define CONTAINER_OF(ptr_/*!=NULL*/, type_, member_) __extension__({      \
	__typeof__(ptr_) p_ = (ptr_);                                         \
	CONTAINER_OF1(p_, type_, member_);                                    \
})
#define OPT_CONTAINER_OF(ptr_/*NULL?*/, type_, member_) __extension__({   \
	__typeof__(ptr_) p_ = (ptr_);                                         \
	OPT_CONTAINER_OF1(p_, type_, member_);                                \
})
#endif
#endif

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr_/*!=NULL*/, type_, member_) CONTAINER_OF1(ptr_, type_, member_)
#endif

#ifndef OPT_CONTAINER_OF
#define OPT_CONTAINER_OF(ptr_/*NULL?*/, type_, member_) OPT_CONTAINER_OF1(ptr_, type_, member_)
#endif

#ifdef __cplusplus
}
#endif

#endif /* CCASTS_H_INCLUDED */
