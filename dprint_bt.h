#ifndef DPRINT_BT_H_INCLUDED
#define DPRINT_BT_H_INCLUDED

/**********************************************************************************
* Debug printing/tracing helpers
* Copyright (C) 2021 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* dprint_bt.h */

/* defines macros:

  DBGPRINT_BT(bt, format, ...)
  DBGPRINTX_BT(bt, file, line, function, format, ...)
*/

#include "dprint.h" /* for DBGPRINT()/DBGPRINTX() */

#ifdef __cplusplus
extern "C" {
#endif

/* backtracing calls:

void bar(DBGTRACE_PARAM(bt))
{
	DBGPRINT_BT(bt, "Achtung!");
}

void baz(int i DBGTRACE_NEXT_PARAM(bt))
{
	DBGPRINT_BT(bt, "Achtung: %d", i);
}

void bak(DBGTRACE_FIRST_PARAM(bt) int i)
{
	DBGPRINT_BT(bt, "Achtung: %d", i);
}

void foo(void)
{
	DBGTRACE_SITE(bt_site, NULL)
	bar(DBGTRACE_POS(bt_site));
	baz(-3 DBGTRACE_NEXT_POS(bt_site));
	bak(DBGTRACE_FIRST_POS(bt_site) -8);
}

*/

struct dbgtrace {
	struct dbgtrace *prev;
	const char *func;
	const char *file;
	int line;
};

#if defined DPRINT_TO_LOG || defined DPRINT_TO_STREAM

static inline void dbgtrace_print_(const struct dbgtrace *b)
{
	for (; b; b = b->prev)
		DBGPRINT3x1(b->file, b->line, b->func, "<- called from here");
}

#define DBGPRINT_BT(d_b_, ...)                         ((void)DBGPRINT(__VA_ARGS__), dbgtrace_print_(d_b_))
#define DBGPRINTX_BT(d_b_,d_file_,d_line_,d_func_,...) (DBGPRINTX(d_file_, d_line_, d_func_, __VA_ARGS__), dbgtrace_print_(d_b_))

#define DBGTRACE_FIELD(d_b_)                           struct dbgtrace *d_b_;
#define DBGTRACE_SITE(d_name_, d_b_)                   struct dbgtrace d_name_ = {d_b_, DPRINT_FUNC, /*file:*/NULL, /*line:*/-1};

#define DBGTRACE_PARAM(d_b_)                           struct dbgtrace *const d_b_
#define DBGTRACE_FIRST_PARAM(d_b_)                     DBGTRACE_PARAM(d_b_),
#define DBGTRACE_NEXT_PARAM(d_b_)                      , DBGTRACE_PARAM(d_b_)

#define DBGTRACE_POS(d_name_)                          (d_name_.file = __FILE__, d_name_.line = __LINE__, &d_name_)
#define DBGTRACE_FIRST_POS(d_name_)                    DBGTRACE_POS(d_name_),
#define DBGTRACE_NEXT_POS(d_name_)                     , DBGTRACE_POS(d_name_)

#else /* !DPRINT_TO_LOG || !DPRINT_TO_STREAM */

#define DBGPRINT_BT(d_b_, ...)                         ((void)0)
#define DBGPRINTX_BT(d_b_,d_file_,d_line_,d_func_,...) ((void)(d_file_),(void)(d_line_),(void)(d_func_))

#define DBGTRACE_FIELD(d_b_)
#define DBGTRACE_SITE(d_name_, d_b_)

#define DBGTRACE_PARAM(d_b_)                           void
#define DBGTRACE_FIRST_PARAM(d_b_)
#define DBGTRACE_NEXT_PARAM(d_b_)

#define DBGTRACE_POS(d_name_)
#define DBGTRACE_FIRST_POS(d_name_)
#define DBGTRACE_NEXT_POS(d_name_)

#endif /* !DPRINT_TO_LOG || !DPRINT_TO_STREAM */

#ifdef __cplusplus
}
#endif

#endif /* DPRINT_BT_H_INCLUDED */
