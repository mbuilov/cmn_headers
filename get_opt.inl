#ifndef GET_OPT_INL_INCLUDED
#define GET_OPT_INL_INCLUDED

/**********************************************************************************
* Program options parsing
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt.inl */

/* structure that describes state of options parsing */
struct opt_info {

	/* current argument,
	   initially may be == &argv[1] (argv[0] is a program name, so skip it) */
	char **arg;

	/* end of program arguments array,
	   initially may be == &argv[argc] */
	char **args_end;

	/* if not NULL, then short options string in format: two symbols for one option (so string length must be even),
	  first symbol:
	    - option name, usually a letter or decimal digit, but not colon ':'
	  second symbol:
	    - ':' colon denotes that option may be specified with a value,
	    - ' ' space denotes that not expecting a value for an option (but value may be set, if specified, like for -ovalue),
	  special option name: dash '-' denotes option without name (also usually without a value, used to specify stdin/stdout),
	  spaces may be used to adjust returned option number
	  for example: "a:  b:c:  d:e f g - h " */
	const char *opts;

	/* if not NULL, then NULL-terminated array of strings - names of long options,
	  names must not contain '=' character, except at the beginning:
	  if a name starts with '=', this means that an option may be specified with a value, character '=' is ignored,
	  use empty string to adjust returned option number,
	  for example: {"","","=file","=level","","=debug","=output","verbose","","trace",NULL} */
	const char *const *long_opts;

	/* out: option value or parameter,
	  as option value may be NULL - if no value provided for an option,
	  short option cannot have empty value, but the long option may,
	  parameter value also may be empty */
	char *value;
};

/* get_opt() normally returns value >= 0 - parsed option number, but also may return next values: */
#define OPT_UNKNOWN     -1 /* i->arg points to an unknown option */
#define OPT_PARAMETER   -2 /* i->value points to parameter value */
#define OPT_REST_PARAMS -3 /* all arguments starting with i->arg and until i->args_end - are parameters */

/* Notes:
  1) get_opt(i) expects that i->arg < i->args_end,
  2) a non-NULL value may be parsed for an option that do not expects one - if a value was provided:
     for short option:  -ovalue
     for long option:   -option=value
*/

#if 0
/* example */
int main(int argc, char *argv[])
{
	struct opt_info i;
	i.arg       = &argv[1];
	i.args_end  = &argv[argc];
	i.opts      = "a:  - ";
	i.long_opts = {"","=file","beta",NULL};
	while (i.arg < i.args_end) {
		switch (get_opt(&i)) {
			case 0: /* short option 'a' */
				if (i.value)
					fprintf(stderr, "'a' has value: %s\n", i.value);
				else
					fprintf(stderr, "no value provided for 'a'\n");
				break;
			case 1: /* long option "file" */
				if (i.value)
					fprintf(stderr, "'file' has value: %s\n", i.value);
				else
					fprintf(stderr, "no value provided for 'file'\n");
				break;
			case 2: { /* short option '-' or long option "beta" */
				int long_opt = ('-' == i.arg[-1 - ('-' == i.arg[-1][0])][1]);
				if (i.value)
					fprintf(stderr, "not expecting a value for option '%s': %s\n", long_opt ? "beta" : "-", i.value);
				else
					fprintf(stderr, "option '%s'\n", long_opt ? "beta" : "-");
				break;
			}
			case OPT_UNKNOWN:
				fprintf(stderr, "unknown option: '%s'\n", *i.arg);
				error();
			case OPT_PARAMETER:
				fprintf(stderr, "parameter: '%s'\n", i.value);
				break;
			case OPT_REST_PARAMS:
				fprintf(stderr, "all parameters starting with: '%s'\n", *i.arg);
				process_params(i);
				i.arg = i.args_end;
				break;
			default:
				assert(false);
		}
	}
	...
}
#endif

#ifdef A_Nonnull_all_args
A_Nonnull_all_args
#endif
#ifdef A_Ret_range
A_Ret_range(>=,OPT_REST_PARAMS)
#endif
#ifdef A_Pre_satisfies
A_Pre_satisfies(i->arg < i->args_end)
#endif
int get_opt(
#ifdef A_Inout
	A_Inout
#endif
	struct opt_info *i
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
			/* no more options after '--' */
			return OPT_REST_PARAMS; /* all arguments starting with i->arg are parameters */
		}
		a += 2; /* skip "--" */
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
							if (!v) {
								/* no value passed, like "--help" */
								if (n != beg && i->arg != i->args_end) {
									/* option wants a value */
									a = *i->arg;
									if ('-' != *a) {
										i->arg++;
										i->value = a;
									}
									else
										i->value = NULL; /* no value provided: next is an another option */
								}
								else
									i->value = NULL; /* not expecting a value for the option or end of args */
							}
							else {
								/* "--file=f": set (may be empty) option value, even if not expecting one */
								*v = '='; /* restore */
								i->value = v + 1; /* skip '=' */
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
	else if (opts) {
		/* short option, like "-h" or "-ffile" or "-" */
		const char *o = strchr(opts, a[1] ? a[1] : '-');
		if (o) {
			if (!a[1] || !a[2]) {
				/* no value passed, like "-h" or "-" */
				if (':' == o[1] && i->arg != i->args_end) {
					/* option wants a value */
					a = *i->arg;
					if ('-' != *a) {
						i->arg++;
						i->value = a;
					}
					else
						i->value = NULL; /* no value provided: next is an another option */
				}
				else
					i->value = NULL; /* not expecting a value for the option or end of args */
			}
			else {
				/* "-ffile": set (non-empty) option value, even if not expecting one */
				i->value = a + 2; /* skip "-f" */
			}
			return (int)(o - opts)/2;
		}
	}
	i->arg--;
	return OPT_UNKNOWN; /* i->arg points to unknown (may be long) option */
}

#endif /* GET_OPT_INL_INCLUDED */
