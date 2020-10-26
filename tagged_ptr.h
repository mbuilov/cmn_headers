#ifndef TAGGED_PTR_H_INCLUDED
#define TAGGED_PTR_H_INCLUDED

/**********************************************************************************
* Add a tag to a pointer value
* Copyright (C) 2019-2020 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* tagged_ptr.h */

/* defines:
  PTR_ADD_TAG(type, ptr, tag)
  PTR_CLEAR_TAGS(type, ptr)
  PTR_GET_TAGS(ptr)
  PTR_MAKE_TAGGED(type, value, tag)
*/

#include "static_asserts.h"

/* this mask is used while converting pointer to an unsigned long long integer and back:
  if pointer type is signed, it may be sign-extended during conversion to unsigned long long integer
  - these extra bits should be masked out before converting unsigned long long integer back to a pointer */
#define PTR_VALUE_MASK \
	(/* byte has 8 bits */0*sizeof(int[1-2*(255 != (unsigned char)-1)]) + \
	((1llu << (8*sizeof(void*) - 1)) | ~(~0llu << (8*sizeof(void*) - 1))))

#if defined __cplusplus && __cplusplus >= 201402L
constexpr
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && __clang_major__ > 3 - (__clang_minor__ >= 7))
__attribute__ ((const))
#endif
static inline void *ptr_add_tag_(void *const ptr/*NULL?*/, const unsigned tag/*>=0*/)
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4826) /* Conversion from 'const char *' to 'unsigned __int64' is sign-extended */
#pragma warning(disable:4305) /* 'type cast': truncation from 'unsigned __int64' to 'void *' */
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast" /* warning: cast from pointer to integer of different size */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" /* warning: cast from pointer to integer of different size */
#endif
	return (void*)(PTR_VALUE_MASK & ((unsigned long long)ptr | tag));
#ifdef _MSC_VER
#pragma warning(pop)
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
}

#if defined __cplusplus && __cplusplus >= 201402L
constexpr
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && __clang_major__ > 3 - (__clang_minor__ >= 7))
__attribute__ ((const))
#endif
static inline void *ptr_clear_tags_(void *const ptr/*NULL?*/, const unsigned align/*>0*/)
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4826) /* Conversion from 'const char *' to 'unsigned __int64' is sign-extended */
#pragma warning(disable:4305) /* 'type cast': truncation from 'unsigned __int64' to 'void *' */
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast" /* warning: cast from pointer to integer of different size */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" /* warning: cast from pointer to integer of different size */
#endif
	return (void*)(PTR_VALUE_MASK & ((unsigned long long)ptr & ~(align - 1llu)));
#ifdef _MSC_VER
#pragma warning(pop)
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
}

#if defined __cplusplus && __cplusplus >= 201402L
constexpr
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && __clang_major__ > 3 - (__clang_minor__ >= 7))
__attribute__ ((const))
#endif
static inline unsigned ptr_get_tags_(const void *const ptr/*NULL?*/, const unsigned align/*>0*/)
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4826) /* Conversion from 'const char *' to 'unsigned __int64' is sign-extended */
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast" /* warning: cast from pointer to integer of different size */
#endif
	return (unsigned)((unsigned long long)ptr & (align - 1llu));
#ifdef _MSC_VER
#pragma warning(pop)
#elif !defined __cplusplus && defined __GNUC__ && __GNUC__ > 4 - (__GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
}

#if defined __cplusplus && __cplusplus >= 201402L
constexpr
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && __clang_major__ > 3 - (__clang_minor__ >= 7))
__attribute__ ((const))
#endif
static inline void *ptr_make_tagged_(const unsigned value, const unsigned tag/*>=0*/)
{
	return (void*)(value + tag);
}

#if defined __cplusplus && __cplusplus >= 201103L
#define ALIGNOF_TYPE(type) alignof(type)
#define ALIGNOF_EXPR(expr) alignof(decltype(expr))
#elif defined _MSC_VER
#define ALIGNOF_TYPE(type) __alignof(type)
#define ALIGNOF_EXPR(expr) __alignof(expr)
#else /* assume GCC-compatible compiler */
#define ALIGNOF_TYPE(type) __alignof__(type)
#define ALIGNOF_EXPR(expr) __alignof__(expr)
#endif /* assume GCC-compatible compiler */

/* add non-zero constant tag to a pointer,
  returns tagged pointer */
/* 'type' - type of a pointed object */
/* note: 'type' must have non-zero alignment requirement */
#define PTR_ADD_TAG(type, ptr, tag) \
	((type*)ptr_add_tag_(ptr, (tag) +                                                   \
	/* type must be a type of ptr */0*sizeof((const type*)(const void*)(ptr) - (ptr)) + \
	/* tag must be non-zero constant */EMBED_ASSERT((tag) > 0) +                        \
	/* ALIGNOF() must be integer */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr)) <= (unsigned)-1) + \
	/* tag must be small enough */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr)) > (tag))))

/* remove tags from a (tagged?) pointer,
  returns pointer without tags */
/* 'type' - type of a pointed object */
#define PTR_CLEAR_TAGS(type, ptr) \
	((type*)ptr_clear_tags_(ptr, ALIGNOF_EXPR(*(ptr)) +                                 \
	/* type must be a type of ptr */0*sizeof((const type*)(const void*)(ptr) - (ptr)) + \
	/* ALIGNOF() must be integer */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr)) <= (unsigned)-1) + \
	/* type alignment must be non-zero */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr)))))

/* extract tags from a pointer value,
  returns tags */
#define PTR_GET_TAGS(ptr) \
	ptr_get_tags_(ptr, ALIGNOF_EXPR(*(ptr)) +                                           \
	/* ALIGNOF() must be integer */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr)) <= (unsigned)-1) + \
	/* type alignment must be non-zero */EMBED_ASSERT(ALIGNOF_EXPR(*(ptr))))

/* make (invalid) tagged pointer,
  such a pointer may be used as an error indicator,
  where 'value' - the error number */
#define PTR_MAKE_TAGGED(type, value, tag) \
	((type*)ptr_make_tagged_((value)*ALIGNOF_TYPE(type), (tag) +                      \
	/* tag must be non-zero constant */EMBED_ASSERT((tag) > 0) +                      \
	/* ALIGNOF() must be integer */EMBED_ASSERT(ALIGNOF_TYPE(type) <= (unsigned)-1) + \
	/* tag must be small enough */EMBED_ASSERT(ALIGNOF_TYPE(type) > (tag)) +          \
	/* value must be non-negative */EMBED_ASSERT((value) >= 0) +                      \
	/* value must not be too big */EMBED_ASSERT(0 + (value) <= (unsigned)-1/ALIGNOF_TYPE(type))))

#endif /* TAGGED_PTR_H_INCLUDED */
