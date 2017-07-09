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

/*

1) init opt_info structure:

void opt_info_init(
	struct opt_info *i,
	int argc,
	char *const argv[]);

2) get next option value or parameter specified in a command line:

int get_opt(
	struct opt_info *i,
	const char short_opts[],
	const char *const long_opts[]);

*/

/* structure that describes state of options parsing */
struct opt_info {

	/* in/out: current argument,
	   initially should be set to &argv[1] (argv[0] is a program name, so skip it) */
	char *const *arg;

	/* in: end of program arguments array,
	   initially should be set to &argv[argc] */
	char *const *args_end;

	/* out: option value or parameter,
	  as option value may be NULL - if no value was provided for that option */
	char *value;

	/* in/out: next short option in a bundle,
	  used when multiple short options are bundled together: -abc */
	char *sopt;
};

/* get_opt() normally returns a value >= 0 - matched option position, encoded as follows: */
#define SHORT_OPT(p)   ((p)<<1)     /* encode short option position in short options format string  */
#define LONG_OPT(p)    (((p)<<1)+1) /* encode long option index in long options format array */
#define DECODE_OPT(c)  ((c)>>1)     /* decode short or long option position */
#define IS_LONG_OPT(c) ((c)&1)      /* check if long option was matched */

/* get_opt() also may return next error codes: */
#define OPT_UNKNOWN     -1 /* i->arg points to unknown option argument, check i->sopt - it may point to unknown short option character */
#define OPT_BAD_BUNDLE  -2 /* i->sopt denotes short option that cannot be bundled, i->arg points to whole short options bundle argument */
#define OPT_PARAMETER   -3 /* i->value points to non-NULL parameter value */
#define OPT_DASH        -4 /* '-' (dash) option was parsed (this option usually used to specify stdin/stdout) */
#define OPT_REST_PARAMS -5 /* after '--' option, all rest arguments starting with i->arg and until i->args_end - are parameters */

/* Notes:

  1) get_opt(i) expects that i->arg < i->args_end,

  2) a non-NULL value may be parsed for a long option that do not expects a value,
     if a value was provided together with the option, for example: --option=value

  3) a NULL value may be parsed for an option that expects some value
     - if no value was provided for the option in the command line,

  4) get_opt() accepts short options as '\0'-terminated C-string, formatted as follows:
      first symbol - option name, usually a letter or decimal digit, except '-' (dash),
      second symbol - option type (optional):
       copy of the first symbol denotes that option expects a value (like "-oval" or "-o val", but value may be not provided),
       '-' (dash) denotes that option is a first letter of long option started with a dash (like "-myopt" or "-myopt=val"),
       other character means that option has no type, this character is a name of another option,
     example of short options string: "aabbccde-f"

  5) get_opt() accepts long options as NULL-terminated array of options names, in the following format:
      each name is a (non-empty) '\0'-terminated C-string,
      name must not contain a '=' character, except at the beginning, where it denotes that the option expects a value,
     example of long options array: {"=file","=level","=debug","=output","verbose","trace",NULL}

  6) special options:
      empty short option "-" is a dash option, usually used to denote stdin/stdout, get_opt() returns OPT_DASH,
      empty long option "--" denotes end of options, all arguments after it - are parameters, get_opt() returns OPT_REST_PARAMS.
*/

#if 0
/* example */
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
	i->arg = &argv[1];         /* skip program name */
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
	A_In_opt
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
					/* either option is a first name of long option or
					   option expects a value, this option cannot be bundled with other short options */
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
				return OPT_REST_PARAMS; /* all arguments starting with i->arg are parameters */
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
									/* option wants a value */
									a = *i->arg;
									if ('-' != *a) {
										i->arg++;
										i->value = a; /* option value */
									}
									else
										i->value = NULL; /* no value provided: next is an another option */
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
					/* may be short option with a value or multiple short options bundled together: -abc */
					a += 2; /* skip "-a" */
					if (a[-1] == o[1]) {
						/* "-ffile": set (non-empty) option value */
						i->value = a;
					}
					else {
						i->sopt = a; /* next short option in a bundle: "bc" in "-abc" */
						i->value = NULL; /* option do not expects a value */
					}
				}
				else {
					/* no value was passed, like "-h" */
					if (a[1] == o[1] && i->arg != i->args_end) {
						/* option wants a value */
						a = *i->arg;
						if ('-' != *a) {
							i->arg++;
							i->value = a; /* option value */
						}
						else
							i->value = NULL; /* no value provided: next is an another option */
					}
					else
						i->value = NULL; /* not expecting a value for the option or end of args */
				}
				return SHORT_OPT((int)(o - short_opts));
			}
		}
	}
	i->arg--;
	return OPT_UNKNOWN; /* i->arg points to unknown option */
}

#endif /* GET_OPT_INL_INCLUDED */
