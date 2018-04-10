/**********************************************************************************
* Program options and parameters parsing test
* Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* get_opt_test.c */

#include <stdio.h>
#include <string.h>
#include "../sal_defs.h"
#include "../get_opt.inl"

int main(int argc, char *argv[])
{
#define SHORT_OPTION_a      SHORT_OPT_MODIFIER("aa", SHORT_OPTION_f)     /* -avalue */
#define SHORT_OPTION_f      SHORT_OPT_MODIFIER("ff", SHORT_OPTION_l)     /* -fvalue */
#define SHORT_OPTION_l      SHORT_OPT_MODIFIER("ll", SHORT_OPTION_d)     /* -lvalue */
#define SHORT_OPTION_d      SHORT_OPT_MODIFIER("dd", SHORT_OPTION_q)     /* -dvalue */
#if 0
#define SHORT_OPTION_q      SHORT_OPT_MODIFIER("q",  SHORT_OPTION_o)
#else
#define SHORT_OPTION_q      SHORT_OPTION_o
#endif
#define SHORT_OPTION_o      SHORT_OPT_MODIFIER("o",  SHORT_OPTION_v)      /* -o */
#define SHORT_OPTION_v      SHORT_OPT_MODIFIER("v",  SHORT_OPTION_g)      /* -v */
#define SHORT_OPTION_g      SHORT_OPT_MODIFIER("g",  DASH_SHORT_OPTION_t) /* -g */
#define DASH_SHORT_OPTION_t SHORT_OPT_MODIFIER("t-", SHORT_OPT_NULL)      /* -trace (equivalent to --trace) */

#define SHORT_OPT_NULL      ""
#define SHORT_OPT_MODIFIER  SHORT_OPT_DEFINER

	static const char short_opts[] = SHORT_OPTION_a;

#define LONG_OPTION_file    LONG_OPT_MODIFIER("=file",   LONG_OPTION_level)  /* --file=value  */
#define LONG_OPTION_level   LONG_OPT_MODIFIER("=level",  LONG_OPTION_debug)  /* --level=value */
#define LONG_OPTION_debug   LONG_OPT_MODIFIER("=debug",  LONG_OPTION_quiet)  /* --debug=value */
#if 0
#define LONG_OPTION_quiet   LONG_OPT_MODIFIER("quiet",   LONG_OPTION_output)
#else
#define LONG_OPTION_quiet   LONG_OPTION_output
#endif
#define LONG_OPTION_output  LONG_OPT_MODIFIER("=output", LONG_OPTION_verbose) /* --output=value */
#define LONG_OPTION_verbose LONG_OPT_MODIFIER("verbose", LONG_OPTION_trace)   /* --verbose      */
#define LONG_OPTION_trace   LONG_OPT_MODIFIER("trace",   LONG_OPT_NULL)       /* --trace        */

#define LONG_OPT_NULL       NULL
#define LONG_OPT_MODIFIER   LONG_OPT_DEFINER

	static const char *const long_opts[] = {LONG_OPTION_file};

#undef  SHORT_OPT_NULL
#undef  SHORT_OPT_MODIFIER

#define SHORT_OPT_NULL      SHORT_OPT_END_POS(short_opts)
#define SHORT_OPT_MODIFIER  SHORT_OPT_ENCODER

#undef  LONG_OPT_NULL
#undef  LONG_OPT_MODIFIER

#define LONG_OPT_NULL       LONG_OPT_END_IDX(long_opts)
#define LONG_OPT_MODIFIER   LONG_OPT_ENCODER

	struct opt_info i;
	opt_info_init(&i, argc, argv);

	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPTION_a:
				printf("a:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_f:
				printf("f:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_l:
				printf("l:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_d:
				printf("d:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_o:
				printf("o:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_v:
				printf("v:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPTION_g:
				printf("g:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_file:
				printf("file:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_level:
				printf("level:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_debug:
				printf("debug:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_output:
				printf("output:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_verbose:
				printf("verbose:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPTION_trace:
				printf("trace:%s\n", i.value ? i.value : "<null>");
				break;
			case OPT_UNKNOWN:
				if (i.sopt)
					printf("unknown short option '%c' in the bundle: '%s'\n", *i.sopt, *i.arg);
				else
					printf("unknown option: '%s'\n", *i.arg);
				i.arg++; /* skip unknown option */
				i.sopt = NULL;
				break;
			case OPT_BAD_BUNDLE:
				printf("short option '%c' cannot be bundled: '%s'\n", *i.sopt, *i.arg);
				i.arg++; /* skip bad option */
				i.sopt = NULL;
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
