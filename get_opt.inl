#ifndef GET_OPT_INL_INCLUDED
#define GET_OPT_INL_INCLUDED

/**********************************************************************************
* Program options and parameters parsing
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt.inl */

/* note: #include <string.h> before this file */

#ifndef GET_OPT_STRCHR
#define GET_OPT_STRCHR(s,c) strchr(s,c)
#endif

#ifndef GET_OPT_STRCMP
#define GET_OPT_STRCMP(s1,s2) strcmp(s1,s2)
#endif

/* this file defines two inline functions:

   1) void opt_info_init(struct opt_info *i, int argc, char *const argv[]);
      - used to initialize options parsing state structure 'opt_info' (declared below)

   2) int get_opt(struct opt_info *i, const char short_opts[], const char *const long_opts[]);
      - get next option value or parameter specified in the command line
*/

/* structure that describes state of options parsing */
struct opt_info {

	/* in/out: next argument to check,
	   initially should be set to &argv[1] (argv[0] is a program name, so skip it) */
	char *const *arg;

	/* in: end of program arguments array,
	   initially should be set to &argv[argc] */
	char *const *args_end;

	/* out: option value or parameter,
	  as option value may be NULL - if no value was provided for the option */
	char *value;

	/* in/out: next short option in the bundle,
	  used when multiple short options are bundled together,
      e.g.: "-abc", which is equivalent of "-a -b -c" */
	char *sopt;
};

/*=========================== Notes: ============================================================================================
|
| 1) get_opt(i) assumes that i->arg < i->args_end, caller must check this prior the call
|
| 2) a non-NULL value may be parsed for a long option that do not expects a value,
|    if a value was provided together with the option, for example: --option=value
|
| 3) a NULL value may be parsed for an option that expects some value
|    - if no value was provided for the option in the command line, e.g.: --option --other
|
| 4) get_opt() accepts *short* options as '\0'-terminated C-string in the following format:
|    . first symbol  - option name, usually a letter or decimal digit, except '-' (dash),
|    . second symbol - option type (optional):
|      a) copy of the first symbol denotes that the option expects a value, like "-oval" or "-o val";
|      b) '-' (dash) denotes that the option is a first letter of a long option started with one dash,
|        for example,  "-myopt" or "-myopt=val", appropriate long option is then looked up in the long options array;
|      c) any other character means that option has no type, this character is the name of another short option.
|
|    Example of short options string: "aabbccde-f"
|
| 5) get_opt() accepts *long* options as NULL-terminated array of options names, in the following format:
|    . each name is a non-empty '\0'-terminated C-string,
|    . the name must not contain a '=' character, except at the beginning, where it denotes that the option expects a value.
|
|    Example of long options array: {"=file","=level","=debug","=output","verbose","trace",NULL}
|
| 6) get_opt() recognizes next special options:
|    - empty short option "-" is a dash option, usually used to denote stdin/stdout, get_opt() returns OPT_DASH,
|    - empty long option "--" denotes end of options, all arguments after it - parameters, get_opt() returns OPT_REST_PARAMS.
|
===============================================================================================================================*/


/* get_opt(i) returns next error codes (negative values): */

#define OPT_UNKNOWN     -1 /* i->arg points to unknown option argument,
                              if i->sopt != NULL, then it points to unknown short option character in the short options bundle */

#define OPT_BAD_BUNDLE  -2 /* i->arg points to the whole short options bundle argument,
                              i->sopt denotes short option that cannot be bundled according to the short options format string:
                              either option expects a value or option is a first letter of a long option started with one dash */

#define OPT_PARAMETER   -3 /* i->value points to non-NULL parameter value */

#define OPT_DASH        -4 /* '-' (dash) option was parsed (this option usually used to specify stdin/stdout) */

#define OPT_REST_PARAMS -5 /* '--' option was parsed, all rest arguments starting with i->arg and until i->args_end - parameters */


/* else, get_opt() returns matched option position (non-negative), encoded as follows: */
#define SHORT_OPT(p)   ((p)<<1)     /* encode short option position in short options format string  */
#define LONG_OPT(p)    (((p)<<1)+1) /* encode long option index in long options names array */

/* helper macros for decoding a value returned by the get_opt() */
#define DECODE_OPT(c)  ((c)>>1)     /* decode short or long option position */
#define IS_LONG_OPT(c) ((c)&1)      /* check if long option was matched */


/*--------------------------------------------------------------------------------------------------------
  note: short options may be defined by the following method:

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
#define DASH_LONG_OPTION_c  SHORT_OPT_MODIFIER("c-", SHORT_OPT_NULL)
#else
#define DASH_LONG_OPTION_c  SHORT_OPT_NULL
#endif

#define SHORT_OPT_NULL      ""
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

const char short_opts[] = SHORT_OPTION_a;
*/

/* where SHORT_OPT_DEFINER - helper macro used to define short options format string: */
#define SHORT_OPT_DEFINER(a,b) a b

/* to make option define to return encoded short option position in short options format string, use this:

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER
*/

/* where SHORT_OPT_END_POS - defines encoded position of last '\0' entry in short options format string: */
#define SHORT_OPT_END_POS(short_opts)   SHORT_OPT(sizeof(short_opts)-1)

/* and SHORT_OPT_ENCODER - helper macro used to obtain encoded position of short option: */
#define SHORT_OPT_ENCODER(a,b)          SHORT_OPT(DECODE_OPT(b)-(sizeof("" a)-1))


/*--------------------------------------------------------------------------------------------------------
  note: long options may be defined by the following method:

#ifdef FEATURE_ALPHA
#define LONG_OPTION_alpha   LONG_OPT_MODIFIER("alpha", LONG_OPTION_beta)
#else
#define LONG_OPTION_alpha   LONG_OPTION_beta
#endif

#ifdef FEATURE_BETA
#define LONG_OPTION_beta    LONG_OPT_MODIFIER("=beta", LONG_OPT_NULL)
#else
#define LONG_OPTION_beta    LONG_OPT_NULL
#endif

#define LONG_OPT_NULL       NULL
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

const char *const long_opts[] = {LONG_OPTION_alpha};
*/

/* where LONG_OPT_DEFINER - helper macro used to define long options names array: */
#define LONG_OPT_DEFINER(a,b) a,b

/* to make option define to return long option index in long options names array, use this:

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER
*/

/* where LONG_OPT_END_IDX - defines encoded index of last NULL entry in long options names array: */
#define LONG_OPT_END_IDX(long_opts)   LONG_OPT(sizeof(long_opts)/sizeof(const char*)-1)

/* and LONG_OPT_ENCODER - helper macro used to obtain encoded index of long option: */
#define LONG_OPT_ENCODER(a,b)         LONG_OPT(DECODE_OPT(b)-1)

/*********************************************************************************************************/


#if 0
/* example */
int main(int argc, char *argv[])
{
	/* short options */

#define SHORT_OPTION_a      SHORT_OPT_MODIFIER("aa", SHORT_OPTION_b)
#define SHORT_OPTION_b      SHORT_OPT_MODIFIER("b",  DASH_LONG_OPTION_c)
#define DASH_LONG_OPTION_c  SHORT_OPT_MODIFIER("c-", SHORT_OPT_NULL)

#define SHORT_OPT_NULL      ""
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

	static const char short_opts[] = SHORT_OPTION_a;

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER

	/* long options */

#define LONG_OPTION_alpha   LONG_OPT_MODIFIER("alpha",   LONG_OPTION_cetera)
#define LONG_OPTION_cetera  LONG_OPT_MODIFIER("=cetera", LONG_OPT_NULL)

#define LONG_OPT_NULL       NULL
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

	static const char *const long_opts[] = {LONG_OPTION_alpha};

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER

	struct opt_info i;
	opt_info_init(&i, argc, argv);

	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPTION_a:
				/* short option 'a', may be with value: '-aval' or '-a val' */
				if (i.value)
					printf("'a' has value: %s\n", i.value);
				else
					printf("no value provided for 'a'\n");
				break;
			case SHORT_OPTION_b:
				/* short option 'b', without a value, may be bundled: '-b' or '-bcdefg' */
				if (i.value) {
					fprintf(stderr, "assert: a value was parsed for short option 'b'!\n");
					return 1;
				}
				printf("option 'b'\n");
				break;
			case DASH_LONG_OPTION_c:
				/* short option 'c' is the first letter of a long option started with one dash, cannot be returned */
				fprintf(stderr, "assert: short option 'c' was parsed!\n");
				return 1;
			case LONG_OPTION_alpha:
				/* long option "alpha", not expecting a value for it, but the value may be provided, e.g.: --alpha=value */
				if (i.value)
					printf("warning: not expecting a value for option 'alpha': %s\n", i.value);
				else
					printf("option 'alpha'\n");
				break;
			case LONG_OPTION_cetera:
				/* long option "cetera", expecting a value for it, but the value may not be provided, e.g.: --cetera --alpha */
				if (i.value)
					printf("'cetera' has value: %s\n", i.value);
				else
					printf("no value provided for 'cetera'\n");
				break;
			case OPT_UNKNOWN:
				if (i.sopt)
					printf("unknown short option '%c' in the bundle: '%s'\n", *i.sopt, *i.arg);
				else
					printf("unknown option: '%s'\n", *i.arg);
				i.arg++;       /* skip unknown option */
				i.sopt = NULL; /* and the rest of the bundle */
				break;
			case OPT_BAD_BUNDLE:
				printf("short option '%c' cannot be bundled: '%s'\n", *i.sopt, *i.arg);
				i.arg++;       /* skip bad option */
				i.sopt = NULL; /* and the rest of the bundle */
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
				fprintf(stderr, "assert: unexpected get_opt() return value!\n");
				return 1;
		}
	}
	return 0;
}
#endif

#ifdef A_Nonnull_all_args
A_Nonnull_all_args
#endif
static void opt_info_init(
#ifdef A_Out
	A_Out
#endif
	struct opt_info *i,
#ifdef A_In_range
	A_In_range(>,0)
#endif
	int argc,
#ifdef A_In_reads
	A_In_reads(argc)
#endif
	char *const argv[]
) {
	i->arg = &argv[1];         /* skip program name at argv[0] */
	i->args_end = &argv[argc];
	i->sopt = NULL;
	/*i->value = NULL;*/       /* should be accessed only after an option was successfully parsed */
}

#ifdef A_Nonnull_arg
A_Nonnull_arg(1)
#endif
#ifdef A_Ret_range
A_Ret_range(>=,OPT_REST_PARAMS)
#endif
#ifdef A_Pre_satisfies
A_Pre_satisfies(i->arg < i->args_end)
#endif
static int get_opt(
#ifdef A_Inout
	A_Inout
#endif
	struct opt_info *i,
#ifdef A_In_opt
	A_In_opt_z
#endif
	const char short_opts[]/*NULL?*/,
#ifdef A_In_opt
	A_In_opt
#endif
	const char *const long_opts[]/*NULL?*/
) {
	char *a = i->sopt;
	if (a) {
		/* next short option in the bundle, like "bc" in "-abc" */
		if ('-' != *a) {
			if (short_opts) {
				const char *o = GET_OPT_STRCHR(short_opts, *a);
				if (o) {
					if ('-' != o[1] && o[1] != *a) {
						i->sopt = a[1]
							? a + 1 /* next short option the bundle: "c" in "bc" */
							: NULL; /* end of short options bundle */
						return SHORT_OPT((int)(o - short_opts));
					}
					/* either option is a first name of long option or option expects a value,
					   such option cannot be bundled with other short options */
					i->arg--;
					return OPT_BAD_BUNDLE;
				}
			}
		}
		else {
			/* not expecting a '-' (dash) in short options bundle */
			i->arg--;
			return OPT_BAD_BUNDLE;
		}
	}
	else {
		a = *i->arg++;
		if ('-' != *a) {
			/* non-option argument (parameter) */
			i->value = a;
			return OPT_PARAMETER;
		}
		if (!a[1])
			return OPT_DASH; /* '-' (dash) option */
		if ('-' == a[1]) {
			/* long option, like "--help" or "--file=f" or "--" */
			if (!a[2]) {
				/* no more options after "--" */
				return OPT_REST_PARAMS; /* all arguments starting with i->arg - parameters */
			}
			a += 2; /* skip "--" */
parse_long_option:
			if (long_opts) {
				char *v = GET_OPT_STRCHR(a, '=');
				if (v)
					*v = '\0'; /* temporary terminate option name */
				{
					const char *const *lo = long_opts;
					for (; *lo; lo++) {
						const char *n = *lo;
						if ('=' == *n)
							n++; /* option expects a value */
						if (!GET_OPT_STRCMP(a, n)) {
							if (v) {
								/* "--file=f": set (may be empty) option value, even if not expecting one */
								*v = '='; /* restore */
								i->value = v + 1; /* skip '=' */
							}
							else {
								/* no value is passed, like "--help" */
								if (n != *lo && i->arg != i->args_end) {
									/* option expects a value, try to get one */
									a = *i->arg;
									if ('-' != *a) {
										i->arg++;
										i->value = a; /* option value */
									}
									else
										i->value = NULL; /* no value provided: next argument is an another option */
								}
								else
									i->value = NULL; /* not expecting a value for the option or end of args */
							}
							return LONG_OPT((int)(lo - long_opts));
						}
					}
				}
				if (v)
					*v = '='; /* restore */
			}
		}
		else if (short_opts) {
			/* short option, like "-h" or "-ffile" */
			const char *o = GET_OPT_STRCHR(short_opts, a[1]);
			if (o) {
				if ('-' == o[1]) {
					/* long option: "-myopt" */
					a++; /* skip "-" */
					goto parse_long_option;
				}
				if (a[2]) {
					/* may be short option with a value: "-ffile" or multiple short options bundled together: -abc */
					if (a[1] == o[1]) {
						/* "-ffile": set (non-empty) option value */
						i->value = a + 2; /* skip "-a" */
					}
					else {
						/* "-abc": multiple short options bundled together */
						i->sopt = a + 2; /* next short option in the bundle: "bc" in "-abc" */
						i->value = NULL; /* option do not expects a value */
					}
				}
				/* no value was passed, like "-h" */
				else if (a[1] == o[1] && i->arg != i->args_end) {
					/* option expects a value, try to get one */
					a = *i->arg;
					if ('-' != *a) {
						i->arg++;
						i->value = a; /* option value */
					}
					else
						i->value = NULL; /* no value provided: next argument is an another option */
				}
				else
					i->value = NULL; /* option do not expects a value or end of args */
				return SHORT_OPT((int)(o - short_opts));
			}
		}
	}
	i->arg--;
	return OPT_UNKNOWN; /* i->arg points to unknown option */
}

#endif /* GET_OPT_INL_INCLUDED */
