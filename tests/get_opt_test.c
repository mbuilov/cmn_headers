/**********************************************************************************
* Program options parsing test
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
	static const char        short_opts[] =  "a\t""  ""f\t"   "l\t"    "  ""d\t"    "o "      "v "      "g ""  ""t-";
	static const char *const long_opts[]  = {"",  "", "=file","=level","", "=debug","=output","verbose","", "trace",NULL};
	struct opt_info i;
	opt_info_init(&i, argc, argv);
	while (i.arg < i.args_end) {
		switch (get_opt(&i, short_opts, long_opts)) {
			case 0:
				printf("a:%s\n", i.value ? i.value : "<null>");
				break;
			case 1:
			case 2:
			case 3:
				fprintf(stderr, "unexpected option number 1,2 or 3\n");
				return 1;
			case 4:
				printf("f:%s\n", i.value ? i.value : "<null>");
				break;
			case 5:
				printf("file:%s\n", i.value ? i.value : "<null>");
				break;
			case 6:
				printf("l:%s\n", i.value ? i.value : "<null>");
				break;
			case 7:
				printf("level:%s\n", i.value ? i.value : "<null>");
				break;
			case 8:
			case 9:
				fprintf(stderr, "unexpected option number 8,9\n");
				return 1;
			case 10:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "debug" : "d", i.value ? i.value : "<null>");
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "debug" : "d", i.value ? i.value : "<null>");
				break;
			case 6:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "output" : "e", i.value ? i.value : "<null>");
				break;
			case 7:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "verbose" : "f", i.value ? i.value : "<null>");
				break;
			case 8:
				printf("g:%s\n", i.value ? i.value : "<null>");
				break;
			case 9:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "trace" : "-", i.value ? i.value : "<null>");
				break;
			case 10:
				printf("h:%s\n", i.value ? i.value : "<null>");
				break;
			case OPT_UNKNOWN:
				printf("unknown option: %s\n", *i.arg++);
				break;
			case OPT_PARAMETER:
				printf("parameter: %s\n", i.value);
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
