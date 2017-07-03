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
	struct opt_info i;
	i.arg       = &argv[1];
	i.args_end  = &argv[argc];
	i.opts      = "a:  b:c:  d:e f g - h ";
	i.long_opts = {"","","=file","=level","","=debug","=output","verbose","","trace",NULL};
	while (i.arg < i.args_end) {
		switch (get_opt(&i)) {
			case 0:
				printf("a:%s\n", i.value ? i.value : "<null>");
				break;
			case 1:
				printf("unexpected option number 1\n");
				return 1;
			case 2:
				printf("b or file:%s\n", i.value ? i.value : "<null>");
				break;
			case 3:
				printf("c or level:%s\n", i.value ? i.value : "<null>");
				break;
			case 4:
				printf("unexpected option number 4\n");
				return 1;
			case 5:
				printf("d or debug:%s\n", i.value ? i.value : "<null>");
				break;
			case 6:
				printf("e or output:%s\n", i.value ? i.value : "<null>");
				break;
			case 7:
				printf("f or verbose:%s\n", i.value ? i.value : "<null>");
				break;
			case 8:
				printf("g:%s\n", i.value ? i.value : "<null>");
				break;
			case 9:
				printf("- or trace:%s\n", i.value ? i.value : "<null>");
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
				printf("assert!\n");
				return 1;
		}
	}
	return 0;
}
