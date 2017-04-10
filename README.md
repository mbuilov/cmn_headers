# cmn_headers
Common C/C++ definitions in header files

## Macros

- [ASSERT](#ensure-that-condition-is-non-zero)
- [DEBUG_CHECK](#check-that-condition-is-non-zero)
- [STATIC_ASSERT](#ensure-that-expression-is-non-zero-on-compile-time)
- [EMBED_ASSERT](#ensure-that-expression-is-non-zero-on-compile-time-and-return-zero)
- [COUNT_OF](#get-number-of-elements-in-array)
- [CAST](#cast-non-const-pointer)

#### Ensure that condition is non-zero
```C
ASSERT(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, must give non-zero result

If `condition` is zero on runtime, then:
* in debug builds (when `\_DEBUG` is defined), program terminates
* in release builds (when `\_DEBUG` is not defined), program behaviour is undefined

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

#### Check that condition is non-zero
```C
DEBUG_CHECK(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, should give non-zero result

If `condition` is zero on runtime, then:
* in debugging builds (when `\_DEBUG` is defined), program terminates
* in release builds (when `\_DEBUG` is not defined), not `condition` is not evaluated

_Note_: in some configurations `condition` may not be evaluated, so its evaluation must have no side-effects

*Example:*
```C
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

#### Ensure that expression is non-zero on compile-time
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

#### Ensure that expression is non-zero on compile-time and return zero
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

#### Get number of elements in array
```C
COUNT_OF(array)
```
Parameters:
- `array` - array to count elements in

**Returns:** number of elements

_Note_:
In C/C++ arrays are passed to functions as pointers, without any information about number of elements.
`COUNT_OF(arr)` will trigger compiler error if `arr` size is not known to compiler.

*Example:*
```C
void foo(int (*arr)[10], int tt[10])
{
	int baz[3];
	size_t count = COUNT_OF(*arr); /* 10 */
	size_t zount = COUNT_OF(baz); /* 3 */
	size_t tount = COUNT_OF(tt); /* compiler error */
}
```

*Declared in:* [`asserts.h`](/asserts.h)

#### Cast non-const pointer
```C
CAST(type, ptr)
```
Parameters:
- `type` - type to cast pointer to
- `ptr`  - non-const pointer to cast

**Returns:** non-const pointer to given type

_Note_:
`CAST()` protects from casting const pointers to non-const ones.
To cast constant pointers - use [`CAST_CONSTANT()`](#fffff) or [`CONST_CAST()`](#ggggg)

*Example:*
```C
struct typeP;
struct typeG;
void foo(struct typeP *p)
{
	struct typeG *g = CAST(struct typeG, p); /* ok */
	const struct typeP *pp = p;
	g = CAST(struct typeG, pp); /* compiler error */
}
```

*Declared in:* [`asserts.h`](/ccasts.h)

#### Cast pointer to const one
```C
CAST_CONSTANT(type, ptr)
```
Parameters:
- `type` - type to cast pointer to
- `ptr`  - pointer to cast

**Returns:** const pointer to given type

_Note_:
`CAST_CONSTANT()` preserves constness when casting const pointer.
To cast constant pointers - use [`CAST_CONSTANT()`](#fffff) or [`CONST_CAST()`](#ggggg)

*Example:*
```C
struct typeP;
struct typeG;
void foo(struct typeP *p)
{
	struct typeG *g = CAST(struct typeG, p); /* ok */
	const struct typeP *pp = p;
	g = CAST(struct typeG, pp); /* compiler error */
}
```

*Declared in:* [`asserts.h`](/ccasts.h)





