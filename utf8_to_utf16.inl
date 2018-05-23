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
  sz - free space in output buffer, in UTF16CHARs, if zero - output buffer is not used
 returns:
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
   last valid UTF8CHAR is not 0;
  (*b) - if sz > 0, points beyond the last stored (non-0) UTF16CHAR */
#if defined(A_Success) && \
    defined(A_At)      && \
	defined(A_Inout)   && \
	defined(A_In_z)    && \
	defined(A_When)    && \
	defined(A_Out)     && \
	defined(A_Out_writes)
/* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(q, A_Inout) A_At(*q, A_In_z)
A_When(sz, A_At(b, A_Inout) A_At(*b, A_Out_writes(sz)))
#endif
static size_t utf8_to_utf16(const UTF8CHAR **q, UTF16CHAR **b, size_t sz)
{
	const UTF8CHAR *s = *q;
	if (sz) {
		UTF16CHAR *d = *b;
		UTF16CHAR *const e = d + sz;
		/* unsigned integer type must be at least of 32 bits */
		sz = 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
		do {
			unsigned a = *s++;
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						if (a >= 0xF8) {
bad_utf8_1:
							*q = s - 1; /* (*q) != 0 */
							*b = d;
							return 0; /* max 4 utf8 bytes for unicode char */
						}
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_2:
							*q = s - 2; /* (*q) != 0 */
							*b = d;
							return 0; /* incomplete utf8 character */
						}
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_3:
							*q = s - 3; /* (*q) != 0 */
							*b = d;
							return 0; /* incomplete utf8 character */
						}
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_4:
							*q = s - 4; /* (*q) != 0 */
							*b = d;
							return 0; /* incomplete utf8 character */
						}
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						if (0x3600000 != (a & 0x3F00000))
							goto bad_utf8_4; /* bad utf8 character */
						if ((size_t)(e - d) < 2) {
							sz = 2;
							break; /* too small output buffer */
						}
						*d++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
						a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
					}
					else {
						r = *s++;
						if (0x80 != (r & 0xC0))
							goto bad_utf8_2; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0))
							goto bad_utf8_3; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0xE2080;
						/* must not begin or end a surrogate pair */
						if (!a ||
						    0xD800 == (a & 0xFC00) ||
						    0xDC00 == (a & 0xFC00))
						{
							goto bad_utf8_3; /* overlong/bad utf8 character */
						}
					}
				}
				else if (a >= 0xC0) {
					r = *s++;
					if (0x80 != (r & 0xC0))
						goto bad_utf8_2; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0x3080;
					if (!a)
						goto bad_utf8_2; /* overlong utf8 character */
				}
				else
					goto bad_utf8_1; /* not expecting 10xxxxxx */
			}
			*d++ = (UTF16CHAR)a;
			if (!a) {
				sz = (size_t)(d - *b);
				*q = s;
				*b = d;
				return sz; /* ok, >0 */
			}
		} while (d != e);
		/* too small output buffer */
		/* no size_t overflow here - each utf16 char needs at least 2 bytes,
		  so (size_t)(d - *b) is a half (or less) of output buffer size in bytes,
		  'sz' here 0 or 2 */
		sz += (size_t)(d - *b);
		*b = d;
	}
	{
		const UTF8CHAR *const t = s;
		size_t m = 0;
		for (;;) {
			unsigned a = *s++;
			if (a >= 0x80) {
				unsigned r;
				if (a >= 0xE0) {
					if (a >= 0xF0) {
						if (a >= 0xF8) {
bad_utf8_1_:
							*q = s - 1; /* (*q) != 0 */
							return 0; /* max 4 utf8 bytes for unicode char */
						}
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_2_:
							*q = s - 2; /* (*q) != 0 */
							return 0; /* incomplete utf8 character */
						}
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_3_:
							*q = s - 3; /* (*q) != 0 */
							return 0; /* incomplete utf8 character */
						}
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0)) {
bad_utf8_4_:
							*q = s - 4; /* (*q) != 0 */
							return 0; /* incomplete utf8 character */
						}
						a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
						/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
						 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
						if (0x3600000 != (a & 0x3F00000))
							goto bad_utf8_4_; /* bad utf8 character */
						m += 2; /* 4 utf8 chars - 2 utf16 chars */
					}
					else {
						r = *s++;
						if (0x80 != (r & 0xC0))
							goto bad_utf8_2_; /* incomplete utf8 character */
						a = (a << 6) ^ r;
						r = *s++;
						if (0x80 != (r & 0xC0))
							 goto bad_utf8_3_; /* incomplete utf8 character */
						a = (a << 6) ^ r ^ 0xE2080;
						/* must not begin or end a surrogate pair */
						if (!a ||
						    0xD800 == (a & 0xFC00) ||
						    0xDC00 == (a & 0xFC00))
						{
							goto bad_utf8_3_; /* overlong/bad utf8 character */
						}
						m += 2; /* 3 utf8 chars - 1 utf16 char */
					}
				}
				else if (a >= 0xC0) {
					r = *s++;
					if (0x80 != (r & 0xC0))
						goto bad_utf8_2_; /* incomplete utf8 character */
					a = (a << 6) ^ r ^ 0x3080;
					if (!a)
						goto bad_utf8_2_; /* overlong utf8 character */
					m += 1; /* 2 utf8 chars - 1 utf16 char */
				}
				else
					goto bad_utf8_1_; /* not expecting 10xxxxxx */
			}
			else if (!a) {
				/* no overflow of size_t is possible when counting the length of
				  resulting utf16 string (in utf16 characters), assuming that the
				  length of utf8 string may be stored in a variable of size_t without loss:
				 4 utf8 chars -> 2 utf16 surrogates,
				 3 utf8 chars -> 1 utf16 char,
				 2 utf8 chars -> 1 utf16 char,
				 1 utf8 char  -> 1 utf16 char */
				/* append a number of utf16 chars in utf8 string started from 't' */
				sz += (size_t)(s - t) - m;
				*q = t; /* points after the last successfully converted non-0 UTF8CHAR */
				return sz; /* ok, >0 */
			}
		}
	}
}

/* convert utf8 0-terminated string to utf16 0-terminated one,
  do not check if there is enough space in output buffer, assume it is large enough,
  returns:
   0    - if utf8 string is invalid,
   else - stored utf16 string length, including 0-terminator, in UTF16CHARs */
#if defined(A_Success) && \
    defined(A_At)      && \
	defined(A_In_z)    && \
	defined(A_Post_z)
/* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(q, A_In_z)
A_At(buf, A_Post_z)
#endif
static size_t utf8_to_utf16_unsafe(const UTF8CHAR *q, UTF16CHAR buf[])
{
	/* unsigned integer type must be at least of 32 bits */
	UTF16CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	for (;;) {
		unsigned a = *q++;
		if (a >= 0x80) {
			unsigned r;
			if (a >= 0xE0) {
				if (a >= 0xF0) {
					if (a >= 0xF8)
						return 0; /* max 4 utf8 bytes for unicode char */
					r = *q++;
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = *q++;
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = *q++;
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = ((a << 6) ^ r ^ 0xA82080) - 0x10000;
					/* a        = 11011aaaaabbbbbbbbcccccccc before substracting 0x10000,
					 a must match 110110xxxxxxxxxxxxxxxxxxxx after  substracting 0x10000 */
					if (0x3600000 != (a & 0x3F00000))
						return 0; /* bad utf8 character */
					*b++ = (UTF16CHAR)(a >> 10); /* 110110aaaabbbbbb */
					a = (a & 0x3FF) + 0xDC00;    /* 110111bbcccccccc */
				}
				else {
					r = *q++;
					if (0x80 != (r & 0xC0))
						return 0; /* incomplete utf8 character */
					a = (a << 6) ^ r;
					r = *q++;
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
				}
			}
			else if (a >= 0xC0) {
				r = *q++;
				if (0x80 != (r & 0xC0))
					return 0; /* incomplete utf8 character */
				a = (a << 6) ^ r ^ 0x3080;
				if (!a)
					return 0; /* overlong utf8 character */
			}
			else
				return 0; /* not expecting 10xxxxxx */
		}
		/* make analyzer happy: this equivalent to:
		   *b++ = (UTF16CHAR)a; */
		buf[b++ - buf] = (UTF16CHAR)a;
		if (!a)
			return (size_t)(b - buf); /* ok, >0 */
	}
}

/* suppress warnings about unreferenced static functions */
typedef int utf8_to_utf16_t[sizeof(&utf8_to_utf16)];
typedef int utf8_to_utf16_unsafe_t[sizeof(&utf8_to_utf16_unsafe)];

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* UTF8_TO_UTF16_INL_INCLUDED */
