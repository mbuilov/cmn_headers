#ifndef BSWAPS_H_INCLUDED
#define BSWAPS_H_INCLUDED

/**********************************************************************************
* Byte-order swap routines
* Copyright (C) 2012-2018 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* Licensed under Apache License v2.0, see LICENSE.TXT
**********************************************************************************/

/* bswaps.h */

#ifdef _MSC_VER
#if !defined(BYTESWAP_UINT16) || !defined(BYTESWAP_UINT32) || !defined(BYTESWAP_UINT64)
#include <stdlib.h> /* for _byteswap_ushort()/_byteswap_ulong()/_byteswap_uint64() */
#define BYTESWAP_UINT16(x) _byteswap_ushort(x)
#define BYTESWAP_UINT32(x) _byteswap_ulong(x)
#define BYTESWAP_UINT64(x) _byteswap_uint64(x)
typedef int ULONG_TYPE_is_not_4_bytes_[1-2*(sizeof(unsigned long) != 4)];
#endif
#endif /* _MSC_VER */

/* define UINT16_TYPE, UINT32_TYPE, UINT64_TYPE if they are not already defined */

#ifndef UINT16_TYPE
#ifdef INT16_TYPE
#define UINT16_TYPE unsigned INT16_TYPE
#else
#define UINT16_TYPE unsigned short int
#endif
#endif

#ifndef UINT32_TYPE
#ifdef INT32_TYPE
#define UINT32_TYPE unsigned INT32_TYPE
#else
#define UINT32_TYPE unsigned int
#endif
#endif

#ifndef UINT64_TYPE
#ifdef INT64_TYPE
#define UINT64_TYPE unsigned INT64_TYPE
#else
#define UINT64_TYPE unsigned long long int
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* check that UINT16_TYPE is 16 bits long, UINT32_TYPE - 32 bits and  UINT64_TYPE - 64 bits */
typedef int CHAR_TYPE_is_not_8_bits_[1-2*(255 != (unsigned char)-1)];
typedef int UINT16_TYPE_is_not_2_bytes_[1-2*(sizeof(UINT16_TYPE) != 2)];
typedef int UINT32_TYPE_is_not_4_bytes_[1-2*(sizeof(UINT32_TYPE) != 4)];
typedef int UINT64_TYPE_is_not_8_bytes_[1-2*(sizeof(UINT64_TYPE) != 8)];

/* endian conversion procedures - swap bytes in a integer */

static inline UINT16_TYPE bswap2(UINT16_TYPE x)
{
#ifdef BYTESWAP_UINT16
	return BYTESWAP_UINT16(x);
#else
	/* gcc, clang, icc optimizes this to simple ror, cl - can't */
	return (UINT16_TYPE)(
		((UINT16_TYPE)(x & 0xFF00u) >> 8) |
		((UINT16_TYPE)(x & 0x00FFu) << 8)
	);
#endif
}

static inline UINT32_TYPE bswap4(UINT32_TYPE x)
{
#ifdef BYTESWAP_UINT32
	return BYTESWAP_UINT32(x);
#else
	/* gcc, clang, icc optimizes this to simple bswap, cl - can't */
	return (
		((x & 0xFF000000u) >> 24) |
		((x & 0x00FF0000u) >>  8) |
		((x & 0x0000FF00u) <<  8) |
		((x & 0x000000FFu) << 24)
	);
#endif
}

static inline UINT64_TYPE bswap8(UINT64_TYPE x)
{
#ifdef BYTESWAP_UINT64
	return BYTESWAP_UINT64(x);
#else
	/* gcc, clang, icc optimizes this to simple bswap, cl - can't */
	return (
		((x & 0xFF00000000000000ull) >> 56) |
		((x & 0x00FF000000000000ull) >> 40) |
		((x & 0x0000FF0000000000ull) >> 24) |
		((x & 0x000000FF00000000ull) >>  8) |
		((x & 0x00000000FF000000ull) <<  8) |
		((x & 0x0000000000FF0000ull) << 24) |
		((x & 0x000000000000FF00ull) << 40) |
		((x & 0x00000000000000FFull) << 56)
	);
#endif
}

/* half-swap 16-bit integer */
static inline UINT16_TYPE hswap2(UINT16_TYPE x)
{
	return bswap2(x);
}

/* half-swap 32-bit integer */
static inline UINT32_TYPE hswap4(UINT32_TYPE x)
{
	return (
		((x & 0xFFFF0000u) >> 16) |
		((x & 0x0000FFFFu) << 16)
	);
}

/* half-swap 64-bit integer */
static inline UINT64_TYPE hswap8(UINT64_TYPE x)
{
	return (
		((x & 0xFFFFFFFF00000000ull) >> 32) |
		((x & 0x00000000FFFFFFFFull) << 32)
	);
}

#ifdef __cplusplus
}
#endif

#endif /* BSWAPS_H_INCLUDED */
