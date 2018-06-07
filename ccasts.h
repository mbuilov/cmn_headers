#ifndef CCASTS_H_INCLUDED
#define CCASTS_H_INCLUDED

/**********************************************************************************
* C-casts
* Copyright (C) 2012-2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* ccasts.h */

/* defines macros:

  "pointer->pointer casts"

  CAST          | A*       -> B*
  CAST_CONSTANT | const A* -> const B*
  CONST_CAST    | const A* -> A*

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

/* for casting pointer to one type to pointer to another type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline void *c_cast_void_(void *p)
{
	return p;
}

/* for casting const pointer to one type to const pointer to another type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline const void *c_cast_constant_void_(const void *p)
{
	return p;
}

/* for casting const pointer to non-const one to the same type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline void *c_const_cast_void_(const void *p)
{
#if defined __GNUC__ && __GNUC__ > 4 || __GNUC__ == 4 2 4
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare" /* warning: nonnull argument 'p' compared to NULL */
#endif
	return (void*)p;
}

/* cast non-const pointer to one type to const/non-const pointer to another type:

  struct my_type1 *p;
  struct my_type2 *q = CAST(struct my_type2, p);
*/
/* compiler will generate a warning if 'ptr' is const */
#define CAST(type, ptr) ((type*)c_cast_void_(ptr))

/* cast const/non-const pointer to one type to const pointer to another type:

  const struct my_type1 *p;
  const struct my_type2 *q = CAST_CONSTANT(const struct my_type2, p);
*/
/* compiler will generate a warning if 'type' is non-const */
#define CAST_CONSTANT(type, ptr) ((type*)c_cast_constant_void_(ptr))

/* cast const pointer to non-const one for the same type:

  const struct my_type *p;
  struct my_type *q = CONST_CAST(struct my_type, p);
*/
/* compiler will generate a warning if type of 'ptr' is not 'type' */
#define CONST_CAST(type, ptr) ((type*)c_const_cast_void_((ptr) + 0*sizeof(((const type*)(const void*)(ptr) - (ptr)))))

A_Nonnull_all_args A_Const_function A_Check_return A_Ret_never_null A_Ret_range(==,(char*)p - offset)
static inline void *c_container_of_(
	A_Notnull A_At((char*)p - offset, A_Writable_bytes(offset)) const void *p/*!=NULL*/,
	size_t offset)
{
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare" /* warning: nonnull argument 'p' compared to NULL */
#endif
	ASSERT(p); /* p must be != NULL */
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
	return (char*)c_const_cast_void_(p) - offset;
}

A_Const_function A_Check_return A_When(!p, A_Ret_null) A_When(p, A_Ret_notnull A_Ret_range(==,(char*)p - offset))
static inline void *c_opt_container_of_(
	A_When(p, A_At((char*)p - offset, A_Writable_bytes(offset))) const void *p/*NULL?*/,
	size_t offset)
{
	return p ? (char*)c_const_cast_void_(p) - offset : NULL;
}

#ifdef __cplusplus
}
#endif

/* check that CONTAINER_OF() doesn't discard const qualifier */
#if defined __cplusplus && __cplusplus >= 201103L
#define c_check_constness_(ptr, member, x) ((x) + 0*sizeof(*(decltype(&(x)->member)*)0 = (ptr)))
#elif defined __GNUC__
#define c_check_constness_(ptr, member, x) ((x) + 0*sizeof(*(__typeof__(&(x)->member)*)0 = (ptr)))
#else
#define c_check_constness_(ptr, member, x) (x)
#endif

#if defined _MSC_VER && defined __cplusplus
/* if type is constant, msvc2017 C++ compiler crashes while evaluating standard offsetof() from <stddef.h> */
template <class T> struct c_casts_offsetof_1 {typedef T XT;};
#define c_casts_offsetof_(type, member) offsetof(c_casts_offsetof_1<type>::XT, member)
#else
#define c_casts_offsetof_(type, member) offsetof(type, member)
#endif

/* ptr must be != NULL and must point to member of allocated object */
/* NOTE: must specify const type for const ptr */
#define CONTAINER_OF(ptr/*!=NULL*/, type, member) \
	c_check_constness_(ptr, member, \
		((type*)c_container_of_(ptr, c_casts_offsetof_(type, member) + \
			0*sizeof((&((type*)c_const_cast_void_(ptr))->member - (ptr))))))

/* ptr may be NULL, else must point to member of allocated object; returns NULL if ptr is NULL */
/* NOTE: must specify const type for const ptr */
#define OPT_CONTAINER_OF(ptr/*NULL?*/, type, member) \
	c_check_constness_(ptr, member, \
		((type*)c_opt_container_of_(ptr, c_casts_offsetof_(type, member) + \
			0*sizeof((&((type*)c_const_cast_void_(ptr))->member - (ptr))))))

#endif /* CCASTS_H_INCLUDED */
