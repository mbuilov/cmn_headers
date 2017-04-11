# cmn_headers
Common C/C++ definitions in header files

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

Debug printing:
- [DBGPRINT](#print-line-of-debugging-text)
- [DBGPRINTX](#print-line-of-debugging-text-at-position)

Swap byte order:
- [bswap2](#swap-byte-order-of-16--32-or-64-bit-unsigned-integer)
- [bswap4](#swap-byte-order-of-16--32-or-64-bit-unsigned-integer)
- [bswap8](#swap-byte-order-of-16--32-or-64-bit-unsigned-integer)
- [hswap2](#swap-halves-of-16--32-or-64-bit-unsigned-integer)
- [hswap4](#swap-halves-of-16--32-or-64-bit-unsigned-integer)
- [hswap8](#swap-halves-of-16--32-or-64-bit-unsigned-integer)

Parse command line options:
- [get_opts](#parse-command-line-options)

----------------------------------------

#### Ensure that condition is non-zero at runtime
```C
ASSERT(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, must give non-zero result

If `condition` is zero on runtime, then:
* in debug builds (when `_DEBUG` is defined), program terminates
* in release builds (when `_DEBUG` is not defined), program behaviour is **undefined**

_Note_: in some configurations `condition` may not be evaluated, so its evaluation must have **no side-effects**.

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
* in release builds (when `_DEBUG` is not defined), `condition` is **not evaluated**

_Note_: in some configurations `condition` may not be evaluated, so its evaluation must have **no side-effects**.

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

_Note_: `expression` is evaluated on compile-time, any side-effects of its evaluation are **discarded**.

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

_Note_: `expression` is evaluated on compile-time, any side-effects of its evaluation are **discarded**.

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

_Note_: Some compilers may not trigger compilation errors on invalid usage of `COUNT_OF()` macro.

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
- `ptr`  - **non-const** pointer to cast

**Returns:** pointer to given type

`CAST()` protects from casting const pointer to non-const one, this macro should trigger compilation error/warning if `ptr` is const

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CAST()` macro.

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
- `type` - **const** type to cast pointer to
- `ptr`  - pointer to cast

**Returns:** const pointer to given type

`CAST_CONSTANT()` protects from casting const pointer to non-const one, this macro should trigger compilation error/warning if `type` is not const

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CAST_CONSTANT()` macro.

_Note_: To cast constant pointers to non-const ones - use [`CONST_CAST()`](#remove-pointer-constness).

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

`CONST_CAST()` removes pointer _constness_ without changing pointer type, this macro should trigger compilation error/warning if `type` is not the type of the pointer.

_Note_: Some compilers may not trigger compilation errors or warnings on invalid usage of `CONST_CAST()` macro.

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

_Note_: returned pointer generally points some bytes before `ptr`.

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

----------------------------------------

#### Print line of debugging text
```C
DBGPRINT(format, ...)
```
Parameters:
- `format` - `printf`-like format string (without `'\n'` at end)
- `...`    - formatting parameters

_Note_:
* if `DEBUG_TO_LOG` is defined, this must be a name of printing function: `void DEBUG_TO_LOG(const char *format, ...);`
* if `DEBUG_TO_LOG` is not defined, in release builds (when `_DEBUG` is not defined), parameters of `DBGPRINT` are not evaluated, so their evaluation must have **no side-effects**
* if `CURRENT_THREAD_ID` is defined, this must be a macro returning current thread identifier number, which is formatted according to `THREAD_ID_FORMAT` value

*Example:*
```C
void foo()
{
	DPRINT("debug message %d: %s", 1, "some message");
}
```

*Declared in:* [`dprint.h`](/dprint.h)

#### Print line of debugging text at position
```C
DBGPRINTX(file, line, function, format, ...)
```
Parameters:
- `file`     - position file name
- `line`     - position line number
- `function` - position function name
- `format`   - `printf`-like format string (without `'\n'` at end)
- `...`      - formatting parameters

_Note_: this macro is implicitly called from [`DBGPRINT`](#print-line-of-debugging-text)

*Declared in:* [`dprint.h`](/dprint.h)

----------------------------------------

#### Swap byte order of 16, 32 or 64-bit unsigned integer
```C
UINT16_TYPE bswap2(UINT16_TYPE x);
UINT32_TYPE bswap4(UINT32_TYPE x);
UINT64_TYPE bswap8(UINT64_TYPE x);
```
Parameters:
- `x` - unsigned 16, 32 or 64-bit integer to swap bytes of

**Returns:** unsigned 16, 32 or 64-bit integer with reverse byte order

_Note_: UINT16_TYPE, UINT32_TYPE and UINT64_TYPE integer types are defined in place if they are not defined already

*Declared in:* [`bswaps.h`](/bswaps.h)

#### Swap halves of 16, 32 or 64-bit unsigned integer
```C
UINT16_TYPE hswap2(UINT16_TYPE x);
UINT32_TYPE hswap4(UINT32_TYPE x);
UINT64_TYPE hswap8(UINT64_TYPE x);
```
Parameters:
- `x` - unsigned 16, 32 or 64-bit integer to swap haves of

**Returns:** unsigned 16, 32 or 64-bit integer with swapped high and low halves

_Note_: UINT16_TYPE, UINT32_TYPE and UINT64_TYPE integer types are defined in place if they are not defined already

*Declared in:* [`bswaps.h`](/bswaps.h)

----------------------------------------

#### Parse command line options
```C
int get_opts(
	int argc,
	char *argv[/*argc*/],
	const char *opts/*'\0'-terminated*/,
	char *values[/*strlen(opts)*/]
);
```
Parameters:
- `argc`   - number of program arguments, as passed to `int main(int argc, char *argv[])`
- `argv`   - program arguments array, as passed to `int main(int argc, char *argv[])`
- `opts`   - options string in format: one letter for one option, for example: `"abc"`
- `values` - to be filled array of pointers to values of the options, must be big enough

**Returns:**
* `0`  - no errors, `values` array is filled
* `>0` - number of first non-option program argument (not started with `'-'`)
* `<0` - number of first unknown option argument

_Notes_:
* options may be specified in any order
* options may be optional, corresponding value in `values` array will be `NULL`
* an option may be specified many times - only the last value is used

*Example:*
```C
int main(int argc, char *argv[])
{
	#define opts "abc"
	#define A values[0]
	#define B values[1]
	#define C values[2]
	char *values[sizeof(opts) - 1];
	{
		int i = get_opts(argc, argv, opts, values);
		if (i > 0) {
			fprintf(stderr, "%s: expecting an option argument started with '-': %s\n", argv[0], argv[i]);
			return 1;
		}
		if (i < 0) {
			fprintf(stderr, "%s: unknown option: %s\n", argv[0], argv[-i]);
			return 2;
		}
	}
	....
}
```

*Declared in:* [`get_opts.inl`](/get_opts.inl)
