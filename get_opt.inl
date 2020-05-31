#ifndef GET_OPT_INL_INCLUDED
#define GET_OPT_INL_INCLUDED

/**********************************************************************************
* Program options and parameters parsing
* Copyright (C) 2012-2020 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt.inl */

#include "get_opt_info.h"

/* note: #include <string.h> before this file */

#ifndef GET_OPT_TEXT
#ifdef GET_OPT_WIDE_CHAR_SUPPORT
#define GET_OPT_TEXT(text) L ## text
#else
#define GET_OPT_TEXT(text) text
#endif
#endif

#ifndef GET_OPT_STRCHR
#ifdef GET_OPT_WIDE_CHAR_SUPPORT
#define GET_OPT_STRCHR(s,c) wcschr(s,c)
#else
#define GET_OPT_STRCHR(s,c) strchr(s,c)
#endif
#endif

#ifndef GET_OPT_MEMCMP
#define GET_OPT_MEMCMP(a,b,n) memcmp(a,b,n)
#endif

/* note: may #include "asserts.h" for the ASSERT macro */
#ifndef ASSERT
#define ASSERT(x) ((void)0)
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4505) /* unreferenced local function has been removed */
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif

/* this file defines 7 static functions:

   1) opt_info_init             - initialize options parsing state structure 'opt_info' (declared in "get_opt_info.h"),
   2) opt_info_is_end           - check if there are no more unchecked command-line arguments,
   3) opt_arg_is_option         - check if an argument looks like an option (begins with a dash '-' and not a dash alone),
   4) get_opt                   - get next option value or parameter specified on the command line,
   5) opt_skip_unknown          - skip unrecognized option,
   6) opt_is_separate_value     - check if option value is specified in a separate argument,
   7) opt_unread_separate_value - unread option value specified in the separate argument.

   Note: get_opt() considers all strings referenced via argv[] as constants, ignoring the fact that they are declared
     as non-constant ones - C language does not allow conversion 'char **' -> 'const char *const *' (but C++ does).
*/

/*=========================== Notes: ============================================================================================
|
| 1) get_opt(i) assumes that i->arg[0] != NULL (or i->arg < i->args_end), caller must check this prior the call
|
| 2) a non-NULL value may be parsed for a long option that do not expects a value
|    - if a value was specified together with the option, for example: "--option=value"
|
| 3) a NULL value may be parsed for an option that expects some value
|    - if no value was specified for the option (e.g. at end of the command line or if another option follows current one)
|
| 4) get_opt() accepts *short* options description as '\0'-terminated C-string, in the following format:
|    . first symbol  - option name, usually a letter or decimal digit, except a dash '-' or space character,
|    . second symbol - option type (optional):
|      a) copy of the first symbol denotes that the option expects a value, like "-oval" or "-o val";
|      b) space ' ' denotes that the option is the first letter of a long option started with one dash,
|        for example, "-file" or "-file=abc", appropriate long option is then looked up in the long options array;
|      c) any other character means that the option has no type, this character is the name of another short option.
|
|    Example of short options format string: "xxf yz"
|
| 5) get_opt() accepts *long* options description as array of struct long_opt_info, where the last entry has 'len' == 0:
|    . 'len'  - is the option name length, must be non-zero, except for the last terminating entry,
|    . 'name' - is the option name - a non-empty '\0'-terminated C-string, the name must not contain a '=' character,
|               except at the beginning, where it denotes that the option expects a value. This leading '=' is not
|               counted in the 'len' value.
|
|    Example of long options array: {{4,"=file"},{5,"trace"},{0,NULL}}
|
| 6) dash '-' alone interpreted as a parameter, it is often used to denote standard input stream (stdin)
|
| 7) special option "--" denotes end of options, all arguments after it - parameters,
|    get_opt() returns OPT_REST_PARAMS
|
| 8) if an option expects a value, but a value wasn't specified together with the option (like "-fabc" or "--file=abc"),
|    get_opt() may take the next argument after the option as its value, but only if the argument do not looks like another
|    option: the argument must not begin with a dash (with exception for a dash alone),
|    for example:
|      "-f abc"         - equivalent to "-fabc",
|      "--file abc"     - equivalent to "--file=abc",
|    no value for the options "-f"/"--file":
|      "-f -h",     "-f --help",
|      "--file -h", "--file --help",
|    but
|      "-f -"           - option "-f"     will have value "-",
|      "--file -"       - option "--file" will have value "-",
|      "-f-h"           - option "-f"     will have value "-h",
|      "--file=--help"  - option "--file" will have value "--help".
|
===============================================================================================================================*/


/* get_opt(i) returns next special codes (negative values): */

#define OPT_UNKNOWN     -1 /* i->arg points to unknown option (or short options bundle) argument,
                              if i->sopt != NULL, then it points to unknown short option character inside
                              the short options bundle argument pointed by i->arg */

#define OPT_PARAMETER   -2 /* i->value points to non-NULL parameter value */

#define OPT_REST_PARAMS -3 /* "--" option was parsed, all rest arguments, if any, starting with current i->arg - parameters */


/* else, get_opt() returns matched option position (non-negative), encoded as follows: */
#define SHORT_OPT(p)   ((p)<<1)     /* encode short option position in the short options format string  */
#define LONG_OPT(p)    (((p)<<1)+1) /* encode long option index in the long options names array */

/* helper macros for decoding a (non-negative) value returned by the get_opt() */
#define DECODE_OPT(c)  ((c)>>1)     /* decode short or long option position */
#define IS_LONG_OPT(c) ((c)&1)      /* check if long option was matched */


/*--------------------------------------------------------------------------------------------------------
  note: a short options format string may be built by the following method:

#ifdef FEATURE_A
#define SHORT_OPTION_a      SHORT_OPT_MODIFIER("aa", SHORT_OPTION_b)
#else
#define SHORT_OPTION_a      SHORT_OPTION_b
#endif

#ifdef FEATURE_B
#define SHORT_OPTION_b      SHORT_OPT_MODIFIER("b", DASH_LONG_OPTION_c)
#else
#define SHORT_OPTION_b      DASH_LONG_OPTION_c
#endif

#ifdef FEATURE_C
#define DASH_LONG_OPTION_c  SHORT_OPT_MODIFIER("c ", SHORT_OPT_NULL)
#else
#define DASH_LONG_OPTION_c  SHORT_OPT_NULL
#endif

#define SHORT_OPT_NULL      GET_OPT_TEXT("")
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

const GET_OPT_CHAR short_opts[] = SHORT_OPTION_a;
*/

/* where: SHORT_OPT_DEFINER - helper macro used to define short options format string:
 'name' - option name, e.g.: "aa"
 'next' - next option, e.g.: SHORT_OPTION_b */
#define SHORT_OPT_DEFINER(name, next) GET_OPT_TEXT(name) next

/* then, to be able to re-use options macros as (encoded) short option positions (in the short options format string):

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER
*/

/* where: SHORT_OPT_END_POS - returns encoded position of last '\0' in the short options format string: */
#define SHORT_OPT_END_POS(short_opts) SHORT_OPT(sizeof(short_opts)/sizeof(GET_OPT_CHAR) - 1)

/* and SHORT_OPT_ENCODER - helper macro used to obtain encoded position of a short option:
 'name' - option name, e.g.: "aa"
 'next' - next option, e.g.: SHORT_OPTION_b */
#define SHORT_OPT_ENCODER(name, next) SHORT_OPT(DECODE_OPT(next) - (sizeof("" name) - 1))


/* long option description */
struct long_opt_info {
	unsigned len;
	const GET_OPT_CHAR *name;
};

/*--------------------------------------------------------------------------------------------------------
  note: a long options names array may be build by the following method:

#ifdef FEATURE_ALPHA
#define LONG_OPTION_alpha   LONG_OPT_MODIFIER("alpha", 0, LONG_OPTION_beta)
#else
#define LONG_OPTION_alpha   LONG_OPTION_beta
#endif

#ifdef FEATURE_BETA
#define LONG_OPTION_beta    LONG_OPT_MODIFIER("beta", 1, LONG_OPT_NULL)
#else
#define LONG_OPTION_beta    LONG_OPT_NULL
#endif

#define LONG_OPT_NULL       {0, NULL}
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

const struct long_opt_info long_opts[] = {LONG_OPTION_alpha};
*/

/* where: LONG_OPT_DEFINER - helper macro used to define long options names array:
 'name'      - option name, must be non-empty, e.g.: "alpha"
 'has_value' - non-zero if option accepts a value,
 'next'      - next option, e.g.: LONG_OPTION_beta */
#define LONG_OPT_DEFINER(name, has_value, next) {                 \
    sizeof("" name) - 1 + 0*sizeof(int[1-2*(sizeof(name) <= 1)]), \
    (has_value) ? GET_OPT_TEXT("=" name) : GET_OPT_TEXT(name)     \
  }, next

/* then, to be able to re-use options macros as (encoded) long option indexes (in the long options names array):

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER
*/

/* where: LONG_OPT_END_IDX - returns encoded index of last terminating entry in the long options array: */
#define LONG_OPT_END_IDX(long_opts)   LONG_OPT(sizeof(long_opts)/sizeof(struct long_opt_info) - 1)

/* and LONG_OPT_ENCODER - helper macro used to obtain encoded index of a long option:
 'name'      - option name, must be non-empty, e.g.: "alpha"
 'has_value' - non-zero if option accepts a value,
 'next'      - next option, e.g.: LONG_OPTION_beta */
#define LONG_OPT_ENCODER(name, has_value, next)  LONG_OPT(DECODE_OPT(next) - 1)

/*********************************************************************************************************/


#if 0
/* example */
int main(int argc, char *argv[])
{
	/* short options */

#define SHORT_OPTION_a      SHORT_OPT_MODIFIER("aa", SHORT_OPTION_b)
#define SHORT_OPTION_b      SHORT_OPT_MODIFIER("b",  DASH_LONG_OPTION_c)
#define DASH_LONG_OPTION_c  SHORT_OPT_MODIFIER("c ", SHORT_OPT_NULL)

#define SHORT_OPT_NULL      GET_OPT_TEXT("")
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

	static const GET_OPT_CHAR short_opts[] = SHORT_OPTION_a;

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER

	/* long options */

#define LONG_OPTION_alpha   LONG_OPT_MODIFIER("alpha",  0, LONG_OPTION_cetera)
#define LONG_OPTION_cetera  LONG_OPT_MODIFIER("cetera", 1, LONG_OPT_NULL)

#define LONG_OPT_NULL       {0, NULL}
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

	static const struct long_opt_info long_opts[] = {LONG_OPTION_alpha};

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER

	struct opt_info i;
	opt_info_init(&i, argc, argv);

	while (!opt_info_is_end(&i)) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPTION_a:
				/* short option 'a', accepts a value: '-aval', '-a val', may be bundled: '-baval' or '-ba val' */
				if (i.value)
					printf("'a' has value: %s\n", i.value);
				else
					printf("no value specified for 'a'\n");
				break;
			case SHORT_OPTION_b:
				/* short option 'b', do not accepts values, may be bundled: '-b' or '-bcdefg' */
				/* i.value may contain anything - don't check it */
				printf("option 'b'\n");
				break;
			case DASH_LONG_OPTION_c:
				/* short option 'c' is the first letter of long option "cetera" started with one dash, this case cannot happen */
				fprintf(stderr, "assert: short option 'c' was parsed!\n");
				return 1;
			case LONG_OPTION_alpha:
				/* long option "alpha", not expecting a value for it, but one may be specified, e.g.: --alpha=value */
				if (i.value)
					printf("warning: not expecting a value for option 'alpha': %s\n", i.value);
				else
					printf("option 'alpha'\n");
				break;
			case LONG_OPTION_cetera:
				/* long option "cetera", accepts a value, but it may be not specified, e.g.: --cetera */
				if (i.value)
					printf("'cetera' has value: %s\n", i.value);
				else
					printf("no value specified for 'cetera'\n");
				break;
			case OPT_UNKNOWN:
				/* Note: processing of an unknown option can be delegated to another module, e.g.:
				  other_module_process_option(&i);
				    where
				  void other_module_process_option(struct opt_info *i)
				  {
				    switch (get_opt(i, other_short_opts, other_long_opts) {
				      ...
				    }
				  } */
				if (i.sopt)
					printf("unknown short option '%c' in the bundle: '%s'\n", *i.sopt, *i.arg);
				else
					printf("unknown option: '%s'\n", *i.arg);
				opt_skip_unknown(&i); /* skip unknown option, assume it do not expects a value */
				break;
			case OPT_PARAMETER:
				printf("parameter: %s\n", i.value);
				break;
			case OPT_REST_PARAMS:
				for (; !opt_info_is_end(&i); i.arg++)
					printf("parameter: %s\n", *i.arg);
				break;
			default:
				fprintf(stderr, "assert: unexpected get_opt() return value!\n");
				return 1;
		}
	}
	return 0;
}
#endif

#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Nonnull_all_args
A_At(i, A_Out)
#ifdef GET_OPT_ARGV_NZ
A_At(argc, A_In_range(>,0))
A_At(argv, A_In_reads(argc))
#else
A_At(argv, A_In)
#endif
#endif
#ifdef GET_OPT_ARGV_NZ
static void opt_info_init(
	struct opt_info *const i/*out*/,
	const int argc,
	GET_OPT_CHAR *const argv[/*argc*/]/*may be not NULL-terminated*/)
#else
static void opt_info_init(
	struct opt_info *i/*out*/,
	GET_OPT_CHAR *const argv[]/*NULL-terminated*/)
#endif
{
	i->arg = &argv[1];   /* skip program name at argv[0] */
	i->value = NULL;     /* should be accessed only after an option was successfully parsed */
	i->sopt = NULL;      /* no short options bundle encountered yet */
#ifdef GET_OPT_ARGV_NZ
	i->args_end = &argv[argc];
#endif
}

#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Pure_function
A_Nonnull_all_args
A_At(i, A_In)
A_Ret_range(0,1)
A_Check_return
#endif
static int opt_info_is_end(
	const struct opt_info *const i/*in*/)
{
#ifdef GET_OPT_ARGV_NZ
	return i->arg == i->args_end;
#else
	return !i->arg[0];
#endif
}

#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Pure_function
A_Nonnull_all_args
A_At(arg, A_In_z)
A_Ret_range(0,1)
A_Check_return
#endif
static int opt_arg_is_option(
	const GET_OPT_CHAR *const arg/*!=NULL*/)
{
	/* an argument looks like an option:
	  - if it begins with a dash, except the dash alone */
	return GET_OPT_TEXT('-') == *arg && arg[1];
}

#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Check_return
A_Nonnull_arg(1)
A_Ret_range(>=,OPT_REST_PARAMS)
#ifdef GET_OPT_ARGV_NZ
A_Pre_satisfies(i->arg < i->args_end)
#else
A_Pre_satisfies(!!i->arg[0])
#endif
A_At(i, A_Inout)
A_At(short_opts, A_In_opt_z)
A_At(long_opts, A_In_opt)
#endif
static int get_opt(
	struct opt_info *const i/*in,out*/,
	const GET_OPT_CHAR short_opts[]/*NULL?*/,
	const struct long_opt_info long_opts[]/*NULL?*/
) {
	GET_OPT_CHAR *a = i->sopt;
#ifdef GET_OPT_ARGV_NZ
	ASSERT(i->arg < i->args_end);
#else
	ASSERT(i->arg[0]);
#endif
	if (a) {
		/* next short option in the bundle, like "yz" in "-xyz" */
		if (short_opts) {
			const GET_OPT_CHAR *const o = GET_OPT_STRCHR(short_opts, *a);
			if (o) {
				/* short_opts format string must not contain "-" */
				ASSERT(GET_OPT_TEXT('-') != o[0] && GET_OPT_TEXT('-') != o[1]);
				if (GET_OPT_TEXT(' ') == o[1]) {
					/* option is the first letter of a long option started with one dash:
					   "-xfile=abc" as equivalent of "-x -file=abc" or "-x --file=abc" */
					i->arg++; /* skip bundle */
					i->sopt = NULL; /* end of short options bundle */
					goto parse_long_option; /* 'a' points to the option name */
				}
				if (o[1] == *a) {
					/* short option expects a value: "-xfabc" as equivalent of "-x -fabc" */
					i->arg++; /* skip bundle */
					i->sopt = NULL; /* end of short options bundle */
					if (a[1]) {
						/* "-xfabc": set (non-empty) option value */
						i->value = a + 1; /* skip "f" */
					}
					/* no value was specified together with the option, like "-xf", try to get the next argument */
					else if (opt_info_is_end(i))
						i->value = NULL; /* no value: end of args */
					/* don't take the next argument if it looks like an option */
					else if (opt_arg_is_option(*i->arg))
						i->value = NULL; /* no value: next argument is an option */
					else
						i->value = *i->arg++; /* option value, like "-xf abc" or "-xf -", but not "-xf -a" */
				}
				else {
					/* caller must not check for a value of short option if, according
					  to given short_opts format string, the option do not needs a value */
					/*i->value = NULL;*/
					if (a[1])
						i->sopt = a + 1; /* next short option the bundle: "z" in "yz" */
					else {
						i->arg++; /* skip bundle */
						i->sopt = NULL; /* end of short options bundle */
					}
				}
				return SHORT_OPT((int)(unsigned)(o - short_opts));
			}
		}
		/* unknown short option in the short options bundle:
		  i->arg points to the bundle, like "-xyz",
		  i->sopt points to unknown short option in the bundle, like "y" in "-xyz" */
		return OPT_UNKNOWN;
	}
	a = *i->arg++;
	if (!opt_arg_is_option(a)) {
		/* non-option argument (parameter) */
		i->value = a;
		return OPT_PARAMETER;
	}
	if (GET_OPT_TEXT('-') == a[1]) {
		/* long option, like "--help" or "--file=abc" or "--" */
		if (!a[2]) {
			/* no more options after "--" */
			return OPT_REST_PARAMS; /* all arguments starting with current i->arg - parameters */
		}
		a += 2; /* skip "--" */
parse_long_option:
		if (long_opts) {
			/* check if long option specified with a value, like "--file=name" */
			GET_OPT_CHAR *const v = GET_OPT_STRCHR(a, GET_OPT_TEXT('='));
			const unsigned len = (unsigned)((v ? v : GET_OPT_STRCHR(a, GET_OPT_TEXT('\0'))) - a);
			const struct long_opt_info *lo = long_opts;
			for (; lo->len; lo++) {
				if (len == lo->len) {
					const GET_OPT_CHAR *n = lo->name;
					if (GET_OPT_TEXT('=') == *n)
						n++; /* option expects a value */
					ASSERT(*n); /* bad long_opts array: long option name must be non-empty */
					if (!GET_OPT_MEMCMP(a, n, len*sizeof(GET_OPT_CHAR))) {
						if (v) {
							/* "--file=abc": set (can be empty) option value, even if not expecting one */
							i->value = v + 1; /* skip '=' */
						}
						else if (lo->name == n)
							i->value = NULL; /* option do not needs a value */
						/* no value was specified together with the option, like "--help", try to get the next argument */
						else if (opt_info_is_end(i))
							i->value = NULL; /* no value: end of args */
						/* don't take the next argument if it looks like an option */
						else if (opt_arg_is_option(*i->arg))
							i->value = NULL; /* no value: next argument is an option */
						else
							i->value = *i->arg++; /* option value, like "--file name" */
						return LONG_OPT((int)(unsigned)(lo - long_opts));
					}
				}
			}
		}
		/* check if parsing a long option started with one dash in the short options bundle, like "-xfile=abc":
		   1) it should not be a long option started with one dash alone, outside of a bundle, like "-file",
		   2) it should not be a long option started with two dashes, like "--file". */
		if (a != i->arg[-1] + 1/*'-'*/ && GET_OPT_TEXT('-') != i->arg[-1][1])
			i->sopt = a; /* option should be the first letter of a long option started with one dash, but it wasn't matched */
	}
	else if (short_opts) {
		/* short option(s), like "-h" or "-fabc" */
		const GET_OPT_CHAR *const o = GET_OPT_STRCHR(short_opts, a[1]);
		if (o) {
			/* short_opts format string must not contain "-" */
			ASSERT(GET_OPT_TEXT('-') != o[1]);
			if (GET_OPT_TEXT(' ') == o[1]) {
				/* long option started with one dash: "-file" or "-file=abc" */
				a++; /* skip "-" */
				goto parse_long_option;
			}
			if (a[2]) {
				/* may be short option with a value: "-fabc" or multiple short options bundled together: "-xyz" */
				if (a[1] == o[1]) {
					/* "-fabc": set (non-empty) option value */
					i->value = a + 2; /* skip "-f" */
				}
				else {
					/* "-xyz": multiple short options bundled together */
					i->sopt = a + 2; /* next short option in the bundle: "yz" in "-xyz" */
					/* caller must not check for a value of short option if, according
					  to given short_opts format string, the option do not needs a value */
					/*i->value = NULL;*/
					i->arg--; /* will point to the bundle */
				}
			}
			/* no value was passed, like "-f" */
			else if (a[1] == o[1]) {
				/* short option expects a value, like "-f name", try to get the next argument */
				if (opt_info_is_end(i))
					i->value = NULL; /* no value: end of args */
				/* don't take the next argument if it looks like an option */
				else if (opt_arg_is_option(*i->arg))
					i->value = NULL; /* no value: next argument is an option */
				else
					i->value = *i->arg++; /* option value, like "-f abc" or "-f -", but not "-f -a" */
			}
			else {
				/* caller must not check for a value of short option if, according
				  to given short_opts format string, the option do not needs a value */
				/*i->value = NULL;*/
			}
			return SHORT_OPT((int)(unsigned)(o - short_opts));
		}
	}
	i->arg--; /* undo */
	return OPT_UNKNOWN; /* i->arg points to unknown option (or bundle, if i->sopt != NULL) */
}

#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Nonnull_all_args
A_At(i, A_Inout)
#endif
static void opt_skip_unknown(
	struct opt_info *const i/*in,out*/)
{
	/* skip unknown option, assume it do not expects a value */
	if (i->sopt) {
		if (*++(i->sopt))
			return;
		i->sopt = NULL;
	}
	i->arg++;
}

/* check if option value is specified in a separate argument, like: "-o value" or "--option value" */
/* old_arg - value of i->arg before the call to get_opt(), for example:
  struct opt_info i;
  GET_OPT_CHAR *const *old_arg = i.arg;
  int opt = get_opt(&i, short_opts, long_opts);
  if (opt_is_separate_value(&i, old_arg)) {
    opt_unread_separate_value(&i);
    opt = get_opt(&i, short_opts, long_opts);
    ...
  }
*/
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Pure_function
A_Nonnull_all_args
A_Ret_range(0,1)
A_Check_return
A_At(i, A_In)
A_At(old_arg, A_Notnull)
#endif
static int opt_is_separate_value(
	const struct opt_info *const i,
	GET_OPT_CHAR *const *const old_arg)
{
	ASSERT(i->value);
	return (i->arg - 1) != old_arg;
}

/* unread option value specified in a separate argument, like: "-o value" or "--option value" */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Nonnull_all_args
A_At(i, A_Inout)
#endif
static void opt_unread_separate_value(
	struct opt_info *const i/*in,out*/)
{
	i->arg--;
	ASSERT(*i->arg == i->value);
}

/* suppress warnings about unreferenced static functions */
typedef int get_opt_unused_[sizeof(&get_opt)];
typedef int opt_info_init_unused_[sizeof(&opt_info_init)];
typedef int opt_info_is_end_unused_[sizeof(&opt_info_is_end)];
typedef int opt_skip_unknown_unused_[sizeof(&opt_skip_unknown)];
typedef int opt_arg_is_option_unused_[sizeof(&opt_arg_is_option)];
typedef int opt_is_separate_value_unused_[sizeof(&opt_is_separate_value)];
typedef int opt_unread_separate_value_unused_[sizeof(&opt_unread_separate_value)];

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* GET_OPT_INL_INCLUDED */
