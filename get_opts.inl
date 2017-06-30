#ifndef GET_OPTS_INL_INCLUDED
#define GET_OPTS_INL_INCLUDED

/**********************************************************************************
* Simple program options parsing
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opts.inl */

/*
  int get_opts(int argc, char *argv[], const char *opts, const char *const long_opts[], char *values[]);

  for a program executed as: "program -aavalue -b bval -c -e -fg - --file=f --level 1 --debug --verbose -- ..."

  fill options values array: [
    "avalue", // a        - option with (optional) value (value is specified - "avalue")
    "bval",   // b        - option with (optional) value (value is specified - "bval")
    "",       // c        - option with (optional) value (value is not specified, so value is an empty string "")
    def,      // d        - option with (optional) value (option is not specified, so value is default - def)
    "",       // e        - option without value (option is specified, so value is an empty string "")
    "",       // f        - option without value (option is specified, so value is an empty string "")
    "",       // g        - option without value (option is specified, so value is an empty string "")
    def,      // h        - option without value (option is not specified, so value is default - def)
    "f",      // file     - long option with (optional) value (value is specified - "f")
    "1",      // level    - long option with (optional) value (option is specified - "1")
    "",       // debug    - long option with (optional) value (value is not specified, so value is an empty string "")
    def,      // output   - long option with (optional) value (option is not specified, so value is default - def)
    "",       // verbose  - long option without value (option is specified, so value is an empty string "")
    def,      // trace    - long option without value (option is not specified, so value is default - def)
    "",       // -        - option without value (option is specified, so value is an empty string "")
  ]

  ("--" denotes end of options)

  Notes:
   - array of values must be initialized with default values (likely NULL's) prior the call
   - options may be specified in command line in any order, also after non-option arguments
   - if an option is not specified, corresponding value in array of values will not be changed
   - an option may be specified many times - the last value takes precedence
   - may set values for options that doesn't expect them

  argc      - number of program arguments, as passed to main(int argc, char *argv[])
  argv      - program arguments array, as passed to main(int argc, char *argv[])
  opts      - short options string in format: two symbols for one option, so string length is even,
              first symbol:
                - option name, usually a letter or decimal digit, but not colon ':'
              second symbol:
                - ':' colon denotes that option may be specified with a value,
                - ' ' space denotes that not expecting a value for an option (but value may be set, like for -ovalue),
              special option name: dash '-' denotes option without name and value (usually used to specify stdin/stdout),
              spaces may be used to adjust position of option value in values array
              for example: "a:b:c:d:e f g h             - "
  long_opts - NULL-terminated array of strings - names of long options,
              names must not contain '=' character, except at the beginning:
              if a name starts with '=', this means that the option may be specified with a value, character '=' is ignored,
              use empty string to specify that no long option exists for given position in values array,
              for example: {"","","","","","","","","=file","=level","=debug","=output","verbose","trace","",NULL}
  values    - to be filled array of pointers to values of the options, must be filled with default values (usually NULLs) prior the call.

  returns:
    0 - all options are parsed, no non-option arguments
   >0 - number of first non-option program argument (not started with '-')
   <0 - number of first unknown option argument

example:

int main(int argc, char *argv[])
{
	#define A values[0]
	#define B values[1]
	#define C values[2]
	const char *const long_opts[] = {"","","","","","","","","=file","=level","=debug","=output","verbose","trace","",NULL};
	char *values[sizeof(long_opts)/sizeof(long_opts[0]) - 1] = {NULL};
	int i = get_opts(argc, argv, "a:b:c:d:e f g h             - ", long_opts, values);
	if (i > 0) {
		fprintf(stderr, "%s: non-option argument: %s\n", argv[0], argv[i]);
		return 1;
	}
	if (i < 0) {
		fprintf(stderr, "%s: unknown option: %s\n", argv[0], argv[-i]);
		return 2;
	}
	...
}
*/

#ifdef A_Ret_range
A_Ret_range(-(argc-1), argc-1)
#endif
static int get_opts(
#ifdef A_In_range
	A_In_range(>,0)
#endif
	int argc,
#ifdef A_In_reads
	A_In_reads(argc)
#endif
	char *argv[/*argc*/],
#ifdef A_In_opt_z
	A_In_opt_z
#endif
	const char *opts/*NULL?,'\0'-terminated*/,
#ifdef A_In_opt
	A_In_opt
#endif
	const char *const long_opts[]/*NULL?,NULL-terminated*/,
#ifdef A_Out
	A_Inout
#endif
	char *values[]/*output,initialized with default values*/)
{
	int non_opt = 0;     /* number of first non-option argument, by default 0 - assume there are no non-option arguments */
	char **value = NULL; /* if non-NULL, then take next non-option argument as option value */
	int i = 1;           /* skip program name */
	for (; i < argc; i++) {
		char *a = argv[i];
		if ('-' != a[0]) {
			/* non-option argument */
			if (value) {
				*value = a; /* set option value */
				value = NULL;
			}
			else if (!non_opt)
				non_opt = i; /* number of first non-option argument */
		}
		else if ('-' == a[1]) {
			/* long option, like "--help" */
			if (!a[2])
				return (++i == argc || non_opt) ? non_opt : i; /* no more options after '--' */
			a += 2; /* skip "--" */
			if (long_opts) {
				char *v = strchr(a, '=');
				if (v)
					*v = '\0';
				{
					const char *const *lo = long_opts;
					for (; *lo; lo++) {
						const char *beg = *lo;
						if (*beg) {
							const char *o = beg;
							if ('=' == *o)
								o++; /* expecting an option value */
							if (!strcmp(a, o)) {
								value = &values[lo - long_opts];
								if (!v) {
									*value = a + strlen(a);
									if (o == beg)
										value = NULL; /* not expecting a value for the option */
								}
								else {
									/* set option value, even if not expecting one */
									*v = '='; /* restore */
									*value = v + 1; /* skip '=' */
									value = NULL;
								}
								goto next_args;
							}
						}
					}
				}
				if (v)
					*v = '='; /* restore */
			}
			return -i; /* argument number i is an unknown long option */
next_args:;
		}
		else if (opts) {
			/* short option, like "-h" */
			const char *o = strchr(opts, a[1] ? a[1] : '-');
			if (!o)
				return -i; /* argument number i is an unknown option */
			/* set option value, even if not expecting one */
			value = &values[(o - opts)/2];
			*value = a + 2;
			/* if value is non-empty or not expecting one for an option - reset value pointer */
			if (a[2] || ':' != o[1])
				value = NULL;
		}
		else
			return -i; /* argument number i is an unknown option */
	}
	return non_opt; /* ok */
}

#endif /* GET_OPTS_INL_INCLUDED */
