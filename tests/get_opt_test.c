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
	static const char *const short_opts  = "a:  b:c:  d:e f g - h ";
	static const char *const long_opts[] = {"","","=file","=level","","=debug","=output","verbose","","trace",NULL};
	struct opt_info i;
	i.arg       = &argv[1];
	i.args_end  = &argv[argc];
	i.opts      = short_opts;
	i.long_opts = long_opts;
	while (i.arg < i.args_end) {
		switch (get_opt(&i)) {
			case 0:
				printf("a:%s\n", i.value ? i.value : "<null>");
				break;
			case 1:
				fprintf(stderr, "unexpected option number 1\n");
				return 1;
			case 2:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "file" : "b", i.value ? i.value : "<null>");
				break;
			case 3:
				printf("%s:%s\n", is_long_opt_matched(i.arg) ? "level" : "c", i.value ? i.value : "<null>");
				break;
			case 4:
				fprintf(stderr, "unexpected option number 4\n");
				return 1;
			case 5:
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
