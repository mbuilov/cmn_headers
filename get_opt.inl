#ifndef GET_OPT_INL_INCLUDED
#define GET_OPT_INL_INCLUDED

/**********************************************************************************
* Program options parsing
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt.inl */

/* defines two functions:
  get_opt()             - get next option value specified in command line,
  is_long_opt_matched() - check if long option is matched, may be used while processing get_opt() result. */

/* structure that describes state of options parsing */
struct opt_info {

	/* current argument,
	   initially set to &argv[1] (argv[0] is a program name, so skip it) */
	char *const *arg;

	/* end of program arguments array,
	   initially set to &argv[argc] */
	char *const *args_end;

	/* out: option value or parameter,
	  as option value may be NULL - if no value provided for an option */
	char *value;
};

/* get_opt() normally returns value >= 0 - parsed option number, but also may return next values: */
#define OPT_UNKNOWN     -1 /* i->arg points to an unknown option */
#define OPT_PARAMETER   -2 /* i->value points to non-NULL parameter value */
#define OPT_REST_PARAMS -3 /* all arguments starting with i->arg and until i->args_end - are parameters */

/* Notes:

  1) get_opt(i) expects that i->arg < i->args_end,

  2) a non-NULL value may be parsed for an option that do not expects one,
     if a value was provided together with the option, for example:
     short option: -ovalue
     long option:  --option=value

  3) the NULL value may be parsed for an option that expects a value,
     if a value was not provided for the option in the command line,

  4) get_opt() accepts short options '\0'-terminated C-string in format:
      two symbols for one option (so string length must be even),
     where:
      first symbol - option name, usually a letter or decimal digit, but may be any character except colon, space and semicolon,
      second symbol - option type:
        ':' (colon) denotes that option expects a value ("-oval" or "-o val", but value may be not provided),
        ' ' (space) denotes that option do not expects a value (but value may be set, like for "-oval"),
        ';' (semicolon) denotes that option is a first letter of long option (like "-myopt" or "-myopt=val"),
      special option name: dash '-' denotes option without name, usually used to specify stdin/stdout, option type should be space,
      spaces may be used to adjust returned option number
      example of short options string: "a:  b:c:  d:e f;g - h;"

  5) get_opt() accepts long options as NULL-terminated array of options names in format:
      each name is a '\0'-terminated C-string,
      name must not contain '=' character, except at the beginning, where it denotes that an option expects a value,
      empty names may be used to adjust returned option number,
      example of long options array: {"","","=file","=level","","=debug","=output","verbose","","trace",NULL}

  6) empty long option "--" denotes end of options, all arguments after it - are parameters.
*/

#if 0
/* example */
int main(int argc, char *argv[])
{
	static const char short_opts[] = "a:  - ";
	static const char *const long_opts[] = {"","=file","beta",NULL};
	struct opt_info i;
	i.arg       = &argv[1];
	i.args_end  = &argv[argc];
	i.opts      = short_opts;
	i.long_opts = long_opts;
	while (i.arg < i.args_end) {
		switch (get_opt(&i)) {
			case 0: /* short option 'a' */
				if (i.value)
					printf("'a' has value: %s\n", i.value);
				else
					printf("no value provided for 'a'\n");
				break;
			case 1: /* long option "file" */
				if (i.value)
					printf("'file' has value: %s\n", i.value);
				else
					printf("no value provided for 'file'\n");
				break;
			case 2: { /* short option '-' or long option "beta" */
				const char *opt_name = is_long_opt_matched(i.arg) ? "beta" : "-";
				if (i.value)
					printf("not expecting a value for option '%s': %s\n", opt_name, i.value);
				else
					printf("option '%s'\n", opt_name);
				break;
			}
			case OPT_UNKNOWN:
				printf("unknown option: '%s'\n", *i.arg++);
				break;
			case OPT_PARAMETER:
				printf("parameter: '%s'\n", i.value);
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
	const char *short_opts,
#ifdef A_In_opt
	A_In_opt
#endif
	const char *long_opts[]
) {
	char *a = *i->arg++;
	if ('-' != *a) {
		/* non-option argument (parameter) */
		i->value = a;
		return OPT_PARAMETER;
	}
	if ('-' == a[1]) {
		/* long option, like "--help" or "--file=f" or "--" */
		if (!a[2]) {
			/* no more options after "--" */
			return OPT_REST_PARAMS; /* all arguments starting with i->arg are parameters */
		}
		a += 2; /* skip "--" */
parse_long_option:
		if (long_opts) {
			char *v = strchr(a, '=');
			if (v)
				*v = '\0'; /* temporary terminate option name */
			{
				const char *const *lo = long_opts;
				for (; *lo; lo++) {
					const char *beg = *lo;
					if (*beg) {
						const char *n = beg;
						if ('=' == *n)
							n++; /* option expects a value */
						if (!strcmp(a, n)) {
							if (v) {
								/* "--file=f": set (may be empty) option value, even if not expecting one */
								*v = '='; /* restore */
								i->value = v + 1; /* skip '=' */
							}
							else {
								/* no value passed, like "--help" */
								if (n != beg && i->arg != i->args_end) {
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
							return (int)(lo - long_opts);
						}
					}
				}
			}
			if (v)
				*v = '='; /* restore */
		}
	}
	else if (short_opts) {
		/* short option, like "-h" or "-ffile" or "-" */
		const char *o = strchr(short_opts, a[1] ? a[1] : '-');
		if (o) {
			if (a[1] && a[2]) {
				if (';' == o[1]) {
					/* long option: "-myopt" */
					a++; /* skip "-" */
					goto parse_long_option;
				}
				/* "-ffile": set (non-empty) option value, even if not expecting one */
				i->value = a + 2; /* skip "-f" */
			}
			else {
				/* no value passed, like "-h" or "-" */
				if (':' == o[1] && i->arg != i->args_end) {
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
			return (int)(o - short_opts)/2;
		}
	}
	i->arg--;
	return OPT_UNKNOWN; /* i->arg points to unknown (may be long) option */
}

#ifdef A_Nonnull_all_args
A_Nonnull_all_args
#endif
#ifdef A_Ret_range
A_Ret_range(0,1)
#endif
static inline int is_long_opt_matched(
#ifdef A_In
	A_In
#endif
	char *const *arg
) {
	/* arg points to yet unparsed argument or to end of arguments array */
	int has_value = ('-' != arg[-1][0]);
	char name = arg[-1 - has_value][1];
	return ('-' == name || 
}

#endif /* GET_OPT_INL_INCLUDED */
