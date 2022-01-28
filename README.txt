Common C/C++ definitions in header files

* Copyright (C) 2012-2022 Michael M. Builov, https://github.com/mbuilov/cmn_headers
* All files are licensed under Apache License v2.0, see LICENSE.TXT

Headers:

annotations.h

  A_Printf_format(f,v)       // printf-like format string at f (1-based parameter index), format arguments - at v
  A_Restrict                 // exclusive memory reference, reads may be prefetched, writes may be delayed
  A_Const_function           // function do not reads or writes memory
  A_Read_function            // function do not writes memory
  A_Noreturn_function        // function does not return (exits the program)
  A_Force_inline_function    // always inline function
  A_Non_inline_function      // do not inline function
  A_Hot_function             // optimize function for speed
  A_Cold_function            // optimize function for size

  ASSUME(cond)               // cond must be true: eliminate all runtime checks

asserts.h

  ASSERT(cond)               // cond must be true: runtime-check in DEBUG builds, ASSUME(cond) in RELEASE builds
  ASSERT_PTR(ptr)            // ASSERT() for pointer: pointer must be non-NULL

  DEBUG_CHECK(cond)          // cond must be true: runtime-check in DEBUG builds, in RELEASE builds - failure must be processed
  DEBUG_CHECK_PTR(ptr)       // DEBUG_CHECK() for pointer: pointer must be non-NULL

static_asserts.h

  STATIC_ASSERT(const_expr)  // compile-time check, implemented via typedef, cannot be placed inside expressions
  EMBED_ASSERT(const_expr)   // compile-time check, evaluates to 0, may be placed inside expressions

  STATIC_EXPR(const_expr)    // wrap constant expression to suppress MSVC warnings

bswaps.h

  bswap2(x)      // byte-swap of 2 bytes
  bswap4(x)      // byte-swap of 4 bytes
  bswap8(x)      // byte-swap of 8 bytes

  hswap4(x)      // half-swap of 4 bytes
  hswap8(x)      // half-swap of 8 bytes

  arch_is_be()   // CPU architecture is BIG-endian
  arch_is_le()   // CPU architecture is LITTLE-endian

ccasts.h

  CAST(type, ptr)               //  X*          -> type*
  CAST_CONSTANT(type, ptr)      //  const X*    -> const type*
  CONST_CAST(type, ptr)         //  const type* -> type*
  CAST_PPC(type, pptr)          //  const X**   -> const type**
  CAST_PP(type, pptr)           //  const X**   -> type*, const X*** -> type*, const X*const ** -> type*, ...

  CONTAINER_OF(ptr, type, member)        // get container of type 'type', ptr must be non-NULL pointer to 'member'
  OPT_CONTAINER_OF(optr, type, member)   // get container of type 'type', ptr may be non-NULL pointer to 'member'

countof.h

  COUNT_OF(arr)                       // number of elements in array, arr cannot be a pointer
  MEMBER_SIZE(type, member)           // sizeof of member 'member' of type 'type'
  COUNT_OF_MEMBER(type, member_arr)   // number of elements in array, member_arr cannot be a pointer
  CONST_STR_LEN(str)                  // get length of string constant at compile-time

dprint.h

  DBGPRINT(format, ...)                           // print message in DEBUG builds, noting in RELEASE builds
  DBGPRINTX(file, line, function, format, ...)

dprint_bt.h

  struct dbgtrace

  DBGPRINT_BT(bt, format, ...)                    // print back-trace and message in DEBUG builds, noting in RELEASE builds
  DBGPRINTX_BT(bt, file, line, function, format, ...)

get_opt.inl

  get_opt()                    // get next command line option
  opt_info_init()
  opt_info_is_end()
  opt_skip_unknown()
  opt_arg_is_option()
  opt_is_separate_value()
  opt_unread_separate_value()

get_opt_info.h

  struct opt_info              // structure for calling get_opt()

tagged_ptr.h

  PTR_ADD_TAG(type, ptr, tag)          // add small number 'tag' to a pointer value
  PTR_CLEAR_TAGS(type, ptr)            // clear tags of a pointer value
  PTR_GET_TAGS(ptr)                    // get tags of a pointer value
  PTR_MAKE_TAGGED(type, value, tag)    // make fake (invalid) tagged pointer with numeric value 'value'
