#ifndef GET_OPT_INFO_H_INCLUDED
#define GET_OPT_INFO_H_INCLUDED

/**********************************************************************************
* Program options and parameters parsing
* Copyright (C) 2018-2020 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt_info.h */

#ifndef GET_OPT_CHAR
#ifdef GET_OPT_WIDE_CHAR_SUPPORT
#define GET_OPT_CHAR wchar_t
#else
#define GET_OPT_CHAR char
#endif
#endif

/* structure that describes state of command-line options parsing */
/* NOTE: get_opt() treats all external data referenced by this structure as read-only */
struct opt_info {

	/* in/out: next argument to check or current short options bundle (if 'sopt' != NULL),
	   initially 'arg' should be set to point to &argv[1] (argv[0] is the program name, so skip it) */
	GET_OPT_CHAR *const *arg;

	/* out: option value or parameter,
	  as option value may be NULL - if no value was specified for the option */
	GET_OPT_CHAR *value;

	/* in/out: next short option to check in the bundle pointed by 'arg',
	  used when multiple short options are bundled together,
	  e.g. in "-xyz" (which is equivalent to "-x -y -z"), 'sopt' may point
	  to "y" or "z", while 'arg' pointing to the bundle "-xyz" */
	GET_OPT_CHAR *sopt;

	/* normally 'argv' array, that is passed to 'main(int argc, char *argv[])',
	   is NULL-terminated, but if it's not - need to use a pointer to the end of array */
#ifdef GET_OPT_ARGV_NZ
	/* in: end of program arguments array,
	   initially should be set to point to &argv[argc] */
	GET_OPT_CHAR *const *args_end;
#endif
};

#endif /* GET_OPT_INFO_H_INCLUDED */
