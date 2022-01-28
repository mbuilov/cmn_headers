/**********************************************************************************
* Check compilation of ccasts.h
* Copyright (C) 2020 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* ccasts_test.c */

/* should compile without errors:
  gcc -c ccasts_test.c
  g++ -c ccasts_test.c
*/

/* should not compile:
  gcc -c ccasts_test.c -DBAD...
*/

/* should not compile:
  g++ -c ccasts_test.c -DBAD...
*/

#include "../ccasts.h"

struct A;
struct B;

#define T2(line) t ## line
#define T1(line) T2(line)
#define T T1(__LINE__)

struct B *T(struct A *const a)
{
	return CAST(struct B, a);
}

#ifdef BAD1
/* CAST should not allow casting pointer to a const */
const struct B *T(const struct A *a)
{
	return CAST(const struct B, a);
}
#endif

const struct B *T(const struct A *const a)
{
	return CAST_CONSTANT(const struct B, a);
}

const struct B *T(struct A *const a)
{
	return CAST_CONSTANT(const struct B, a);
}

#ifdef BAD2
/* CAST_CONSTANT should not allow casting const -> non-const */
const struct B *T(const struct A *a)
{
	return CAST_CONSTANT(struct B, a);
}
#endif

struct A *T(const struct A *const a)
{
	return CONST_CAST(struct A, a);
}

struct A *T(struct A *const a)
{
	return CONST_CAST(struct A, a);
}

#ifdef BAD3
/* CONST_CAST should not allow casting to incompatible type */
const struct B *T(const struct A *a)
{
	return CONST_CAST(struct B, a);
}
#endif

#ifdef BAD4
/* CONST_CAST should not allow casting to constant type */
const struct A *T(const struct A *a)
{
	return CONST_CAST(const struct A, a);
}
#endif

const struct B **T(const struct A **const a)
{
	return CAST_PPC(const struct B, a);
}

const struct B **T(struct A **const a)
{
	return CAST_PPC(const struct B, a);
}

#ifdef BAD5
/* CAST_PPC should not allow casting not a pointer-to-pointer */
const struct B **T(const struct A *a)
{
	return CAST_PPC(const struct B, a);
}
#endif

#ifdef BAD6
/* CAST_PPC should not allow casting not a pointer-to-pointer */
const struct B **T(struct A *a)
{
	return CAST_PPC(const struct B, a);
}
#endif

#ifdef BAD7
/* CAST_PPC should not allow casting pointer to constant pointer */
const struct B **T(struct A *const *a)
{
	return CAST_PPC(const struct B, a);
}
#endif

#ifdef BAD8
/* CAST_PPC should not allow casting pointer to constant pointer */
const struct B **T(const struct A *const *a)
{
	return CAST_PPC(const struct B, a);
}
#endif

#ifdef BAD9
/* CAST_PPC should not allow casting to not a pointer-to-const */
struct B **T(const struct A **a)
{
	return CAST_PPC(struct B, a);
}
#endif

struct B *T(struct A **const a)
{
	return CAST_PP(struct B, a);
}

struct B *T(const struct A **const a)
{
	return CAST_PP(struct B, a);
}

#ifdef BAD10
/* CAST_PP should not allow casting not a pointer-to-pointer */
struct B *T(struct A *a)
{
	return CAST_PP(struct B, a);
}
#endif

#ifdef BAD11
/* CAST_PP should not allow casting not a pointer-to-pointer */
const struct B *T(const struct A *a)
{
	return CAST_PP(const struct B, a);
}
#endif

#ifdef BAD12
/* CAST_PP should not allow casting pointer to constant pointer */
struct B *T(struct A *const *a)
{
	return CAST_PP(struct B, a);
}
#endif

struct B *T(struct A ***const a)
{
	return CAST_PP(struct B, a);
}

struct B *T(const struct A ***const a)
{
	return CAST_PP(struct B, a);
}

struct B *T(struct A *const **const a)
{
	return CAST_PP(struct B, a);
}

struct B *T(const struct A *const **const a)
{
	return CAST_PP(struct B, a);
}

struct C {
	double k;
	int m;
};

struct C *T(int *const p)
{
	return CONTAINER_OF(p, struct C, m);
}

const struct C *T(const int *const p)
{
	return CONTAINER_OF(p, const struct C, m);
}

const struct C *T(int *const p)
{
	return CONTAINER_OF(p, const struct C, m);
}

struct C *T(int *const p)
{
	return OPT_CONTAINER_OF(p, struct C, m);
}

const struct C *T(const int *const p)
{
	return OPT_CONTAINER_OF(p, const struct C, m);
}

const struct C *T(int *const p)
{
	return OPT_CONTAINER_OF(p, const struct C, m);
}

#ifdef BAD13
/* CONTAINER_OF should not allow discarding constness from the pointer */
struct C *T(const int *p)
{
	return CONTAINER_OF(p, struct C, m);
}
#endif

#ifdef BAD14
/* OPT_CONTAINER_OF should not allow discarding constness from the pointer */
struct C *T(const int *p)
{
	return OPT_CONTAINER_OF(p, struct C, m);
}
#endif
