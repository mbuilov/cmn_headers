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

#endif /* GET_OPT_INFO_H_INCLUDED */
