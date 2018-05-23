#ifndef UTF16_TO_UTF8_INL_INCLUDED
#define UTF16_TO_UTF8_INL_INCLUDED

/**********************************************************************************
* UTF-16 to UTF-8 characters conversion
* Copyright (C) 2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* utf8_to_utf16.inl */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4505) /* unreferenced local function has been removed */
#endif

/* utf16_to_utf8.inl */

/* type of utf16 character,
  must be unsigned integer of at least 16 bits */
#ifndef UTF16CHAR
#ifdef _WIN32
#define UTF16CHAR wchar_t
#else
#define UTF16CHAR unsigned short
#endif
#endif
typedef int (*utf16_to_utf8_bad_UTF16CHAR)[1-2*((UTF16CHAR)-1 < 0 || (UTF16CHAR)-1 < 0xFFFFu)];

/* type of utf8 character,
  must be unsigned integer of at least 8 bits */
#ifndef UTF8CHAR
#define UTF8CHAR unsigned char
#endif
typedef int (*utf16_to_utf8_bad_UTF8CHAR)[1-2*((UTF8CHAR)-1 < 0 || (UTF8CHAR)-1 < 0xFFu)];

/* convert utf16 0-terminated string to utf8 0-terminated one,
 input:
  w  - address of the pointer to the beginning of input 0-terminated utf16 string,
  b  - optional address of the pointer to the beginning of output buffer,
  sz - free space in output buffer, in UTF8CHARs, if zero - output buffer is not used
 returns:
  0     - if utf16 string is invalid or too long,
  <= sz - 0-terminated utf8 string was successfully stored in output buffer,
  > sz  - output buffer is too small, return value is the required buffer size to store
   whole converted utf8 0-terminated string, including the part that is already converted
   and stored in output buffer, including 0-terminator, in UTF8CHARs;
 - on success:
  (*w) - points beyond the 0-terminator of input utf16 string,
  (*b) - points beyond the 0-terminator stored in output buffer;
 - if output buffer is too small:
  (*w) - if sz == 0, not changed, else - points beyond the last converted (non-0) UTF16CHAR,
  (*b) - if sz == 0, not changed, else - points beyond the last stored (non-0) UTF8CHAR;
 - if input utf16 string is invalid or too long:
  (*w) - points beyond the last valid UTF16CHAR, if output buffer is too small,
   it may be different from the last successfully converted UTF16CHAR,
   if input utf16 string is too long, the last valid UTF16CHAR is the 0-terminator,
   if input utf16 string is invalid, the last valid UTF16CHAR is not 0;
  (*b) - if sz > 0, points beyond the last stored (non-0) UTF8CHAR */
#if defined(A_Success) && \
    defined(A_At)      && \
	defined(A_Inout)   && \
	defined(A_In_z)    && \
	defined(A_When)    && \
	defined(A_Out)     && \
	defined(A_Out_writes)
/* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(w, A_Inout) A_At(*w, A_In_z)
A_When(sz, A_At(b, A_Inout) A_At(*b, A_Out_writes(sz)))
#endif
static size_t utf16_to_utf8(const UTF16CHAR **w, UTF8CHAR **b, size_t sz)
{
	/* unsigned integer type must be at least of 32 bits */
	size_t m = 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	const UTF16CHAR *s = *w;
	if (sz) {
		UTF8CHAR *d = *b;
		UTF8CHAR *const e = d + sz;
		do {
			unsigned c = *s++;
			if (c >= 0x80) {
				if (c >= 0x800) {
					if (0xD800 == (c & 0xFC00)) {
						unsigned r = *s++;
						if (0xDC00 != (r & 0xFC00)) {
							*w = s - 2; /* (*w) != 0 */
							*b = d;
							return 0; /* bad utf16 surrogate pair: no lower surrogate */
						}
						if ((size_t)(e - d) < 4) {
							m = 4;
							break; /* too small output buffer */
						}
						/* c is in range: 0xD800..0xDBFF, i.e.: 110110xxyyyyyyyy */
						/* r is in range: 0xDC00..0xDFFF, i.e.: 110111aabbbbbbbb */
						c = ((c << 10) ^ r ^ 0xA0DC00) + 0x10000;
						*d++ = (UTF8CHAR)(c >> 18);
						*d++ = (UTF8CHAR)(((c >> 12) | 0x80) & 0xBF);
					}
					else if (0xDC00 == (c & 0xFC00)) {
						*w = s - 1; /* (*w) != 0 */
						*b = d;
						return 0; /* bad utf16 surrogate pair: no high surrogate */
					}
					else if ((size_t)(e - d) < 3) {
						m = 3;
						break; /* too small output buffer */
					}
					else
						*d++ = (UTF8CHAR)((c >> 12) | 0xE0);
					*d++ = (UTF8CHAR)(((c >> 6) | 0x80) & 0xBF);
				}
				else if ((size_t)(e - d) < 2) {
					m = 2;
					break; /* too small output buffer */
				}
				else
					*d++ = (UTF8CHAR)((c >> 6) | 0xC0);
				*d++ = (UTF8CHAR)((c | 0x80) & 0xBF);
			}
			else {
				*d++ = (UTF8CHAR)c;
				if (!c) {
					sz = (size_t)(d - *b);
					*w = s;
					*b = d;
					return sz; /* ok, >0 */
				}
			}
		} while (d != e);
		/* too small output buffer */
		sz = (size_t)(d - *b);
		*b = d;
	}
	/* NOTE: assume total size in bytes of input utf16 string, including terminating 0,
	  may be stored in a variable of size_t type without loss, so for each utf16 char may
	  safely increment 'm' at least by 2 without integer overflow */
	/* NOTE: also, 'm' will not overflow, if it's zero or have one of next values:
	  2 - at least one utf16 char (2 bytes) was checked, no overflow of 'm' is possible,
	  3 - at least one utf16 char (2 bytes) was checked and, because input utf16 string is
	   0-terminated, but for the last 0 'm' is not incremented - no overflow of 'm' is possible,
	  4 - at least two utf16 chars (4 bytes) were checked, no overflow of 'm' is possible */
	{
		const UTF16CHAR *const t = s;
		for (;;) {
			unsigned c = *s++;
			if (c >= 0x80) {
				if (c >= 0x800) {
					if (0xD800 == (c & 0xFC00)) {
						c = *s++;
						if (0xDC00 != (c & 0xFC00)) {
							*w = s - 2; /* (*w) != 0 */
							return 0; /* bad utf16 surrogate pair: no lower surrogate */
						}
					}
					else if (0xDC00 == (c & 0xFC00)) {
						*w = s - 1; /* (*w) != 0 */
						return 0; /* bad utf16 surrogate pair: no high surrogate */
					}
					m++;
				}
				m++;
			}
			else if (!c) {
#ifdef UBSAN_UNSIGNED_OVERFLOW
				if (sz > (size_t)~(size_t)0 - m)
					goto too_long;
#endif
				sz += m;
#ifndef UBSAN_UNSIGNED_OVERFLOW
				if (sz < m)
					goto too_long;
#endif
				m = (size_t)(s - t);
#ifdef UBSAN_UNSIGNED_OVERFLOW
				if (sz > (size_t)~(size_t)0 - m)
					goto too_long;
#endif
				sz += m;
#ifndef UBSAN_UNSIGNED_OVERFLOW
				if (sz < m)
					goto too_long;
#endif
				*w = t; /* points after the last successfully converted non-0 UTF16CHAR */
				return sz; /* ok, >0 */
too_long:
				*w = s; /* points after the 0-terminator */
				return 0; /* integer overflow, input string is too long */
			}
		}
	}
}

/* convert utf16 0-terminated string to utf8 0-terminated one,
  do not check if there is enough space in output buffer, assume it is large enough,
  returns:
   0    - if utf16 string is invalid,
   else - stored utf8 string length, including 0-terminator, in UTF8CHARs */
#if defined(A_Success) && \
    defined(A_At)      && \
	defined(A_In_z)    && \
	defined(A_Post_z)
/* include "sal_defs.h" for the annotations */
A_Success(return)
A_At(w, A_In_z)
A_At(buf, A_Post_z)
#endif
static size_t utf16_to_utf8_unsafe(const UTF16CHAR *w, UTF8CHAR buf[])
{
	/* unsigned integer type must be at least of 32 bits */
	UTF8CHAR *b = buf + 0*sizeof(int(*)[1-2*((unsigned)-1 < 0xFFFFFFFF)]);
	for (;;) {
		unsigned c = *w++;
		if (c >= 0x80) {
			if (c >= 0x800) {
				if (0xD800 == (c & 0xFC00)) {
					unsigned r = *w++;
					if (0xDC00 != (r & 0xFC00))
						return 0; /* bad utf16 surrogate pair: no lower surrogate */
					c = ((c << 10) ^ r ^ 0xA0DC00) + 0x10000;
					*b++ = (UTF8CHAR)(c >> 18);
					*b++ = (UTF8CHAR)(((c >> 12) | 0x80) & 0xBF);
				}
				else if (0xDC00 == (c & 0xFC00))
					return 0; /* bad utf16 surrogate pair: no high surrogate */
				else
					*b++ = (UTF8CHAR)((c >> 12) | 0xE0);
				*b++ = (UTF8CHAR)(((c >> 6) | 0x80) & 0xBF);
			}
			else
				*b++ = (UTF8CHAR)((c >> 6) | 0xC0);
			*b++ = (UTF8CHAR)((c | 0x80) & 0xBF);
		}
		else {
			/* make analyzer happy: this equivalent to:
			   *b++ = (UTF8CHAR)c; */
			buf[b++ - buf] = (UTF8CHAR)c;
			if (!c)
				return (size_t)(b - buf); /* ok, >0 */
		}
	}
}

/* suppress warnings about unreferenced static functions */
typedef int utf16_to_utf8_t[sizeof(&utf16_to_utf8)];
typedef int utf16_to_utf8_unsafe_t[sizeof(&utf16_to_utf8_unsafe)];

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* UTF16_TO_UTF8_INL_INCLUDED */
