#ifndef GET_OPT_INL_INCLUDED
#define GET_OPT_INL_INCLUDED

/**********************************************************************************
* Program options and parameters parsing
* Copyright (C) 2012-2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt.inl */

/* note: #include <string.h> before this file */

#ifndef GET_OPT_CHAR
#define GET_OPT_CHAR char
#endif

#ifndef GET_OPT_TEXT
#define GET_OPT_TEXT(text) text
#endif

#ifndef GET_OPT_STRCHR
#define GET_OPT_STRCHR(s,c) strchr(s,c)
#endif

#ifndef GET_OPT_STRCMP
#define GET_OPT_STRCMP(s1,s2) strcmp(s1,s2)
#endif

#ifndef ASSERT
#define ASSERT(x) ((void)0)
#endif

/* this file defines two inline functions:

   1) void opt_info_init(struct opt_info *i, int argc, GET_OPT_CHAR *const argv[]);
      - used to initialize options parsing state structure 'opt_info' (declared below)

   2) int get_opt(struct opt_info *i, const GET_OPT_CHAR short_opts[], const GET_OPT_CHAR *const long_opts[]);
      - get next option value or parameter specified in the command line
*/

/* structure that describes state of options parsing */
struct opt_info {

	/* in/out: next argument to check,
	   except if sopt != NULL - the arg points to the whole short options bundle argument,
	   initially should be set to &argv[1] (argv[0] is a program name, so skip it) */
	GET_OPT_CHAR *const *arg;

	/* in: end of program arguments array,
	   initially should be set to &argv[argc] */
	GET_OPT_CHAR *const *args_end;

	/* out: option value or parameter,
	  as option value may be NULL - if no value was specified for the option */
	GET_OPT_CHAR *value;

	/* in/out: next short option in the bundle pointed by arg,
	  used when multiple short options are bundled together,
      e.g.: "-xyz-", which is equivalent of "-x -y -z -" */
	GET_OPT_CHAR *sopt;
};

/*=========================== Notes: ============================================================================================
|
| 1) get_opt(i) assumes that i->arg < i->args_end, caller must check this prior the call
|
| 2) a non-NULL value may be parsed for a long option that do not expects a value
|    - if a value was specified together with the option, for example: "--option=value"
|
| 3) a NULL value may be parsed for an option that expects some value
|    - if no value was specified for the option at the end of the command line
|
| 4) get_opt() accepts *short* options as '\0'-terminated C-string in the following format:
|    . first symbol  - option name, usually a letter or decimal digit, possible a dash '-', except space character,
|    . second symbol - option type (optional, not for dash option):
|      a) copy of the first symbol denotes that the option expects a value, like "-oval" or "-o val";
|      b) space character denotes that the option is the first letter of a long option started with one dash,
|        for example, "-file" or "-file=abc", appropriate long option is then looked up in the long options array;
|      c) any other character means that the option has no type, this character is the name of another short option.
|
|    Example of short options format string: "xxy z-"
|
| 5) get_opt() accepts *long* options as NULL-terminated array of options names, in the following format:
|    . each name is a non-empty '\0'-terminated C-string,
|    . the name must not contain a '=' character, except at the beginning, where it denotes that the option expects a value.
|
|    Example of long options array: {"=file","=level","=debug","=output","verbose","trace",NULL}
|
| 6) special option "--" denotes end of options, all arguments after it - parameters,
|    get_opt() returns OPT_REST_PARAMS
|
| 7) if an option expects a value, but a value wasn't specified together with the option (like "-fabc" or "--file=abc),
|    get_opt() takes next argument after the option as its value, for example: "--file --help" is equivalent to "--file=--help"
|
===============================================================================================================================*/


/* get_opt(i) returns next error codes (negative values): */

#define OPT_UNKNOWN     -1 /* i->arg points to unknown option argument (may be bundle),
                              if i->sopt != NULL, then it points to unknown short option character in the short options bundle */

#define OPT_PARAMETER   -2 /* i->value points to non-NULL parameter value */

#define OPT_REST_PARAMS -3 /* "--" option was parsed, all rest arguments starting with current i->arg - parameters */


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
#define DASH_LONG_OPTION_c  SHORT_OPT_MODIFIER("c ", SHORT_OPT_NULL)
#else
#define DASH_LONG_OPTION_c  SHORT_OPT_NULL
#endif

#define SHORT_OPT_NULL      GET_OPT_TEXT("")
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

const GET_OPT_CHAR short_opts[] = SHORT_OPTION_a;
*/

/* where SHORT_OPT_DEFINER - helper macro used to define short options format string:
 a - option name, e.g.: "aa"
 b - next option, e.g.: SHORT_OPTION_b */
#define SHORT_OPT_DEFINER(a,b) GET_OPT_TEXT(a) b

/* to make option define to return encoded short option position in short options format string, use this:

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER
*/

/* where SHORT_OPT_END_POS - defines encoded position of last '\0' entry in short options format string: */
#define SHORT_OPT_END_POS(short_opts)   SHORT_OPT(sizeof(short_opts)/sizeof(GET_OPT_CHAR) - 1)

/* and SHORT_OPT_ENCODER - helper macro used to obtain encoded position of short option:
 a - option name, e.g.: "aa"
 b - next option, e.g.: SHORT_OPTION_b */
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

const GET_OPT_CHAR *const long_opts[] = {LONG_OPTION_alpha};
*/

/* where LONG_OPT_DEFINER - helper macro used to define long options names array:
 a - option name, e.g.: "alpha"
 b - next option, e.g.: LONG_OPTION_beta */
#define LONG_OPT_DEFINER(a,b) GET_OPT_TEXT(a),b

/* to make option define to return encoded long option index in long options names array, use this:

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER
*/

/* where LONG_OPT_END_IDX - defines encoded index of last NULL entry in long options names array: */
#define LONG_OPT_END_IDX(long_opts)   LONG_OPT(sizeof(long_opts)/sizeof(const GET_OPT_CHAR*) - 1)

/* and LONG_OPT_ENCODER - helper macro used to obtain encoded index of long option:
 a - option name, e.g.: "alpha"
 b - next option, e.g.: LONG_OPTION_beta */
#define LONG_OPT_ENCODER(a,b)         LONG_OPT(DECODE_OPT(b)-1)

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

#define LONG_OPTION_alpha   LONG_OPT_MODIFIER("alpha",   LONG_OPTION_cetera)
#define LONG_OPTION_cetera  LONG_OPT_MODIFIER("=cetera", LONG_OPT_NULL)

#define LONG_OPT_NULL       NULL
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

	static const GET_OPT_CHAR *const long_opts[] = {LONG_OPTION_alpha};

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER

	struct opt_info i;
	opt_info_init(&i, argc, argv);

	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPTION_a:
				/* short option 'a', may be with value: '-aval', '-a val', may be bundled: '-baval' or '-ba val' */
				if (i.value)
					printf("'a' has value: %s\n", i.value);
				else
					printf("no value specified for 'a'\n");
				break;
			case SHORT_OPTION_b:
				/* short option 'b', without a value (which cannot be parsed), may be bundled: '-b' or '-bcdefg' */
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
				/* long option "alpha", not expecting a value for it, but the value may be specified, e.g.: --alpha=value */
				if (i.value)
					printf("warning: not expecting a value for option 'alpha': %s\n", i.value);
				else
					printf("option 'alpha'\n");
				break;
			case LONG_OPTION_cetera:
				/* long option "cetera", expecting a value for it, but the value may not be specified, e.g.: --cetera */
				if (i.value)
					printf("'cetera' has value: %s\n", i.value);
				else
					printf("no value specified for 'cetera'\n");
				break;
			case OPT_UNKNOWN:
				if (i.sopt)
					printf("unknown short option '%c' in the bundle: '%s'\n", *i.sopt, *i.arg);
				else
					printf("unknown option: '%s'\n", *i.arg);
				i.arg++;       /* skip unknown option */
				i.sopt = NULL; /* and the rest of the bundle, if any */
				break;
			case OPT_PARAMETER:
				printf("parameter: %s\n", i.value);
				break;
			case OPT_REST_PARAMS:
				do {
					printf("parameter: %s\n", *i.arg);
				} while (++i.arg < i.args_end);
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
	GET_OPT_CHAR *const argv[]
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
	const GET_OPT_CHAR short_opts[]/*NULL?*/,
#ifdef A_In_opt
	A_In_opt
#endif
	const GET_OPT_CHAR *const long_opts[]/*NULL?*/
) {
	GET_OPT_CHAR *a = i->sopt;
	if (a) {
		/* next short option in the bundle, like "yz-" in "-xyz-" */
		if (short_opts) {
			const GET_OPT_CHAR *o = GET_OPT_STRCHR(short_opts, *a);
			if (o) {
				if (GET_OPT_TEXT(' ') == o[1]) {
					/* option is the first letter of a long option started with one dash:
					   "-xfile=abc" as equivalent of "-x -file=abc" or "-x --file=abc" */
					ASSERT(GET_OPT_TEXT('-') != *a); /* dash option cannot begin a long option */
					i->arg++; /* skip bundle */
					i->sopt = NULL; /* end of short options bundle */
					goto parse_long_option;
				}
				if (o[1] == *a) {
					/* short option expects a value: "-xfabc" as equivalent of "-x -fabc" */
					ASSERT(GET_OPT_TEXT('-') != *a); /* dash option cannot have a value */
					i->arg++; /* skip bundle */
					i->sopt = NULL; /* end of short options bundle */
					if (a[1]) {
						/* "-xfabc": set (non-empty) option value */
						i->value = a + 1; /* skip "f" */
					}
					/* no value was specified together with the option, like "-xf", try to get the next argument */
					else if (i->arg != i->args_end)
						i->value = *i->arg++; /* option value, like "-xf abc" or "-xf -" or "-xf --abc" */
					else
						i->value = NULL; /* no value: end of args */
				}
				else if (a[1])
					i->sopt = a + 1; /* next short option the bundle: "z-" in "yz-" */
				else {
					i->arg++; /* skip bundle */
					i->sopt = NULL; /* end of short options bundle */
				}
				return SHORT_OPT((int)(o - short_opts));
			}
		}
		return OPT_UNKNOWN; /* unknown short option in the short options bundle */
	}
	a = *i->arg++;
	if (GET_OPT_TEXT('-') != *a) {
		/* non-option argument (parameter) */
		i->value = a;
		return OPT_PARAMETER;
	}
	if (!a[1]) {
		/* '-' (dash) option */
		if (short_opts) {
			const GET_OPT_CHAR *o = GET_OPT_STRCHR(short_opts, GET_OPT_TEXT('-'));
			if (o) {
				/* dash option cannot begin a long option and cannot have a value */
				ASSERT(GET_OPT_TEXT(' ') != o[1] && GET_OPT_TEXT('-') != o[1]);
				return SHORT_OPT((int)(o - short_opts));
			}
		}
	}
	else if (GET_OPT_TEXT('-') == a[1]) {
		/* long option, like "--help" or "--file=abc" or "--" */
		if (!a[2]) {
			/* no more options after "--" */
			return OPT_REST_PARAMS; /* all arguments starting with current i->arg - parameters */
		}
		a += 2; /* skip "--" */
parse_long_option:
		if (long_opts) {
			GET_OPT_CHAR *v = GET_OPT_STRCHR(a, GET_OPT_TEXT('='));
			if (v)
				*v = GET_OPT_TEXT('\0'); /* temporary terminate option name */
			{
				const GET_OPT_CHAR *const *lo = long_opts;
				for (; *lo; lo++) {
					const GET_OPT_CHAR *n = *lo;
					if (GET_OPT_TEXT('=') == *n)
						n++; /* option expects a value */
					ASSERT(*n); /* long option name must be non-empty */
					if (!GET_OPT_STRCMP(a, n)) {
						if (v) {
							/* "--file=abc": set (may be empty) option value, even if not expecting one */
							*v = GET_OPT_TEXT('='); /* restore */
							i->value = v + 1; /* skip '=' */
						}
						/* no value was specified together with the option, like "--help" */
						else if (n != *lo && i->arg != i->args_end)
							i->value = *i->arg++; /* option expects a value, take the next argument as the option value */
						else
							i->value = NULL; /* not expecting a value for the option or end of args */
						return LONG_OPT((int)(lo - long_opts));
					}
				}
			}
			if (v)
				*v = GET_OPT_TEXT('='); /* restore */
		}
		/* check if parsing a long option started with one dash in short options bundle, like "-xfile=abc":
		   1) it should not be a long option started with one dash alone, like "-file"
		   2) it should not be a long option started with two dashes, like "--file" */
		if (a != i->arg[-1] + 1/*'-'*/ && GET_OPT_TEXT('-') != i->arg[-1][1])
			i->sopt = a; /* option should be the first letter of a long option started with one dash, but it wasn't matched */
	}
	else if (short_opts) {
		/* short option, like "-h" or "-fabc" */
		const GET_OPT_CHAR *o = GET_OPT_STRCHR(short_opts, a[1]);
		if (o) {
			if (GET_OPT_TEXT(' ') == o[1]) {
				/* long option: "-file" or "-file=abc" */
				a++; /* skip "-" */
				goto parse_long_option;
			}
			if (a[2]) {
				/* may be short option with a value: "-fabc" or multiple short options bundled together: "-xyz-" */
				if (a[1] == o[1]) {
					/* "-fabc": set (non-empty) option value */
					i->value = a + 2; /* skip "-f" */
				}
				else {
					/* "-xyz-": multiple short options bundled together */
					i->sopt = a + 2; /* next short option in the bundle: "yz-" in "-xyz-" */
					i->value = NULL; /* option do not expects a value */
					i->arg--; /* will point to the bundle */
				}
			}
			/* no value was passed, like "-f" */
			else if (a[1] == o[1] && i->arg != i->args_end)
				i->value = *i->arg++; /* option expects a value, take the next argument as the option value */
			else
				i->value = NULL; /* option do not expects a value or end of args */
			return SHORT_OPT((int)(o - short_opts));
		}
	}
	i->arg--;
	return OPT_UNKNOWN; /* i->arg points to unknown option (or bundle, if i->sopt != NULL) */
}

#endif /* GET_OPT_INL_INCLUDED */
