# cmn_headers
Common C/C++ definitions in header files

## **Macros**

Asserts:
- [ASSERT](#ensure-that-condition-is-non-zero-at-runtime)
- [DEBUG_CHECK](#check-that-condition-is-non-zero-at-runtime)
- [STATIC_ASSERT](#ensure-that-expression-is-non-zero-at-compile-time)
- [EMBED_ASSERT](#ensure-that-expression-is-non-zero-at-compile-time-and-return-zero)

Number of elements in array:
- [COUNT_OF](#get-number-of-elements-in-array)

Casting pointers:
- [CAST](#cast-non-const-pointer)
- [CAST_CONSTANT](#cast-pointer-to-constant)
- [CONST_CAST](#remove-pointer-constness)

Container of member field:
- [CONTAINER_OF](#get-non-null-pointer-to-container-from-non-null-pointer-to-member)
- [OPT_CONTAINER_OF](#get-pointer-to-container-from-pointer-to-member)

----------------------------------------

#### Ensure that condition is non-zero at runtime
```C
ASSERT(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, must give non-zero result

If `condition` is zero on runtime, then:
* in debug builds (when `_DEBUG` is defined), program terminates
* in release builds (when `_DEBUG` is not defined), program behaviour is undefined

_Note_: in some configurations `condition` may not be evaluated, so its evaluation must have no side-effects

*Example:*
```C
void foo(int *p)
{
	ASSERT(p);
	*p = 0;
}
```

*Declared in:* [`asserts.h`](/asserts.h)

#### Check that condition is non-zero at runtime
```C
DEBUG_CHECK(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, should give non-zero result

If `condition` is zero on runtime, then:
* in debug builds (when `_DEBUG` is defined), program terminates
* in release builds (when `_DEBUG` is not defined), not `condition` is not evaluated

_Note_: in some configurations `condition` may not be evaluated, so its evaluation must have no side-effects

*Example:*
```C
#define STATUS_OK 1
#define STATUS_FAIL 0
int foo(int *p)
{
	DEBUG_CHECK(p);
	if (!p)
		return STATUS_FAIL;
	*p = 0;
	return STATUS_OK;
}
```

*Declared in:* [`asserts.h`](/asserts.h)

#### Ensure that expression is non-zero at compile-time
```C
STATIC_ASSERT(expression)
```
Parameters:
- `expression` - expression to verify, must give non-zero result

_Note_: `expression` is evaluated on compile-time, any side-effects of its evaluation are discarded

*Example:*
```C
#define SIZE 10
...
void foo(int arr[SIZE])
{
	STATIC_ASSERT(SIZE >= 10);
	arr[9] = 0;
}
```

*Declared in:* [`asserts.h`](/asserts.h)

#### Ensure that expression is non-zero at compile-time and return zero
```C
EMBED_ASSERT(expression)
```
Parameters:
- `expression` - expression to verify, must give non-zero result

**Returns:** zero

_Note_: `expression` is evaluated on compile-time, any side-effects of its evaluation are discarded

*Example:*
```C
#define SIZE 10
...
/* array with at least 10 elements */
typedef int arr_t[SIZE + EMBED_ASSERT(SIZE > 10)];
```

*Declared in:* [`asserts.h`](/asserts.h)

----------------------------------------

#### Get number of elements in array
```C
COUNT_OF(array)
```
Parameters:
- `array` - array to count elements in

**Returns:** number of elements

In C/C++ arrays are passed to functions as pointers, without any information about number of elements.

`COUNT_OF(array)` should trigger compilation error if number of elements in `array` is not known to compiler.

_Note_: Some compilers may not trigger compilation errors on invalid usage of `COUNT_OF()` macro

*Example:*
```C
void foo(int (*arr)[10], int tt[10])
{
	int baz[3];
	size_t count = COUNT_OF(*arr); /* 10 */
	size_t zount = COUNT_OF(baz); /* 3 */
	size_t tount = COUNT_OF(tt); /* compilation error */
}
```

*Declared in:* [`asserts.h`](/asserts.h)

----------------------------------------

#### Cast non-const pointer
```C
CAST(type, ptr)
```
Parameters:
- `type` - type to cast pointer to
- `ptr`  - non-const pointer to cast

**Returns:** pointer to given type

`CAST()` protects from casting const pointer to non-const one, this macro should trigger compilation error/warning if `ptr` is const

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CAST()` macro

_Note_: To cast constant pointers - use [`CAST_CONSTANT()`](#cast-pointer-to-constant) or [`CONST_CAST()`](#remove-pointer-constness)

*Example:*
```C
struct P;
struct G;
void foo(struct P *p)
{
	      struct G *g1 = CAST(      struct G,                  p); /* ok */
	const struct G *g2 = CAST(const struct G,                  p); /* ok */
	      struct G *g3 = CAST(      struct G, (const struct P*)p); /* compilation error or warning */
	const struct G *g4 = CAST(const struct G, (const struct P*)p); /* compilation error or warning */
}
```

*Declared in:* [`ccasts.h`](/ccasts.h)

#### Cast pointer to constant
```C
CAST_CONSTANT(type, ptr)
```
Parameters:
- `type` - const type to cast pointer to
- `ptr`  - pointer to cast

**Returns:** const pointer to given type

`CAST_CONSTANT()` protects from casting const pointer to non-const one, this macro should trigger compilation error/warning if `type` is not const

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CAST_CONSTANT()` macro

_Note_: To cast constant pointers to non-const ones - use [`CONST_CAST()`](#remove-pointer-constness)

*Example:*
```C
struct P;
struct G;
void foo(struct P *p)
{
	      struct G *g1 = CAST_CONSTANT(      struct G,                  p); /* compilation error or warning */
	const struct G *g2 = CAST_CONSTANT(const struct G,                  p); /* ok */
	      struct G *g3 = CAST_CONSTANT(      struct G, (const struct P*)p); /* compilation error or warning */
	const struct G *g4 = CAST_CONSTANT(const struct G, (const struct P*)p); /* ok */
}
```

*Declared in:* [`ccasts.h`](/ccasts.h)

#### Remove pointer constness
```C
CONST_CAST(type, ptr)
```
Parameters:
- `type` - non-const type of the pointer
- `ptr`  - const pointer to cast

**Returns:** non-const pointer to the same type

`CONST_CAST()` removes pointer _constness_ without changing pointer type, this macro should trigger compilation error/warning if `type` is not the type of the pointer

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CONST_CAST()` macro

*Example:*
```C
struct P;
struct G;
void foo(const struct P *cp)
{
	struct P *p = CONST_CAST(struct P, cp); /* ok */
	struct G *g = CONST_CAST(struct G, cp); /* compilation error or warning */
}
```

*Declared in:* [`ccasts.h`](/ccasts.h)

----------------------------------------

#### Get non-NULL pointer to container from non-NULL pointer to member
```C
CONTAINER_OF(ptr, type, member)
```
Parameters:
- `ptr`    - non-`NULL` pointer to member field of container
- `type`   - type of container
- `member` - name of member field

**Returns:** non-`NULL` pointer to container

_Note_: returned pointer generally points some bytes before `ptr`

*Example:*
```C
struct Container {
	int a;
	float b;
	double c;
};
struct Container *foo(float *m)
{
	return CONTAINER_OF(m, struct Container, b);
}
```

*Declared in:* [`ccasts.h`](/ccasts.h)

#### Get pointer to container from pointer to member
```C
OPT_CONTAINER_OF(ptr, type, member)
```
Parameters:
- `ptr`    - pointer to member field of container, may be `NULL`
- `type`   - type of container
- `member` - name of member field

**Returns:** pointer to container or `NULL` if `ptr` is `NULL`

*Example:* see [CONTAINER_OF](#get-non-null-pointer-to-container-from-non-null-pointer-to-member)

*Declared in:* [`ccasts.h`](/ccasts.h)
