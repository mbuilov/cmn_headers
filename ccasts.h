#ifndef CCASTS_H_INCLUDED
#define CCASTS_H_INCLUDED

/********************************************************************************
* Different C-casts
* Public domain
********************************************************************************/

/* ccasts.h */

#include <stddef.h>
#include "asserts.h"

#ifdef __cplusplus
extern "C" {
#endif

/* for casting pointer to one type to pointer to another type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline void *_cast_void__(void *p)
{
	return p;
}

/* for casting const pointer to one type to const pointer to another type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline const void *_cast_constant_void__(const void *p)
{
	return p;
}

/* for casting const pointer to non-const one to the same type */
A_Const_function A_Check_return A_Ret_range(==,p)
static inline void *_const_cast_void__(const void *p)
{
	union {
		const void *c;
		void *v;
	} u;
	u.c = p;
	return u.v;
}

/* cast non-const pointer to one type to non-const pointer to another type:

  struct my_type1 *p;
  struct my_type2 *q = CAST(struct my_type2, p);
*/
#define CAST(type, ptr) ((type*)_cast_void__(ptr))

/* cast const pointer to one type to const pointer to another type:

  const struct my_type1 *p;
  const struct my_type2 *q = CAST_CONSTANT(struct my_type2, p);
*/
#define CAST_CONSTANT(type, ptr) ((type*)_cast_constant_void__(ptr))

/* cast const pointer to non-const one to the same type:

  const struct my_type *p;
  struct my_type *q = CONST_CAST(struct my_type, p);
*/
#define CONST_CAST(type, ptr) ((type*)_const_cast_void__((ptr) + 0*sizeof(((const type*)(const void*)(ptr) - (ptr)))))

A_Nonnull_all_args A_Const_function A_Check_return A_Ret_never_null A_Ret_valid A_Ret_range(==,(char*)p - offset)
static inline void *_container_of__(
	A_Notnull A_At((char*)p - offset, A_Readable_bytes(offset)) const void *p/*!=NULL*/,
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
	return (char*)_const_cast_void__(p) - offset;
}

A_Const_function A_Check_return A_Ret_opt_valid A_When(p, A_Ret_range(==,(char*)p - offset))
static inline void *_opt_container_of__(
	A_When(p, A_At((char*)p - offset, A_Readable_bytes(offset))) const void *p/*NULL?*/,
	size_t offset)
{
	return p ? (char*)_const_cast_void__(p) - offset : NULL;
}

/* check that CONTAINER_OF() doesn't discard const qualifier */
#ifdef __GNUC__
#define _check_constness_(ptr, member, x) ((x) + 0*sizeof(*(__typeof__(&(x)->member)*)NULL = (ptr)))
#else
#define _check_constness_(ptr, member, x) (x)
#endif

/* ptr must be != NULL and must point to member of initialized object */
/* NOTE: must specify const type for const ptr */
#define CONTAINER_OF(ptr/*!=NULL*/, type, member) \
	_check_constness_(ptr, member, \
		((type*)_container_of__(ptr, offsetof(type, member) + \
			0*sizeof((&((type*)_const_cast_void__(ptr))->member - (ptr))))))

/* ptr may be NULL, else must point to member of initialized object; returns NULL if ptr is NULL */
/* NOTE: must specify const type for const ptr */
#define OPT_CONTAINER_OF(ptr/*NULL?*/, type, member) \
	_check_constness_(ptr, member, \
		((type*)_opt_container_of__(ptr, offsetof(type, member) + \
			0*sizeof((&((type*)_const_cast_void__(ptr))->member - (ptr))))))

/* ptr may be NULL and may point to uninitialized object */
/* NOTE: also does const-cast - may get non-const pointer from a const one */
#define RAW_CONTAINER_OF(ptr/*NULL?*/, type, member) \
	((type*)_const_cast_void__((const char*)(ptr) - offsetof(type, member) + \
		0*sizeof((&((type*)_const_cast_void__(ptr))->member - (ptr)))))

#ifdef __cplusplus
}
#endif

#endif /* CCASTS_H_INCLUDED */
