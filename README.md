# cmn_headers
Common C/C++ definitions in header files

* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* All files, except sal_defs.h are licensed under Apache License v2.0, see LICENSE.TXT
* sal_defs.h licensed under Public domain

## Installing

- for UNIX-like OS, run [install.sh](/install.sh)
- for WINDOWS, use [install.bat](/install.bat)

## Api documentation

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

Container from pointer to member:
- [CONTAINER_OF](#get-non-null-pointer-to-container-from-non-null-pointer-to-member)
- [OPT_CONTAINER_OF](#get-pointer-to-container-from-possibly-null-pointer-to-member)

Debug printing:
- [DBGPRINT](#print-line-of-debugging-text)
- [DBGPRINTX](#print-line-of-debugging-text-at-position)

Swap byte order:
- [bswap2/bswap4/bswap8](#swap-byte-order-of-16-32-or-64-bit-unsigned-integer)
- [hswap2/hswap4/hswap8](#swap-halves-of-16-32-or-64-bit-unsigned-integer)

Parse command line options:
- [get_opts](#parse-command-line-options)
- [get_opt](#parse-command-line-options-and-parameters)

----------------------------------------

#### Ensure that condition is non-zero at runtime
```C
ASSERT(condition)
```
Parameters:
- `condition` - condition to check: if evaluated, must give non-zero result

If `condition` is zero at runtime, then:
* in debug builds (when `_DEBUG` is defined), program terminates with some error message
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

If `condition` is zero at runtime, then:
* in debug builds (when `_DEBUG` is defined), program terminates with some error message
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

_Note_: `expression` is evaluated at compile-time, any side-effects of its evaluation are **discarded**.

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

_Note_: `expression` is evaluated at compile-time, any side-effects of its evaluation are **discarded**.

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

_Note_: Some compilers may not trigger compilation error on invalid usage of `COUNT_OF()` macro.

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

`CAST()` allows casting only non-const pointers, for a const pointer this macro should trigger compilation error or warning.

_Note_: Some compilers may not trigger compilation error or warning on invalid usage of `CAST()` macro.

_Note_: To cast **const** pointers use [`CAST_CONSTANT()`](#cast-pointer-to-constant) or [`CONST_CAST()`](#remove-pointer-constness)

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

`CAST_CONSTANT()` allows casting only to const type, assuming that `ptr` points to some constant. For a non-const type this macro should trigger compilation error or warning.

_Note_: Some compilers may not trigger compilation error or warning on invalid usage of `CAST_CONSTANT()` macro.

_Note_: To cast **const** pointer to **non-const** one without changing pointer type (remove _constness_) use [`CONST_CAST()`](#remove-pointer-constness)

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
- `type` - **non-const** type of the pointer
- `ptr`  - **const** pointer to cast

**Returns:** non-const pointer to the same type

`CONST_CAST()` removes pointer _constness_ without changing pointer type, this macro should trigger compilation error or warning if `type` is not the type of the pointer.

_Note_: Some compilers may not trigger compilation error or warning on invalid usage of `CONST_CAST()` macro.

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
- `ptr`    - non-`NULL` pointer to member field of container structure
- `type`   - type of container
- `member` - name of member field

**Returns:** non-`NULL` pointer to container

_Note_: returned pointer points to memory at some, possible zero, offset above `ptr`.

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

#### Get pointer to container from possibly NULL pointer to member
```C
OPT_CONTAINER_OF(ptr, type, member)
```
Parameters:
- `ptr`    - pointer to member field of container structure, may be `NULL`
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
* if `DEBUG_TO_LOG` is defined, it must be a name of printing function:
  ```C
  void DEBUG_TO_LOG(const char *format, ...);
  ```
* if `DEBUG_TO_LOG` is not defined, in release builds (when `_DEBUG` is not defined), parameters of `DBGPRINT` are not evaluated, so their evaluation must have **no side-effects**
* if `CURRENT_THREAD_ID` is defined, it must be a macro returning current thread identifier number, which is formatted according to `THREAD_ID_FORMAT` value

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

_Note_: this macro is implicitly called by [`DBGPRINT`](#print-line-of-debugging-text)

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

**Returns:** `x` with reverse byte order

_Note_: `UINT16_TYPE`, `UINT32_TYPE` and `UINT64_TYPE` integer types are defined in place if they are not defined already

*Declared in:* [`bswaps.h`](/bswaps.h)

#### Swap halves of 16, 32 or 64-bit unsigned integer
```C
UINT16_TYPE hswap2(UINT16_TYPE x);
UINT32_TYPE hswap4(UINT32_TYPE x);
UINT64_TYPE hswap8(UINT64_TYPE x);
```
Parameters:
- `x` - unsigned 16, 32 or 64-bit integer to swap haves of

**Returns:** `x` with swapped high and low halves

_Note_: `UINT16_TYPE`, `UINT32_TYPE` and `UINT64_TYPE` integer types are defined in place if they are not defined already

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

#### Parse command line options and parameters
Parsing is done by `get_opt()` function, which recognizes 4 types of options:
1. short options with or without value (like `-h` for help or `-f file` or `-ffile` to specify a file)
2. long options with or without value (like `--help` or `--file file` or `--file=file`)
3. long options started with one dash (like `-help` or `-file file` or `-file=file`)
4. dash or double-dash options (`-` or `--` alone)

Multiple short options that do not require a value may be bundled together: `-abc` is equivalent to `-a -b -c`.

Dash option `-` usually used to specify `stdin` or `stdout` streams.

Double-dash option `--` denotes end of options, all arguments after it - are parameters.

`get_opt()` function uses next structure to hold state of options parsing:
```C
struct opt_info {
	char *const *arg;       /* in/out: current argument               */
	char *const *args_end;  /* in:     end of program arguments array */
	char *value;            /* out:    option value or parameter      */
	char *sopt;             /* in/out: next short option in a bundle  */
};
```

`struct opt_info` must be initialized by next function prior passing it into `get_opt()`:
```C
void opt_info_init(
	struct opt_info *i,  /* out, != NULL */
	int argc,            /* in,  > 0     */
	char *const argv[]   /* in,  != NULL */
);
```
Parameters:
- `i`    - structure to initialize
- `argc` - number of program arguments, as passed to `int main(int argc, char *argv[])`
- `argv` - program arguments array, as passed to `int main(int argc, char *argv[])`

Main function for parsing command-line options and parameters:
```C
int get_opt(
	struct opt_info *i,            /* in/out, != NULL  */
	const char short_opts[],       /* in,     == NULL? */
	const char *const long_opts[]  /* in,     == NULL? */
);
```
Parameters:
- `i`          - structure that describes state of options parsing, must be initialized by `opt_info_init()`
- `short_opts` - `'\0'`-terminated short options format string
- `long_opts`  - `NULL`-terminated array of long options names

_Note_: `get_opt()` expects that `i->arg < i->args_end`, caller must check this prior the call.

__Format of `short_opts` string:__
* first symbol - option name: any character, usually a letter or decimal digit, except `'-'` (dash)
* second symbol - option type or another short option name, depending on the value:
  - copy of the first symbol denotes that option expects a value (like `-f file` or `-ffile`)
  - `'-'` (dash) denotes that option is a first letter of long option started with one dash (like `-myopt` or `-myopt=val`)
  - other character means that option has no type, this character is a name of another option

Example of short options format string: `"aabbccde-f"`

(options `a`,`b`,`c` should be passed with a value, option `d` do not expects a value, option `e` is the first letter of long option, `f` - also do not expects a value)

__Format of `long_opts` array:__
* each name is a (non-empty) `'\0'`-terminated C-string
* name must not contain a `'='` character, except at the beginning, where it denotes that the option expects a value

Example of long options names array: `{"=file","=output","verbose","trace",NULL}`

(options `file` and `output` should be passed with a value, options `verbose` and `trace` do not expect a value)

Macros for encoding and decoding parsed options positions, returned by `get_opt()`:
```C
SHORT_OPT(pos)    /* encode short option position in short options format string  */
LONG_OPT(idx)     /* encode long option index in long options names array */
DECODE_OPT(code)  /* decode short or long option position */
IS_LONG_OPT(code) /* check if long option has been matched */
```

**Returns:**
* `>=0`             - matched option position in `short_opts` string or `long_opts` array, encoded correspondingly via `SHORT_OPT` or `LONG_OPT` macros
* `OPT_UNKNOWN`     - `i->arg` points to unknown option argument (may be bundle), `i->sopt`, if not `NULL`, points to unknown short option character in short options bundle
* `OPT_BAD_BUNDLE`  - `i->sopt` denotes short option that cannot be bundled, `i->arg` points to whole short options bundle argument
* `OPT_PARAMETER`   - `i->value` points to non-`NULL` parameter
* `OPT_DASH`        - `-` (dash) option was parsed (this option usually used to specify `stdin`/`stdout`)
* `OPT_REST_PARAMS` - after `--`, all rest arguments starting with `i->arg` and until `i->args_end` - are parameters

_Notes_:
* a non-`NULL` value may be parsed for a long option that do not expects one, if a value was provided together with the option (i.g. `--option=value`)
* a `NULL` value may be parsed for an option that expects some value - if no value was provided for the option in the command line

*Example:*
```C
int main(int argc, char *argv[])
{
	static const char short_opts[] = "aacb-";
	static const char *const long_opts[] = {"=file","beta",NULL};
	struct opt_info i;
	opt_info_init(&i, argc, argv);
	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPT(0): /* short option 'a', may be specified with a value */
				printf("'a' has value: %s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT(2): /* short option 'c' */
				printf("option 'c'\n");
				break;
			case LONG_OPT(0): /* long option "file", may be specified with a value */
				printf("'file' has value: %s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT(1): /* long option "beta", not expecting a value, but one may be specified */
				printf("option 'beta' has value: '%s'\n", i.value ? i.value : "<null>");
				break;
			case OPT_UNKNOWN:
				if (i.sopt)
					printf("unknown short option '%c' in the bundle: '%s'\n", *i.sopt, *i.arg);
				else
					printf("unknown option: '%s'\n", *i.arg);
				i.arg++; /* skip unknown option (whole bundle) */
				i.sopt = NULL; /* reset current bundle */
				break;
			case OPT_BAD_BUNDLE:
				printf("short option '%c' cannot be bundled: '%s'\n", *i.sopt, *i.arg);
				i.arg++; /* skip bad bundle */
				i.sopt = NULL; /* reset current bundle */
				break;
			case OPT_PARAMETER:
				printf("parameter: %s\n", i.value);
				break;
			case OPT_DASH:
				printf("dash option: '-'\n");
				break;
			case OPT_REST_PARAMS:
				do {
					printf("parameter: %s\n", *i.arg++);
				} while (i.arg != i.args_end);
				break;
			default:
				fprintf(stderr, "assert!\n");
				return 1;
		}
	}
	return 0;
}
```

*Declared in:* [`get_opt.inl`](/get_opt.inl)
