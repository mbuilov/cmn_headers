#ifndef UTF8_TO_UTF16_INL_INCLUDED
#define UTF8_TO_UTF16_INL_INCLUDED

/**********************************************************************************
* UTF-8 to UTF-16 characters conversion
* Copyright (C) 2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* utf8_to_utf16.inl */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4505) /* unreferenced local function has been removed */
#endif

/* type of utf16 character,
  must be unsigned integer of at least 16 bits */
#ifndef UTF16CHAR
#ifdef _WIN32
#define UTF16CHAR wchar_t
#else
#define UTF16CHAR unsigned short
#endif
#endif
typedef int (*utf8_to_utf16_bad_UTF16CHAR)[1-2*((UTF16CHAR)-1 < 0 || (UTF16CHAR)-1 < 0xFFFFu)];

/* type of utf8 character,
  must be unsigned integer of at least 8 bits */
#ifndef UTF8CHAR
#define UTF8CHAR unsigned char
#endif
typedef int (*utf8_to_utf16_bad_UTF8CHAR)[1-2*((UTF8CHAR)-1 < 0 || (UTF8CHAR)-1 < 0xFFu)];

/* convert utf8 0-terminated string to utf16 0-terminated one,
 input:
  q  - address of the pointer to the beginning of input 0-terminated utf8 string,
  b  - optional address of the pointer to the beginning of output buffer,
  sz - free space in output buffer, in UTF16CHARs, if zero - output buffer is not used.
 returns number of stored UTF16CHARs, including terminating 0:
  0     - if utf8 string is invalid,
  <= sz - 0-terminated utf16 string was successfully stored in output buffer,
  > sz  - output buffer is too small, return value is the required buffer size to store
   whole converted utf16 0-terminated string, including the part that is already converted
   and stored in output buffer, including 0-terminator, in UTF16CHARs;
 - on success:
  (*q) - points beyond the 0-terminator of input utf8 string,
  (*b) - points beyond the 0-terminator stored in output buffer;
 - if output buffer is too small:
  (*q) - if sz == 0, not changed, else - points beyond the last converted (non-0) UTF8CHAR,
  (*b) - if sz == 0, not changed, else - points beyond the last stored (non-0) UTF16CHAR;
 - if input utf8 string is invalid:
  (*q) - points beyond the last valid UTF8CHAR, if output buffer is too small,
   it may be different from the last successfully converted UTF8CHAR,
   the last valid UTF8CHAR is _not_ 0;
  (*b) - if sz > 0, points beyond the last stored (non-0) UTF16CHAR */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(q, A_Inout) A_At(*q, A_In_z)
A_When(sz, A_At(b, A_Notnull) A_At(*b, A_Writable_elements(sz)))
#endif
static size_t utf8_to_utf16_z(const UTF8CHAR **const q, UTF16CHAR **const b, size_t sz)
{
	/* unsigned integer type must be at least of 32 bits */
	size_t m = 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	const UTF8CHAR *s = *q;
	const UTF8CHAR *t; /* points beyond the last converted UTF8CHAR */
	if (!sz)
		t = s;
	else {
		UTF16CHAR *d = *b;
		UTF16CHAR *const e = d + sz;
		do {
			unsigned a = s[0];
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						if (a >= 0xF8)
							goto bad_utf8; /* expecting max 4 utf8 bytes for a unicode code point */
						r = s[1];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[2];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[3];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						if (0x3600000 != (a & 0x3F00000))
							goto bad_utf8; /* bad utf8 character */
						s += 4;
						if ((size_t)(e - d) < 2) {
							t = s - 4;
							m = 2; /* = (4 UTF8CHARs - 2 UTF16CHARs) */
							goto small_buf; /* too small output buffer */
						}
						*d++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
						a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
					}
					else {
						r = s[1];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[2];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0xE2080;
						/* must not begin or end a surrogate pair */
						if (!a ||
						    0xD800 == (a & 0xFC00) ||
						    0xDC00 == (a & 0xFC00))
						{
							goto bad_utf8; /* overlong/bad utf8 character */
						}
						s += 3;
					}
				}
				else if (a >= 0xC0) {
					r = s[1];
					if (0x80 != (r & 0xC0))
						goto bad_utf8; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0x3080;
					if (!a)
						goto bad_utf8; /* overlong utf8 character */
					s += 2;
				}
				else
					goto bad_utf8; /* not expecting 10xxxxxx */
			}
			else
				s++;
			*d++ = (UTF16CHAR)a;
			if (!a) {
				m = (size_t)(d - *b);
bad_utf8:
				*q = s; /* (*q) != 0 if bad_utf8, else (*q) points beyond successfully converted 0 */
				*b = d;
				return m; /* 0 if bad_utf8, else >0 */
			}
		} while (d != e);
		/* too small output buffer */
		t = s;
small_buf:
		sz = (size_t)(d - *b);
		*b = d;
	}
	for (;;) {
		unsigned a = s[0];
		if (a >= 0x80) {
			unsigned r;
			if (a >= 0xE0) {
				if (a >= 0xF0) {
					if (a >= 0xF8)
						goto bad_utf8_s; /* expecting max 4 utf8 bytes for a unicode code point */
					r = s[1];
					if (0x80 != (r & 0xC0))
						goto bad_utf8_s; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = s[2];
					if (0x80 != (r & 0xC0))
						goto bad_utf8_s; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = s[3];
					if (0x80 != (r & 0xC0))
						goto bad_utf8_s; /* incomplete utf8 character */
					a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
					/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
					 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
					if (0x3600000 != (a & 0x3F00000))
						goto bad_utf8_s; /* bad utf8 character */
					s += 4;
					m += 2; /* + (4 UTF8CHARs - 2 UTF16CHARs) */
				}
				else {
					r = s[1];
					if (0x80 != (r & 0xC0))
						goto bad_utf8_s; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = s[2];
					if (0x80 != (r & 0xC0))
						 goto bad_utf8_s; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0xE2080;
					/* must not begin or end a surrogate pair */
					if (!a ||
						0xD800 == (a & 0xFC00) ||
						0xDC00 == (a & 0xFC00))
					{
						goto bad_utf8_s; /* overlong/bad utf8 character */
					}
					s += 3;
					m += 2; /* + (3 UTF8CHARs - 1 UTF16CHAR) */
				}
			}
			else if (a >= 0xC0) {
				r = s[1];
				if (0x80 != (r & 0xC0))
					goto bad_utf8_s; /* incomplete utf8 character */
				a = (a << 6) ^ r ^ 0x3080;
				if (!a)
					goto bad_utf8_s; /* overlong utf8 character */
				s += 2;
				m++; /* + (2 UTF8CHARs - 1 UTF16CHAR) */
			}
			else {
				/* not expecting 10xxxxxx */
bad_utf8_s:
				*q = s; /* (*q) != 0 */
				return 0; /* incomplete utf8 character */
			}
		}
		else {
			s++;
			if (!a) {
				/* no overflow of size_t is possible when counting the length of
				  resulting utf16 string (in UTF16CHARs), assuming that the
				  length of utf8 string may be stored in a variable of size_t without loss:
				 4 UTF8CHARs -> 2 UTF16CHAR (surrogate pair),
				 3 UTF8CHARs -> 1 UTF16CHAR,
				 2 UTF8CHARs -> 1 UTF16CHAR,
				 1 UTF8CHAR  -> 1 UTF16CHAR*/
				/* append a number of UTF16CHARs in utf8 string started from 't' */
				sz += (size_t)(s - t) - m;
				*q = t; /* points after the last successfully converted non-0 UTF8CHAR */
				return sz; /* ok, >0 */
			}
		}
	}
}

/* convert 'n' UTF8CHARs to utf16 ones,
 input:
  q  - address of the pointer to the beginning of input utf8 string,
  b  - optional address of the pointer to the beginning of output buffer,
  sz - free space in output buffer, in UTF16CHARs, if zero - output buffer is not used.
  n  - number of UTF8CHARs to convert, if zero - input and output buffers are not used.
 returns number of stored UTF16CHARs:
  0     - if 'n' is zero or an invalid utf8 character is encountered,
  <= sz - all 'n' UTF8CHARs were successfully converted to utf16 ones and stored in the output buffer,
  > sz  - output buffer is too small, return value is the required buffer size to store whole converted
   utf16 string, including the part that is already converted and stored in the output buffer, in UTF16CHARs;
 - on success:
  (*q) - points beyond the last source UTF8CHAR of input string,
  (*b) - points beyond the last converted UTF16CHAR stored in output buffer;
 - if output buffer is too small:
  (*q) - if sz == 0, not changed, else - points beyond the last converted UTF8CHAR,
  (*b) - if sz == 0, not changed, else - points beyond the last stored UTF16CHAR;
 - if input utf8 string is invalid:
  (*q) - points beyond the last valid UTF8CHAR, if output buffer is too small,
   it may be different from the last successfully converted UTF8CHAR,
   the last valid UTF8CHAR is _not_ the last character of utf8 string;
  (*b) - if sz > 0, points beyond the last successfully converted and stored UTF16CHAR */
/* Note: zero UTF8CHAR is not treated specially, i.e. conversion do not stops */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_When(!n, A_Ret_range(==,0))
A_Success(return)
A_When(n, A_At(q, A_Inout) A_At(*q, A_In_reads(n)))
A_When(n && sz, A_At(b, A_Notnull) A_At(*b, A_Writable_elements(sz)))
#endif
static size_t utf8_to_utf16(const UTF8CHAR **const q, UTF16CHAR **const b, size_t sz, const size_t n)
{
	if (n) {
		/* unsigned integer type must be at least of 32 bits */
		size_t m = 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
		const UTF8CHAR *s = *q;
		const UTF8CHAR *const se = s + n;
		const UTF8CHAR *t; /* points beyond the last converted UTF8CHAR */
		if (!sz)
			t = s;
		else {
			UTF16CHAR *d = *b;
			UTF16CHAR *const e = d + sz;
			do {
				unsigned a = s[0];
				if (a >= 0x80) {
					unsigned r;
					if (a >= 0xE0) {
						if (a >= 0xF0) {
							if (a >= 0xF8)
								goto bad_utf8; /* expecting max 4 utf8 bytes for a unicode code point */
							if ((size_t)(se - s) < 4)
								goto bad_utf8; /* incomplete utf8 character */
							r = s[1];
							if (0x80 != (r & 0xC0))
								goto bad_utf8; /* incomplete utf8 character */
							a = (a << 6) ^ r;
							r = s[2];
							if (0x80 != (r & 0xC0))
								goto bad_utf8; /* incomplete utf8 character */
							a = (a << 6) ^ r;
							r = s[3];
							if (0x80 != (r & 0xC0))
								goto bad_utf8; /* incomplete utf8 character */
							a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
							/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
							 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
							if (0x3600000 != (a & 0x3F00000))
								goto bad_utf8; /* bad utf8 character */
							s += 4;
							if ((size_t)(e - d) < 2) {
								t = s - 4;
								m = 2; /* = (4 UTF8CHARs - 2 UTF16CHARs) */
								goto small_buf; /* too small output buffer */
							}
							*d++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
							a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
						}
						else {
							if ((size_t)(se - s) < 3)
								goto bad_utf8; /* incomplete utf8 character */
							r = s[1];
							if (0x80 != (r & 0xC0))
								goto bad_utf8; /* incomplete utf8 character */
							a = (a << 6) ^ r;
							r = s[2];
							if (0x80 != (r & 0xC0))
								goto bad_utf8; /* incomplete utf8 character */
							a = (a << 6) ^ r ^ 0xE2080;
							/* must not begin or end a surrogate pair */
							if (!a ||
								0xD800 == (a & 0xFC00) ||
								0xDC00 == (a & 0xFC00))
							{
								goto bad_utf8; /* overlong/bad utf8 character */
							}
							s += 3;
						}
					}
					else if (a >= 0xC0) {
						if ((size_t)(se - s) < 2)
							goto bad_utf8; /* incomplete utf8 character */
						r = s[1];
						if (0x80 != (r & 0xC0))
							goto bad_utf8; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0x3080;
						if (!a)
							goto bad_utf8; /* overlong utf8 character */
						s += 2;
					}
					else
						goto bad_utf8; /* not expecting 10xxxxxx */
				}
				else
					s++;
				*d++ = (UTF16CHAR)a;
				if (s == se) {
					m = (size_t)(d - *b);
bad_utf8:
					*q = s; /* (*q) < se if bad_utf8, else (*q) == se */
					*b = d;
					return m; /* 0 if bad_utf8, else >0 */
				}
			} while (d != e);
			/* too small output buffer */
			t = s;
small_buf:
			sz = (size_t)(d - *b);
			*b = d;
		}
		do {
			unsigned a = s[0];
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						if (a >= 0xF8)
							goto bad_utf8_s; /* expecting max 4 utf8 bytes for a unicode code point */
						if ((size_t)(se - s) < 4)
							goto bad_utf8_s; /* incomplete utf8 character */
						r = s[1];
						if (0x80 != (r & 0xC0))
							goto bad_utf8_s; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[2];
						if (0x80 != (r & 0xC0))
							goto bad_utf8_s; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[3];
						if (0x80 != (r & 0xC0))
							goto bad_utf8_s; /* incomplete utf8 character */
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						if (0x3600000 != (a & 0x3F00000))
							goto bad_utf8_s; /* bad utf8 character */
						s += 4;
						m += 2; /* + (4 UTF8CHARs - 2 UTF16CHARs) */
					}
					else {
						if ((size_t)(se - s) < 3)
							goto bad_utf8_s; /* incomplete utf8 character */
						r = s[1];
						if (0x80 != (r & 0xC0))
							goto bad_utf8_s; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = s[2];
						if (0x80 != (r & 0xC0))
							 goto bad_utf8_s; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0xE2080;
						/* must not begin or end a surrogate pair */
						if (!a ||
							0xD800 == (a & 0xFC00) ||
							0xDC00 == (a & 0xFC00))
						{
							goto bad_utf8_s; /* overlong/bad utf8 character */
						}
						s += 3;
						m += 2; /* + (3 UTF8CHARs - 1 UTF16CHAR) */
					}
				}
				else if (a >= 0xC0) {
					if ((size_t)(se - s) < 2)
						goto bad_utf8_s; /* incomplete utf8 character */
					r = s[1];
					if (0x80 != (r & 0xC0))
						goto bad_utf8_s; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0x3080;
					if (!a)
						goto bad_utf8_s; /* overlong utf8 character */
					s += 2;
					m++; /* + (2 UTF8CHARs - 1 UTF16CHAR) */
				}
				else {
					/* not expecting 10xxxxxx */
bad_utf8_s:
					*q = s; /* (*q) < se */
					return 0; /* incomplete utf8 character */
				}
			}
			else
				s++;
		} while (s != se);
		/* no overflow of size_t is possible when counting the length of
		  resulting utf16 string (in UTF16CHARs), assuming that the
		  length of utf8 string may be stored in a variable of size_t without loss:
		 4 UTF8CHARs -> 2 UTF16CHAR (surrogate pair),
		 3 UTF8CHARs -> 1 UTF16CHAR,
		 2 UTF8CHARs -> 1 UTF16CHAR,
		 1 UTF8CHAR  -> 1 UTF16CHAR*/
		/* append a number of UTF16CHARs in utf8 string started from 't' */
		sz += (size_t)(s - t) - m;
		*q = t; /* points after the last successfully converted UTF8CHAR, (*q) < se */
		return sz; /* ok, >0 */
	}
	return 0; /* n is zero */
}

/* convert utf8 0-terminated string to utf16 0-terminated one,
  do not check if there is enough space in output buffer, assume it is large enough.
 returns number of stored UTF16CHARs, including terminating 0:
  0    - if utf8 string is invalid,
  else - stored utf16 string length, including 0-terminator, in UTF16CHARs */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(q, A_In_z)
A_At(buf, A_Post_z A_Post_readable_size(return))
#endif
static size_t utf8_to_utf16_z_unsafe_out(const UTF8CHAR *q, UTF16CHAR *const buf)
{
	/* unsigned integer type must be at least of 32 bits */
	UTF16CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	for (;;) {
		unsigned a = q[0];
		if (a >= 0x80) {
			unsigned r;
			if (a >= 0xE0) {
				if (a >= 0xF0) {
					if (a >= 0xF8)
						return 0; /* expecting max 4 utf8 bytes for a unicode code point */
					r = q[1];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = q[2];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = q[3];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
					/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
					 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
					if (0x3600000 != (a & 0x3F00000))
						return 0; /* bad utf8 character */
					*b++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
					a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
					q += 4;
				}
				else {
					r = q[1];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = q[2];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0xE2080;
					/* must not begin or end a surrogate pair */
					if (!a ||
						0xD800 == (a & 0xFC00) ||
						0xDC00 == (a & 0xFC00))
					{
						return 0; /* overlong/bad utf8 character */
					}
					q += 3;
				}
			}
			else if (a >= 0xC0) {
				r = q[1];
				if (0x80 != (r & 0xC0))
					return 0; /* incomplete utf8 character */
				a = (a << 6) ^ r ^ 0x3080;
				if (!a)
					return 0; /* overlong utf8 character */
				q += 2;
			}
			else
				return 0; /* not expecting 10xxxxxx */
		}
		else
			q++;
		/* make analyzer happy: this equivalent to:
		   *b++ = (UTF16CHAR)a; */
		buf[b++ - buf] = (UTF16CHAR)a;
		if (!a)
			return (size_t)(b - buf); /* ok, >0 */
	}
}

/* same as utf8_to_utf16_z_unsafe_out(), but also do not check that input utf8 string is valid */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_Ret_range(>,0)
A_At(q, A_In_z)
A_At(buf, A_Post_z A_Post_readable_size(return))
#endif
static size_t utf8_to_utf16_z_unsafe(const UTF8CHAR *q, UTF16CHAR *const buf)
{
	/* unsigned integer type must be at least of 32 bits */
	UTF16CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	for (;;) {
		unsigned a = q[0];
		if (a >= 0x80) {
			unsigned r;
			if (a >= 0xE0) {
				if (a >= 0xF0) {
					r = q[1];
					a = (a << 6) ^ r;
					r = q[2];
					a = (a << 6) ^ r;
					r = q[3];
					a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
					/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
					 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
					*b++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
					a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
					q += 4;
				}
				else {
					r = q[1];
					a = (a << 6) ^ r;
					r = q[2];
					a = (a << 6) ^ r ^ 0xE2080;
					q += 3;
				}
			}
			else {
				r = q[1];
				a = (a << 6) ^ r ^ 0x3080;
				q += 2;
			}
		}
		else
			q++;
		/* make analyzer happy: this equivalent to:
		   *b++ = (UTF16CHAR)a; */
		buf[b++ - buf] = (UTF16CHAR)a;
		if (!a)
			return (size_t)(b - buf); /* ok, >0 */
	}
}

/* convert 'n' UTF8CHARs to utf16 ones (if 'n' is zero - input and output buffers are not used),
  do not check if there is enough space in output buffer, assume it is large enough.
 returns number of stored UTF16CHARs:
  0    - if 'n' is zero or an invalid utf8 character is encountered,
  else - number of stored UTF16CHARs */
/* Note: zero UTF8CHAR is not treated specially, i.e. conversion do not stops */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_When(!n, A_Ret_range(==,0))
A_Success(return)
A_When(n, A_At(q, A_In_reads(n)))
A_When(n, A_At(buf, A_Out A_Post_readable_size(return)))
#endif
static size_t utf8_to_utf16_unsafe_out(const UTF8CHAR *q, UTF16CHAR *const buf, const size_t n)
{
	if (n) {
		/* unsigned integer type must be at least of 32 bits */
		UTF16CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
		const UTF8CHAR *const qe = q + n;
		do {
			unsigned a = q[0];
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						if (a >= 0xF8)
							return 0; /* expecting max 4 utf8 bytes for a unicode code point */
						if ((size_t)(qe - q) < 4)
							return 0; /* incomplete utf8 character */
						r = q[1];
						if (0x80 != (r & 0xC0))
							return 0; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = q[2];
						if (0x80 != (r & 0xC0))
							return 0; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = q[3];
						if (0x80 != (r & 0xC0))
							return 0; /* incomplete utf8 character */
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						if (0x3600000 != (a & 0x3F00000))
							return 0; /* bad utf8 character */
						*b++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
						a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
						q += 4;
					}
					else {
						if ((size_t)(qe - q) < 3)
							return 0; /* incomplete utf8 character */
						r = q[1];
						if (0x80 != (r & 0xC0))
							return 0; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = q[2];
						if (0x80 != (r & 0xC0))
							return 0; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0xE2080;
						/* must not begin or end a surrogate pair */
						if (!a ||
							0xD800 == (a & 0xFC00) ||
							0xDC00 == (a & 0xFC00))
						{
							return 0; /* overlong/bad utf8 character */
						}
						q += 3;
					}
				}
				else if (a >= 0xC0) {
					if ((size_t)(qe - q) < 2)
						return 0; /* incomplete utf8 character */
					r = q[1];
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0x3080;
					if (!a)
						return 0; /* overlong utf8 character */
					q += 2;
				}
				else
					return 0; /* not expecting 10xxxxxx */
			}
			else
				q++;
			*b++ = (UTF16CHAR)a;
		} while (q != qe);
		return (size_t)(b - buf); /* ok, >0 */
	}
	return 0; /* n is zero */
}

/* same as utf8_to_utf16_unsafe_out(), but also do not check that input utf8 string is valid */
#ifdef SAL_DEFS_H_INCLUDED /* include "sal_defs.h" for the annotations */
A_When(!n, A_Ret_range(==,0))
A_When(n, A_Ret_range(>,0))
A_When(n, A_At(q, A_In_reads(n)))
A_When(n, A_At(buf, A_Out A_Post_readable_size(return)))
#endif
static size_t utf8_to_utf16_unsafe(const UTF8CHAR *q, UTF16CHAR *const buf, const size_t n)
{
	if (n) {
		/* unsigned integer type must be at least of 32 bits */
		UTF16CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
		const UTF8CHAR *const qe = q + n;
		do {
			unsigned a = q[0];
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						r = q[1];
						a = (a << 6) ^ r;
						r = q[2];
						a = (a << 6) ^ r;
						r = q[3];
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						*b++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
						a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
						q += 4;
					}
					else {
						r = q[1];
						a = (a << 6) ^ r;
						r = q[2];
						a = (a << 6) ^ r ^ 0xE2080;
						q += 3;
					}
				}
				else {
					r = q[1];
					a = (a << 6) ^ r ^ 0x3080;
					q += 2;
				}
			}
			else
				q++;
			*b++ = (UTF16CHAR)a;
		} while (q != qe);
		return (size_t)(b - buf); /* ok, >0 */
	}
	return 0; /* n is zero */
}

/* suppress warnings about unreferenced static functions */
typedef int utf8_to_utf16_z_t[sizeof(&utf8_to_utf16_z)];
typedef int utf8_to_utf16_z_unsafe_t[sizeof(&utf8_to_utf16_z_unsafe_out)];
typedef int utf8_to_utf16_z_unsafe_t[sizeof(&utf8_to_utf16_z_unsafe)];
typedef int utf8_to_utf16_t[sizeof(&utf8_to_utf16)];
typedef int utf8_to_utf16_unsafe_out_t[sizeof(&utf8_to_utf16_unsafe_out)];
typedef int utf8_to_utf16_unsafe_t[sizeof(&utf8_to_utf16_unsafe)];

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* UTF8_TO_UTF16_INL_INCLUDED */
