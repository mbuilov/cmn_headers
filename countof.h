#ifndef COUNTOF_H_INCLUDED
#define COUNTOF_H_INCLUDED

/**********************************************************************************
* Obtain number of elements in array
* Copyright (C) 2019-2020 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* countof.h */

/* defines:
  COUNT_OF(arr)
  MEMBER_SIZE(type, member)
  COUNT_OF_MEMBER(type, member_arr)
  CONST_STR_LEN(str)
*/

/* number of elements in array:

  int arr[10];
  size_t count = COUNT_OF(arr); // 10

or

  struct S {int arr[10];}
  struct S *s;
  size_t count = COUNT_OF(s->arr); // 10
*/

#ifndef COUNT_OF
#ifdef __cplusplus
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtemplates"
#endif
template <typename T, unsigned long long N> char (&COUNT_OF_(T (&arr)[N]))[N];
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

/* size of type's member:

  struct S {int a;}
  size_t sz = MEMBER_SIZE(S, a); // sizeof(int)
*/

#ifndef MEMBER_SIZE
#define MEMBER_SIZE(type, member) sizeof(((type*)0)->member)
#endif

/* number of elements in type's member array:

  struct S {int arr[10];}
  size_t count = COUNT_OF_MEMBER(S, arr); // 10
*/

#ifndef COUNT_OF_MEMBER
#define COUNT_OF_MEMBER(type, member_arr) COUNT_OF(((type*)0)->member_arr)
#endif

/* constant string length:

  #define STR "str"
  size_t len = CONST_STR_LEN(STR); // 3

or

  #define LSTR L"str"
  size_t len = CONST_STR_LEN(LSTR); // 3
*/

#ifndef CONST_STR_LEN
#define CONST_STR_LEN(str) ((sizeof(str "") - sizeof(str[0]))/sizeof(str[0]))
#endif

#endif /* COUNTOF_H_INCLUDED */
