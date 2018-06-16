#ifndef GET_OPT_INFO_H_INCLUDED
#define GET_OPT_INFO_H_INCLUDED

/**********************************************************************************
* Program options and parameters parsing
* Copyright (C) 2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt_info.h */

#ifndef GET_OPT_CHAR
#define GET_OPT_CHAR char
#endif

/* structure that describes state of options parsing */
struct opt_info {

	/* in/out: next argument to check,
	   but if 'sopt' != NULL, then 'arg' points to the whole short options bundle argument,
	   initially 'arg' should be set to &argv[1] (argv[0] is a program name, so skip it) */
	GET_OPT_CHAR *const *arg;

	/* normally, 'argv' array, that is passed to 'main(int argc, char *argv[])',
	   is NULL-terminated, but if it's not - need to use a pointer to the end array */
#ifdef GET_OPT_ARGV_NZ
	/* in: end of program arguments array,
	   initially should be set to &argv[argc] */
	GET_OPT_CHAR *const *args_end;
#endif

	/* out: option value or parameter,
	  as option value may be NULL - if no value was specified for the option */
	GET_OPT_CHAR *value;

	/* in/out: next short option in a bundle pointed by 'arg',
	  used when multiple short options are bundled together,
      e.g.: "-xyz-", which is equivalent of "-x -y -z -" */
	GET_OPT_CHAR *sopt;
};

#endif /* GET_OPT_INFO_H_INCLUDED */
