#ifndef CCASTS_H_INCLUDED
#define CCASTS_H_INCLUDED

/**********************************************************************************
* C-casts
* Copyright (C) 2012-2019 Michael M. Builov, https://github.com/mbuilov/cmn_headers
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

/* used in place of void* */
struct c_casts_void_;

/* for casting pointer to one type -> pointer to another type */
A_Const_function
A_Force_inline_function
A_Check_return
A_Ret_range(==,p)
static struct c_casts_void_ *c_cast_void_(void *p)
{
	return (struct c_casts_void_*)p;
}

/* for casting pointer to one const type -> pointer to another const type */
A_Const_function
A_Force_inline_function
A_Check_return A_Ret_range(==,p)
static const struct c_casts_void_ *c_cast_constant_void_(const void *p)
{
	return (const struct c_casts_void_*)p;
}

/* for removing 'constness' of the pointed type */
A_Const_function
A_Force_inline_function
A_Check_return
A_Ret_range(==,p)
static struct c_casts_void_ *c_const_cast_void_(const void *p)
{
#if defined __clang__ || (defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual" /* warning: cast from type 'const void*' to type 'void*' casts away qualifiers */
#endif
	return (struct c_casts_void_*)p;
#if defined __clang__ || (defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic pop
#endif
}

/* cast pointer to one non-const type -> pointer to another const/non-const type:

  struct my_type1 *p;
  struct my_type2 *q = CAST(struct my_type2, p);
*/
/* compiler will generate a warning if 'ptr' points to const type */
#define CAST(type_, ptr_) ((type_*)c_cast_void_(ptr_))

/* cast pointer to one const/non-const type -> pointer to another const type:

  const struct my_type1 *p;
  const struct my_type2 *q = CAST_CONSTANT(const struct my_type2, p);
*/
/* compiler will generate a warning if 'type' is non-const */
#define CAST_CONSTANT(type_, ptr_) ((type_*)c_cast_constant_void_(ptr_))

/* remove 'constness' of the pointed type:

  const struct my_type *p;
  struct my_type *q = CONST_CAST(struct my_type, p);
*/
/* compiler will generate a warning if type of 'ptr' is not 'type' */
#define CONST_CAST(type_, ptr_) \
  ((void)sizeof(((const type_*)(const void*)(ptr_) - (ptr_))), (type_*)c_const_cast_void_(ptr_))

/* cast pointer to non-const pointer to one const/non-const type -> pointer to non-const pointer to another const type:

  const struct my_type1 **pp;
  const struct my_type2 **qq = CAST_PPC(const struct my_type2, pp);
*/
/* compiler will generate an error if 'pp' is not a pointer (to pointer) */
#ifndef _MSC_VER
/* compiler will generate a warning if 'type' is non-const */
/* compiler will generate a warning if 'pp' points to const pointer */
#define CAST_PPC(type_, pp_) ((void)sizeof((type_*)c_cast_constant_void_(*(pp_))), (type_**)c_cast_void_(pp_))
#else
/* C++ compiler will generate an error if 'type' is non-const */
#define CAST_PPC(type_, pp_) ((void)sizeof(c_cast_constant_void_(*(pp_))), (type_**)c_const_cast_void_(pp_))
#endif

/* cast pointer to non-const pointer to some type -> pointer to given non-const type:

  const struct my_type1 **pp;
  struct my_type2 *qq = CAST_PP(struct my_type2, pp);
*/
/* compiler will generate an error if 'pp' is not a pointer (to pointer) */
#ifndef _MSC_VER
/* compiler will generate a warning if 'pp' points to const pointer */
#define CAST_PP(type_, pp_) ((void)sizeof(c_cast_constant_void_(*(pp_))), CAST(type_, pp_))
#else
#define CAST_PP(type_, pp_) ((void)sizeof(c_cast_constant_void_(*(pp_))), (type_*)c_const_cast_void_(pp_))
#endif

A_Nonnull_all_args
A_Const_function
A_Force_inline_function
A_Check_return
A_Ret_never_null
A_Ret_range(==,(char*)p - offset)
A_Ret_valid
static struct c_casts_void_ *c_container_of_(
	A_In A_At((char*)p - offset, A_Writable_bytes(offset)) const void *p/*!=NULL*/,
	size_t offset/*0?*/)
{
	ASSERT_PTR(p); /* p must be != NULL */
	return (struct c_casts_void_*)((char*)c_const_cast_void_(p) - offset);
}

A_Const_function
A_Force_inline_function
A_Check_return
A_When(!p, A_Ret_null)
A_When(!!p, A_Ret_range(==,(char*)p - offset) A_Ret_valid)
static struct c_casts_void_ *c_opt_container_of_(
	A_In_opt A_When(!!p, A_At((char*)p - offset, A_Writable_bytes(offset))) const void *p/*NULL?*/,
	size_t offset/*0?*/)
{
	return (struct c_casts_void_*)(p ? (char*)c_const_cast_void_(p) - offset : NULL);
}

#ifdef __cplusplus
}
#endif

/* check that CONTAINER_OF() doesn't discard const qualifier */
#if defined __cplusplus && __cplusplus >= 201103L
#define c_check_constness_(ptr_, member_, x_) ((void)sizeof(*(decltype(&(x_)->member_)*)0 = (ptr_)), (x_))
#elif defined __GNUC__
#define c_check_constness_(ptr_, member_, x_) ((void)sizeof(*(__typeof__(&(x_)->member_)*)0 = (ptr_)), (x_))
#else
#define c_check_constness_(ptr_, member_, x_) (x_)
#endif

#if defined _MSC_VER && defined __cplusplus
/* if type is constant, msvc2017 C++ compiler crashes while evaluating standard offsetof() from <stddef.h> */
template <class T> struct c_casts_offsetof_1 {typedef T XT;};
#define c_casts_offsetof_(type_, member_) offsetof(c_casts_offsetof_1<type_>::XT, member_)
#else
#define c_casts_offsetof_(type_, member_) offsetof(type_, member_)
#endif

/* ptr must be != NULL and must point to member of allocated object */
/* NOTE: must specify const type for const ptr */
#define CONTAINER_OF(ptr_/*!=NULL*/, type_, member_) \
	c_check_constness_(ptr_, member_, \
		((type_*)c_container_of_(ptr_, c_casts_offsetof_(type_, member_) + \
			0*sizeof((&((type_*)c_const_cast_void_(ptr_))->member_ - (ptr_))))))

/* ptr may be NULL, else must point to member of allocated object; returns NULL if ptr is NULL */
/* NOTE: must specify const type for const ptr */
#define OPT_CONTAINER_OF(ptr_/*NULL?*/, type_, member_) \
	c_check_constness_(ptr_, member_, \
		((type_*)c_opt_container_of_(ptr_, c_casts_offsetof_(type_, member_) + \
			0*sizeof((&((type_*)c_const_cast_void_(ptr_))->member_ - (ptr_))))))

#endif /* CCASTS_H_INCLUDED */
