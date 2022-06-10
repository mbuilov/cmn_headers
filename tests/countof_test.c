/**********************************************************************************
* Check compilation of countof.h
* Copyright (C) 2020-2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* countout_test.c */

/* should compile without errors:
  gcc -c countof_test.c
  g++ -c countof_test.c
*/

/* should not compile:
  gcc -c countof_test.c -DBAD...
*/

/* should not compile:
  g++ -c countof_test.c -DBAD...
*/

#include "../countof.h"

#define T2(line) t ## line
#define T1(line) T2(line)
#define T T1(__LINE__)

int T(unsigned x)
{
	int arr[10];
	switch (x) {
		/* COUNT_OF result must be constant */
		case COUNT_OF(arr):
			return 1;
		default:
			return 0;
	}
}

#ifdef BAD1
/* argument of COUNT_OF must be an array */
unsigned T(unsigned *a)
{
	return COUNT_OF(a);
}
#endif

#ifdef BAD2
/* argument of COUNT_OF must be an array */
struct G;
unsigned T(struct G *a)
{
	return COUNT_OF(a);
}
#endif

struct S {
	int arr[10];
	int q;
	long *p;
};

int T(struct S *const s, unsigned x)
{
	switch (x) {
		/* COUNT_OF result must be constant */
		case COUNT_OF(s->arr):
			return 1;
		default:
			return 0;
	}
}

int T(unsigned x)
{
	switch (x) {
		/* MEMBER_SIZE result must be constant */
		case MEMBER_SIZE(struct S, arr):
			return 1;
		default:
			return 0;
	}
}

int T(unsigned x)
{
	switch (x) {
		/* COUNT_OF_MEMBER result must be constant */
		case COUNT_OF_MEMBER(struct S, arr):
			return 1;
		default:
			return 0;
	}
}

#ifdef BAD3
/* argument of COUNT_OF_MEMBER must be an array */
unsigned T(void)
{
	return COUNT_OF_MEMBER(struct S, q);
}
#endif

#ifdef BAD4
/* argument of COUNT_OF_MEMBER must be an array */
unsigned T(void)
{
	return COUNT_OF_MEMBER(struct S, p);
}
#endif

int T(unsigned x)
{
	switch (x) {
		/* CONST_STR_LEN result must be constant */
		case CONST_STR_LEN("abc"):
			return 1;
		default:
			return 0;
	}
}

#ifdef BAD5
/* argument of CONST_STR_LEN must be a string constant */
unsigned T(const char *c)
{
	return CONST_STR_LEN(c);
}
#endif

/* check that COUNT_OF/MEMBER_SIZE/COUNT_OF_MEMBER/CONST_STR_LEN give correct values */
void T(void)
{
	int arr[11];
	struct X {
		unsigned x : 1-2*(COUNT_OF(arr) != 11);
	};
	(void)arr;
}

void T(const struct S *const s)
{
	struct X {
		unsigned x : 1-2*(COUNT_OF(s->arr) != 10);
	};
}

void T(void)
{
	struct X {
		unsigned x : 1-2*(MEMBER_SIZE(struct S, arr) != 10*sizeof(int));
	};
}

void T(void)
{
	struct X {
		unsigned x : 1-2*(COUNT_OF_MEMBER(struct S, arr) != 10);
	};
}

void T(void)
{
	struct X {
		unsigned x : 1-2*(CONST_STR_LEN("abc") != 3);
	};
}

void T(void)
{
	struct X {
		unsigned x : 1-2*(CONST_STR_LEN(L"abc") != 3);
	};
}
