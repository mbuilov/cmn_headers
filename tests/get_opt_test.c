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
	static const char short_opts[] =
#define SHORT_OPT_A SHORT_OPT(0)
		"aa"
#define SHORT_OPT_F SHORT_OPT(2)
		"ff"
#define SHORT_OPT_L SHORT_OPT(4)
		"ll"
#define SHORT_OPT_D SHORT_OPT(6)
		"dd"
#define SHORT_OPT_O SHORT_OPT(8)
		"o"
#define SHORT_OPT_V SHORT_OPT(9)
		"v"
#define SHORT_OPT_G SHORT_OPT(10)
		"g"
		"t-"
	;
	static const char *const long_opts[]= {
#define LONG_OPT_FILE    LONG_OPT(0)
		"=file",
#define LONG_OPT_LEVEL   LONG_OPT(1)
		"=level",
#define LONG_OPT_DEBUG   LONG_OPT(2)
		"=debug",
#define LONG_OPT_OUTPUT  LONG_OPT(3)
		"=output",
#define LONG_OPT_VERBOSE LONG_OPT(4)
		"verbose",
#define LONG_OPT_TRACE   LONG_OPT(5)
		"trace",
		NULL
	};
	struct opt_info i;
	opt_info_init(&i, argc, argv);
	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case SHORT_OPT_A:
				printf("a:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_F:
				printf("f:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_L:
				printf("l:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_D:
				printf("d:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_O:
				printf("o:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_V:
				printf("v:%s\n", i.value ? i.value : "<null>");
				break;
			case SHORT_OPT_G:
				printf("g:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_FILE:
				printf("file:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_LEVEL:
				printf("level:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_DEBUG:
				printf("debug:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_OUTPUT:
				printf("output:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_VERBOSE:
				printf("verbose:%s\n", i.value ? i.value : "<null>");
				break;
			case LONG_OPT_TRACE:
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
