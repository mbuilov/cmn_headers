#ifndef GET_OPTS_INL_INCLUDED
#define GET_OPTS_INL_INCLUDED

/**********************************************************************************
* Simple program options parsing
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opts.inl */

/*
  for a program executed as: "program -aavalue -bbval -c ..."
  fill options values array: ["avalue", "bval", ""]

  NOTE: options may be specified in any order
  NOTE: options may be optional, corresponding value in values array will be NULL
  NOTE: an option may be specified many times - only the last value is used

  argc   - number of program arguments, as passed to main(int argc, char *argv[])
  argv   - program arguments array, as passed to main(int argc, char *argv[])
  opts   - options string in format: one letter for one option, for example: "abc"
  values - to be filled array of pointers to values of the options, must be big enough

  returns:
    0 - ok
   >0 - number of first non-option program argument (not started with '-')
   <0 - number of first unknown option argument

example:

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
*/

static int get_opts(
#ifdef A_In_range
	A_In_range(>,0)
#endif
	int argc,
#ifdef A_In_reads
	A_In_reads(argc)
#endif
	char *argv[/*argc*/],
#ifdef A_In_z
	A_In_z
#endif
	const char *opts/*'\0'-terminated*/,
#ifdef A_Out
	A_Out
#endif
	char *values[/*strlen(opts)*/])
{
	/* fill values array with NULLs */
	{
		const char *o = opts;
		for (; *o; o++)
			values[o - opts] = NULL;
	}
	/* parse options */
	{
		int i = 1; /* skip program name */
		for (; i < argc; i++) {
			if ('-' != argv[i][0])
				return i; /* expecting an option started with '-' at argument number i */
			{
				const char *o = opts;
				for (; *o; o++) {
					if (argv[i][1] == *o)
						break; /* ok, option found */
				}
				if (!*o)
					return -i; /* unknown option at argument number i */
				values[o - opts] = &argv[i][2]; /* NOTE: value may be empty */
			}
		}
	}
	return 0; /* ok */
}

#endif /* GET_OPTS_INL_INCLUDED */
