#ifndef SAL_DEFS_H_INCLUDED
#define SAL_DEFS_H_INCLUDED

/********************************************************************************
* Static analysis definitions
* Public domain, see latest version at https://github.com/mbuilov/cmn_headers
********************************************************************************/

/* sal_defs.h */

#if defined _MSC_VER && _MSC_VER >= 1600 && !defined NO_SAL_ANNOTATIONS
/* SAL annotations, see http://www.codeproject.com/Reference/879527/SAL-Function-Parameters-Annotations */
#include <sal.h>
/* NOTE: post-annotations considered valid only if A_Success() condition is true or not specified */
#define A_Restrict                               __restrict
#define A_Noreturn_function                      __declspec(noreturn)
#define A_Const_function
#define A_Pure_function
#define A_Force_inline_function                  __forceinline
#define A_Non_inline_function                    __declspec(noinline)
#define A_Non_const_function
#define A_Non_pure_function
#define A_Empty                                  /* empty, use to workaround bugs in compiler */
#define A_Curr                                   _Curr_
#define A_Old                                    _Old_
#define A_In                                     _In_
#define A_In_opt                                 _In_opt_
#define A_In_z                                   _In_z_
#define A_In_opt_z                               _In_opt_z_
#define A_Inout                                  _Inout_
#define A_Inout_opt                              _Inout_opt_
#define A_Inout_z                                _Inout_z_
#define A_Inout_opt_z                            _Inout_opt_z_
#define A_Out                                    _Out_
#define A_Out_opt                                _Out_opt_
#define A_Outptr                                 _Outptr_
#define A_Outptr_opt                             _Outptr_opt_
#define A_Outptr_result_maybenull                _Outptr_result_maybenull_
#define A_Outptr_opt_result_maybenull            _Outptr_opt_result_maybenull_
#define A_Outptr_result_z                        _Outptr_result_z_
#define A_Outptr_opt_result_z                    _Outptr_opt_result_z_
#define A_Outptr_result_maybenull_z              _Outptr_result_maybenull_z_
#define A_Outptr_opt_result_maybenull_z          _Outptr_opt_result_maybenull_z_
#define A_In_reads(s)                            _In_reads_(s)
#define A_In_reads_opt(s)                        _In_reads_opt_(s)
#define A_In_reads_bytes(s)                      _In_reads_bytes_(s)
#define A_In_reads_bytes_opt(s)                  _In_reads_bytes_opt_(s)
#define A_In_reads_to_ptr(end)                   _In_reads_to_ptr_(end)
#define A_In_reads_to_ptr_opt(end)               _In_reads_to_ptr_opt_(end)
#define A_Inout_updates(s)                       _Inout_updates_(s)
#define A_Inout_updates_opt(s)                   _Inout_updates_opt_(s)
#define A_Inout_updates_bytes(s)                 _Inout_updates_bytes_(s)
#define A_Inout_updates_bytes_opt(s)             _Inout_updates_bytes_opt_(s)
#define A_Inout_updates_to(s,c)                  _Inout_updates_to_(s,c)
#define A_Inout_updates_to_opt(s,c)              _Inout_updates_to_opt_(s,c)
#define A_Inout_updates_all(s)                   _Inout_updates_all_(s)
#define A_Inout_updates_all_opt(s)               _Inout_updates_all_opt_(s)
#define A_Inout_updates_bytes_to(s,c)            _Inout_updates_bytes_to_(s,c)
#define A_Inout_updates_bytes_to_opt(s,c)        _Inout_updates_bytes_to_opt_(s,c)
#define A_Inout_updates_bytes_all(s)             _Inout_updates_bytes_all_(s)
#define A_Inout_updates_bytes_all_opt(s)         _Inout_updates_bytes_all_opt_(s)
#define A_Out_writes(s)                          _Out_writes_(s)
#define A_Out_writes_opt(s)                      _Out_writes_opt_(s)
#define A_Out_writes_bytes(s)                    _Out_writes_bytes_(s)
#define A_Out_writes_bytes_opt(s)                _Out_writes_bytes_opt_(s)
#define A_Out_writes_to_ptr(end)                 _Out_writes_to_ptr_(end)
#define A_Out_writes_to_ptr_opt(end)             _Out_writes_to_ptr_opt_(end)
#define A_Out_writes_to(s,c)                     _Out_writes_to_(s,c)
#define A_Out_writes_to_opt(s,c)                 _Out_writes_to_opt_(s,c)
#define A_Out_writes_all(s)                      _Out_writes_all_(s)
#define A_Out_writes_all_opt(s)                  _Out_writes_all_opt_(s)
#define A_Out_writes_bytes_to(s,c)               _Out_writes_bytes_to_(s,c)
#define A_Out_writes_bytes_to_opt(s,c)           _Out_writes_bytes_to_opt_(s,c)
#define A_Out_writes_bytes_all(s)                _Out_writes_bytes_all_(s)
#define A_Out_writes_bytes_all_opt(s)            _Out_writes_bytes_all_opt_(s)
#define A_Ret_null                               _Ret_null_
#define A_Ret_notnull                            _Ret_notnull_
#define A_Ret_maybenull                          _Ret_maybenull_
#define A_Ret_z                                  _Ret_z_
#define A_Ret_maybenull_z                        _Ret_maybenull_z_
#define A_Ret_valid                              _Ret_valid_
#define A_Ret_opt_valid                          _Ret_opt_valid_
#define A_Ret_writes(s)                          _Ret_writes_(s)
#define A_Ret_writes_z(s)                        _Ret_writes_z_(s)
#define A_Ret_writes_bytes(s)                    _Ret_writes_bytes_(s)
#define A_Ret_writes_maybenull(s)                _Ret_writes_maybenull_(s)
#define A_Ret_writes_maybenull_z(s)              _Ret_writes_maybenull_z_(s)
#define A_Ret_writes_bytes_maybenull(s)          _Ret_writes_bytes_maybenull_(s)
#define A_Ret_writes_to(s,c)                     _Ret_writes_to_(s,c)
#define A_Ret_writes_bytes_to(s,c)               _Ret_writes_bytes_to_(s,c)
#define A_Ret_writes_to_maybenull(s,c)           _Ret_writes_to_maybenull_(s,c)
#define A_Ret_writes_bytes_to_maybenull(s,c)     _Ret_writes_bytes_to_maybenull_(s,c)
#define A_Check_return                           _Check_return_
#define A_Must_inspect_result                    _Must_inspect_result_
#define A_In_range(l,h)                          _In_range_(l,h)
#define A_Out_range(l,h)                         _Out_range_(l,h)
#define A_Ret_range(l,h)                         _Ret_range_(l,h)
#define A_Deref_in_range(l,h)                    _Deref_in_range_(l,h)
#define A_Deref_out_range(l,h)                   _Deref_out_range_(l,h)
#define A_Deref_ret_range(l,h)                   _Deref_ret_range_(l,h)
#define A_Pre_equal_to(v)                        _Pre_equal_to_(v)
#define A_Post_equal_to(v)                       _Post_equal_to_(v)
#define A_Pre_satisfies(cond)                    _Pre_satisfies_(cond)
#define A_Post_satisfies(cond)                   _Post_satisfies_(cond)
#define A_Success(expr)                          _Success_(expr)
#define A_Return_type_success(expr)              _Return_type_success_(expr)
#define A_Result_nullonfailure                   _Result_nullonfailure_
#define A_Result_zeroonfailure                   _Result_zeroonfailure_
#define A_Outptr_result_nullonfailure            _Outptr_result_nullonfailure_
#define A_Outptr_opt_result_nullonfailure        _Outptr_opt_result_nullonfailure_
#define A_Printf_format_string                   _Printf_format_string_
#define e_A_Group(anns,empty)                    _Group_(anns##empty)
#define A_Group(anns)                            e_A_Group(anns,A_Empty)
#define e_A_On_failure(anns,empty)               _On_failure_(anns##empty)
#define A_On_failure(anns)                       e_A_On_failure(anns,A_Empty)
#define e_A_Always(anns,empty)                   _Always_(anns##empty)
#define A_Always(anns)                           e_A_Always(anns,A_Empty)
#define A_At(at,anns)                            _At_(at,anns)
#define A_When(cond,anns)                        _When_(cond,anns)
#define A_Post_z                                 _Post_z_
#define A_Post_valid                             _Post_valid_
#define A_Post_invalid                           _Post_invalid_
#define A_Post_ptr_invalid                       _Post_ptr_invalid_
#define A_Post_null                              _Post_null_
#define A_Post_notnull                           _Post_notnull_
#define A_Post_maybenull                         _Post_maybenull_
#define A_Pre_valid                              _Pre_valid_
#define A_Pre_opt_valid                          _Pre_opt_valid_
#define A_Pre_invalid                            _Pre_invalid_
#define A_Pre_unknown                            _Pre_unknown_
#define A_Pre_notnull                            _Pre_notnull_
#define A_Pre_maybenull                          _Pre_maybenull_
#define A_Pre_null                               _Pre_null_
#define A_Use_decl_annotations                   _Use_decl_annotations_
#define A_Pre                                    _Pre_
#define A_Post                                   _Post_
#define A_Null                                   _Null_
#define A_Notnull                                _Notnull_
#define A_Maybenull                              _Maybenull_
#define A_Valid                                  _Valid_
#define A_Notvalid                               _Notvalid_
#define A_Maybevalid                             _Maybevalid_
#define A_Readable_bytes(n)                      _Readable_bytes_(n)
#define A_Readable_elements(n)                   _Readable_elements_(n)
#define A_Writable_bytes(n)                      _Writable_bytes_(n)
#define A_Writable_elements(n)                   _Writable_elements_(n)
#define A_Null_terminated                        _Null_terminated_
#define A_Pre_readable_size(s)                   _Pre_readable_size_(s)
#define A_Pre_writable_size(s)                   _Pre_writable_size_(s)
#define A_Pre_readable_byte_size(s)              _Pre_readable_byte_size_(s)
#define A_Pre_writable_byte_size(s)              _Pre_writable_byte_size_(s)
#define A_Post_readable_size(s)                  _Post_readable_size_(s)
#define A_Post_writable_size(s)                  _Post_writable_size_(s)
#define A_Post_readable_byte_size(s)             _Post_readable_byte_size_(s)
#define A_Post_writable_byte_size(s)             _Post_writable_byte_size_(s)
#define A_Struct_size_bytes(s)                   _Struct_size_bytes_(s)
#define A_Field_size(s)                          _Field_size_(s)
#define A_Field_size_opt(s)                      _Field_size_opt_(s)
#define A_Field_size_part(s,c)                   _Field_size_part_(s,c)
#define A_Field_size_part_opt(s,c)               _Field_size_part_opt_(s,c)
#define A_Field_size_full(s)                     _Field_size_full_(s)
#define A_Field_size_full_opt(s)                 _Field_size_full_opt_(s)
#define A_Field_size_bytes(s)                    _Field_size_bytes_(s)
#define A_Field_size_bytes_opt(s)                _Field_size_bytes_opt_(s)
#define A_Field_size_bytes_part(s,c)             _Field_size_bytes_part_(s,c)
#define A_Field_size_bytes_part_opt(s,c)         _Field_size_bytes_part_opt_(s,c)
#define A_Field_size_bytes_full(s)               _Field_size_bytes_full_(s)
#define A_Field_size_bytes_full_opt(s)           _Field_size_bytes_full_opt_(s)
#define A_Field_z                                _Field_z_
#define A_Field_range(min,max)                   _Field_range_(min,max)
#define A_Ret_restrict                           __declspec(restrict)
#define A_Ret_never_null                         _Always_(_Ret_notnull_)
#define A_Nonnull_all_args
#define A_Nonnull_arg(i)
#define A_Printf_format_at(f,v)
#else /* !_MSC_VER */
#if defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 2))
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define A_Restrict                               restrict
#else
#define A_Restrict                               __restrict__
#endif
#define A_Noreturn_function                      __attribute__ ((noreturn))
#define A_Const_function                         __attribute__ ((const))
#define A_Pure_function                          __attribute__ ((pure))
#define A_Force_inline_function                  __attribute__ ((always_inline)) inline
#define A_Non_inline_function                    __attribute__ ((noinline))
#define A_Non_const_function                     __attribute__ ((const))      /* only for function definition - to silence warning */
#define A_Non_pure_function                      __attribute__ ((pure))       /* only for function definition - to silence warning */
#define A_Check_return                           __attribute__ ((warn_unused_result))
#else /* no GCC extensions */
#define A_Restrict
#define A_Noreturn_function                      /* function which never returns - calls exit()                                    */
#define A_Const_function                         /* declare function without side effects, cannot access any memory by pointer     */
#define A_Pure_function                          /* declare function without side effects, may read but cannot write memory        */
#define A_Force_inline_function inline           /* forcibly inlined function                                                      */
#define A_Non_inline_function                    /* forbid inlining a function                                                     */
#define A_Non_const_function                     /* define function as const to silence "-Wsuggest-attribute=const" warning        */
#define A_Non_pure_function                      /* define function as pure to silence "-Wsuggest-attribute=pure" warning          */
#define A_Check_return                           /* caller must check function's return value                                      */
#endif /* no GCC extensions */
#define A_Ret_restrict                           /* returned pointer is the only alias to allocated memory - result of malloc()    */
#if (defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 9))) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Ret_never_null                         __attribute__ ((returns_nonnull))
#define A_Nonnull_all_args                       __attribute__ ((nonnull))
#define A_Nonnull_arg(i)                         __attribute__ ((nonnull(i,i)))
#define A_Printf_format_at(f,v)                  __attribute__ ((format(printf, f, v)))
#else /* no GCC extensions */
#define A_Ret_never_null                         /* never returns NULL, even if A_Success() is false                               */
#define A_Nonnull_all_args                       /* all function arguments pointers are != NULL                                    */
#define A_Nonnull_arg(i)                         /* function argument number i is != NULL                                          */
#define A_Printf_format_at(f,v)                  /* f arg - printf format string, v - va_arg                                       */
#endif /* no GCC extensions */
#define A_Curr                                   /* references current function parameter, like A_At(*A_Curr, A_Post_notnull)      */
#define A_Old                                    /* old value, like A_Old(*A_Curr)                                                 */
#define A_In                                     /* readonly,      !=NULL                                                          */
#define A_In_opt                                 /* readonly,       NULL?                                                          */
#define A_In_z                                   /* readonly,      !=NULL, '\0'-terminated string                                  */
#define A_In_opt_z                               /* readonly,       NULL?, '\0'-terminated string                                  */
#define A_Inout                                  /* read-write,    !=NULL                                                          */
#define A_Inout_opt                              /* read-write,     NULL?                                                          */
#define A_Inout_z                                /* read-write,    !=NULL, '\0'-terminated string                                  */
#define A_Inout_opt_z                            /* read-write,     NULL?, '\0'-terminated string                                  */
#define A_Out                                    /* uninitialized, !=NULL                                                          */
#define A_Out_opt                                /* uninitialized,  NULL?                                                          */
#define A_Outptr                                 /* uninitialized, !=NULL, (*p) !=NULL                                             */
#define A_Outptr_opt                             /* uninitialized,  NULL?, (*p) !=NULL                                             */
#define A_Outptr_result_maybenull                /* uninitialized, !=NULL, (*p)  NULL?                                             */
#define A_Outptr_opt_result_maybenull            /* uninitialized,  NULL?, (*p)  NULL?                                             */
#define A_Outptr_result_z                        /* uninitialized, !=NULL, (*p) !=NULL, '\0'-terminated                            */
#define A_Outptr_opt_result_z                    /* uninitialized,  NULL?, (*p) !=NULL, '\0'-terminated                            */
#define A_Outptr_result_maybenull_z              /* uninitialized, !=NULL, (*p)  NULL?, '\0'-terminated                            */
#define A_Outptr_opt_result_maybenull_z          /* uninitialized,  NULL?, (*p)  NULL?, '\0'-terminated                            */
#define A_In_reads(s)                            /* readonly,      !=NULL, reads s-elements                                        */
#define A_In_reads_opt(s)                        /* readonly,       NULL?, reads s-elements                                        */
#define A_In_reads_bytes(s)                      /* readonly,      !=NULL, reads s-bytes                                           */
#define A_In_reads_bytes_opt(s)                  /* readonly,       NULL?, reads s-bytes                                           */
#define A_In_reads_to_ptr(end)                   /* readonly,      !=NULL, reads until end                                         */
#define A_In_reads_to_ptr_opt(end)               /* readonly,       NULL?, reads until end                                         */
#define A_Inout_updates(s)                       /* read-write,    !=NULL, array of s-elements, unknown number of updated elements */
#define A_Inout_updates_opt(s)                   /* read-write,     NULL?, array of s-elements, unknown number of updated elements */
#define A_Inout_updates_bytes(s)                 /* read-write,    !=NULL, array of s-bytes,    unknown number of updated bytes    */
#define A_Inout_updates_bytes_opt(s)             /* read-write,     NULL?, array of s-bytes,    unknown number of updated bytes    */
#define A_Inout_updates_to(s,c)                  /* read-write,    !=NULL, array of s-elements, c elements updated                 */
#define A_Inout_updates_to_opt(s,c)              /* read-write,     NULL?, array of s-elements, c elements updated                 */
#define A_Inout_updates_all(s)                   /* read-write,    !=NULL, array of s-elements, all elements updated               */
#define A_Inout_updates_all_opt(s)               /* read-write,     NULL?, array of s-elements, all elements updated               */
#define A_Inout_updates_bytes_to(s,c)            /* read-write,    !=NULL, array of s-bytes,    c bytes updated                    */
#define A_Inout_updates_bytes_to_opt(s,c)        /* read-write,     NULL?, array of s-bytes,    c bytes updated                    */
#define A_Inout_updates_bytes_all(s)             /* read-write,    !=NULL, array of s-bytes,    all bytes updated                  */
#define A_Inout_updates_bytes_all_opt(s)         /* read-write,     NULL?, array of s-bytes,    all bytes updated                  */
#define A_Out_writes(s)                          /* uninitialized, !=NULL, array of s-elements, unknown number of written elements */
#define A_Out_writes_opt(s)                      /* uninitialized,  NULL?, array of s-elements, unknown number of written elements */
#define A_Out_writes_bytes(s)                    /* uninitialized, !=NULL, array of s-bytes,    unknown number of written bytes    */
#define A_Out_writes_bytes_opt(s)                /* uninitialized,  NULL?, array of s-bytes,    unknown number of written bytes    */
#define A_Out_writes_to_ptr(end)                 /* uninitialized, !=NULL, writes until end                                        */
#define A_Out_writes_to_ptr_opt(end)             /* uninitialized,  NULL?, writes until end                                        */
#define A_Out_writes_to(s,c)                     /* uninitialized, !=NULL, array of s-elements, c elements written                 */
#define A_Out_writes_to_opt(s,c)                 /* uninitialized,  NULL?, array of s-elements, c elements written                 */
#define A_Out_writes_all(s)                      /* uninitialized, !=NULL, array of s-elements, all elements written               */
#define A_Out_writes_all_opt(s)                  /* uninitialized,  NULL?, array of s-elements, all elements written               */
#define A_Out_writes_bytes_to(s,c)               /* uninitialized, !=NULL, array of s-bytes,    c bytes written                    */
#define A_Out_writes_bytes_to_opt(s,c)           /* uninitialized,  NULL?, array of s-bytes,    c bytes written                    */
#define A_Out_writes_bytes_all(s)                /* uninitialized, !=NULL, array of s-bytes,    all bytes written                  */
#define A_Out_writes_bytes_all_opt(s)            /* uninitialized,  NULL?, array of s-bytes,    all bytes written                  */
#define A_Ret_null                               /* returns  NULL                                                                  */
#define A_Ret_notnull                            /* returns != NULL, not initialized object                                        */
#define A_Ret_maybenull                          /* returns  NULL?, not initialized object                                         */
#define A_Ret_z                                  /* returns !=NULL, '\0'-terminated string                                         */
#define A_Ret_maybenull_z                        /* returns  NULL?, '\0'-terminated string                                         */
#define A_Ret_valid                              /* returns !=NULL, initialized object                                             */
#define A_Ret_opt_valid                          /* returns  NULL?, initialized object                                             */
#define A_Ret_writes(s)                          /* returns !=NULL, initialized buffer of s-elements                               */
#define A_Ret_writes_z(s)                        /* returns !=NULL, '\0-terminated buffer of s-elements                            */
#define A_Ret_writes_bytes(s)                    /* returns !=NULL, initialized buffer of s-bytes                                  */
#define A_Ret_writes_maybenull(s)                /* returns  NULL?, initialized buffer of s-elements                               */
#define A_Ret_writes_maybenull_z(s)              /* returns  NULL?, '\0-terminated buffer of s-elements                            */
#define A_Ret_writes_bytes_maybenull(s)          /* returns  NULL?, initialized buffer of s-bytes                                  */
#define A_Ret_writes_to(s,c)                     /* returns !=NULL, partially initialized to c-elements buffer of s-elements       */
#define A_Ret_writes_bytes_to(s,c)               /* returns !=NULL, partially initialized to c-bytes buffer of s-bytes             */
#define A_Ret_writes_to_maybenull(s,c)           /* returns  NULL?, partially initialized to c-elements buffer of s-elements       */
#define A_Ret_writes_bytes_to_maybenull(s,c)     /* returns  NULL?, partially initialized to c-bytes buffer of s-bytes             */
#define A_Ret_z_t                                /* for typedef                                                                    */
#define A_Ret_valid_t                            /* for typedef                                                                    */
#define A_Ret_writes_t(s)                        /* for typedef                                                                    */
#define A_Ret_writes_z_t(s)                      /* for typedef                                                                    */
#define A_Ret_writes_bytes_t(s)                  /* for typedef                                                                    */
#define A_Ret_writes_to_t(s,c)                   /* for typedef                                                                    */
#define A_Ret_writes_bytes_to_t(s,c)             /* for typedef                                                                    */
#define A_Must_inspect_result                    /* caller must check value of out-parameter                                       */
#define A_In_range(l,h)                          /* input parameter value is in given range                                        */
#define A_Out_range(l,h)                         /* NULL? output parameter value is in given range, usable for C++ references      */
#define A_Ret_range(l,h)                         /* return value is in given range                                                 */
#define A_Deref_in_range(l,h)                    /* NULL? pointer to value in given range                                          */
#define A_Deref_out_range(l,h)                   /* NULL? pointer to value in given range                                          */
#define A_Deref_ret_range(l,h)                   /* NULL? pointer to value in given range                                          */
#define A_Pre_equal_to(v)                        /* shortcut to A_In_range(=,v)                                                    */
#define A_Post_equal_to(v)                       /* shortcut to A_Out_range(=,v), usable for C++ references                        */
#define A_Pre_satisfies(cond)                    /* custom condition for input parameter                                           */
#define A_Post_satisfies(cond)                   /* custom condition for output parameter                                          */
#define A_Success(expr)                          /* successful return                                                              */
#define A_Return_type_success(expr)              /* typedef A_Return_type_success(return >= 0) int ret_t                           */
#define A_Result_nullonfailure                   /* out parameter is NULL on failure                                               */
#define A_Result_zeroonfailure                   /* out parameter is zero on failure                                               */
#define A_Outptr_result_nullonfailure            /* A_Outptr     value is NULL on failure                                          */
#define A_Outptr_opt_result_nullonfailure        /* A_Outptr_opt value is NULL on failure                                          */
#define A_Printf_format_string                   /* printf-like format string                                                      */
#define A_Group(anns)                            /* apply annotations if A_Success(expr) gives true result                         */
#define A_On_failure(anns)                       /* apply annotations if A_Success(expr) gives false result                        */
#define A_Always(anns)                           /* apply annotations even if A_Success(expr) gives false result                   */
#define A_At(at,anns)                            /* A_On_failure(_At(*p, A_Post_null)) T **p                                       */
#define A_When(cond,anns)                        /* A_When(return != 0, A_Out_writes(size)) T p[]                                  */
#define A_Post_z                                 /* !=NULL, buffer will be '\0'-terminated after the call                          */
#define A_Post_valid                             /* !=NULL,   legal post read:       init(A_Post_valid struct X *x) ;              */
#define A_Post_invalid                           /*  NULL?, illegal post read:       fini(A_Post_invalid struct X *x);             */
#define A_Post_ptr_invalid                       /*  NULL?, illegal post read/write: free(A_Post_ptr_invalid void *p);             */
#define A_Post_null                              /* ==NULL after the call                                                          */
#define A_Post_notnull                           /* !=NULL after the call                                                          */
#define A_Post_maybenull                         /*  NULL? after the call                                                          */
#define A_Pre_valid                              /* !=NULL,   legal pre read                                                       */
#define A_Pre_opt_valid                          /*  NULL?,   legal pre read                                                       */
#define A_Pre_invalid                            /*  NULL?, illegal pre read                                                       */
#define A_Pre_unknown                            /*  NULL? before the call                                                         */
#define A_Pre_notnull                            /* !=NULL before the call                                                         */
#define A_Pre_maybenull                          /*  NULL? before the call                                                         */
#define A_Pre_null                               /* ==NULL before the call                                                         */
#define A_Use_decl_annotations                   /* use annotations from declaration for function definition                       */
#define A_Pre                                    /* marks pre-condition                                                            */
#define A_Post                                   /* marks post-condition                                                           */
#define A_Null                                   /* ==NULL                                                                         */
#define A_Notnull                                /* !=NULL                                                                         */
#define A_Maybenull                              /*  NULL?                                                                         */
#define A_Valid                                  /*  NULL?   legal read                                                            */
#define A_Notvalid                               /*  NULL? illegal read                                                            */
#define A_Maybevalid                             /*  NULL?                                                                         */
#define A_Readable_bytes(n)                      /*  NULL? may read given number of bytes                                          */
#define A_Readable_elements(n)                   /*  NULL? may read given number of elements                                       */
#define A_Writable_bytes(n)                      /*  NULL? may write given number of bytes                                         */
#define A_Writable_elements(n)                   /*  NULL? may write given number of elements                                      */
#define A_Null_terminated                        /*  NULL? '\0'-terminated string                                                  */
#define A_Pre_readable_size(s)                   /*  NULL? may read  given size of elements before the call                        */
#define A_Pre_writable_size(s)                   /*  NULL? may write given size of elements before the call                        */
#define A_Pre_readable_byte_size(s)              /*  NULL? may read  given size of    bytes before the call                        */
#define A_Pre_writable_byte_size(s)              /*  NULL? may write given size of    bytes before the call                        */
#define A_Post_readable_size(s)                  /*  NULL? may read  given size of elements  after the call                        */
#define A_Post_writable_size(s)                  /*  NULL? may write given size of elements  after the call                        */
#define A_Post_readable_byte_size(s)             /*  NULL? may read  given size of    bytes  after the call                        */
#define A_Post_writable_byte_size(s)             /*  NULL? may write given size of    bytes  after the call                        */
#define A_Struct_size_bytes(s)                   /* structure resides in memory of given size                                      */
#define A_Field_size(s)                          /* !=NULL uninitialized field of s writable elements                              */
#define A_Field_size_opt(s)                      /*  NULL? uninitialized field of s writable elements                              */
#define A_Field_size_part(s,c)                   /* !=NULL partially initialized field of s writable elements, but only c readable */
#define A_Field_size_part_opt(s,c)               /*  NULL? partially initialized field of s writable elements, but only c readable */
#define A_Field_size_full(s)                     /* !=NULL fully initialized field of s r/w elements                               */
#define A_Field_size_full_opt(s)                 /*  NULL? fully initialized field of s r/w elements                               */
#define A_Field_size_bytes(s)                    /* !=NULL uninitialized field of s writable bytes                                 */
#define A_Field_size_bytes_opt(s)                /*  NULL? uninitialized field of s writable bytes                                 */
#define A_Field_size_bytes_part(s,c)             /* !=NULL partially initialized field of s writable bytes, but only c readable    */
#define A_Field_size_bytes_part_opt(s,c)         /*  NULL? partially initialized field of s writable bytes, but only c readable    */
#define A_Field_size_bytes_full(s)               /* !=NULL fully initialized field of s r/w bytes                                  */
#define A_Field_size_bytes_full_opt(s)           /*  NULL? fully initialized field of s r/w bytes                                  */
#define A_Field_z                                /*  NULL? '\0'-terminated field                                                   */
#define A_Field_range(min,max)                   /* field value is in given range                                                  */
#endif /* !_MSC_VER */

/* for typedefs */
#ifndef A_Ret_null_t
#define A_Ret_null_t A_Ret_null
#endif
#ifndef A_Ret_notnull_t
#define A_Ret_notnull_t A_Ret_notnull
#endif
#ifndef A_Ret_maybenull_t
#define A_Ret_maybenull_t A_Ret_maybenull
#endif
#ifndef A_Ret_z_t
#define A_Ret_z_t A_Ret_z
#endif
#ifndef A_Ret_maybenull_z_t
#define A_Ret_maybenull_z_t A_Ret_maybenull_z
#endif
#ifndef A_Ret_valid_t
#define A_Ret_valid_t A_Ret_valid
#endif
#ifndef A_Ret_opt_valid_t
#define A_Ret_opt_valid_t A_Ret_opt_valid
#endif
#ifndef A_Ret_writes_t
#define A_Ret_writes_t A_Ret_writes
#endif
#ifndef A_Ret_writes_z_t
#define A_Ret_writes_z_t A_Ret_writes_z
#endif
#ifndef A_Ret_writes_bytes_t
#define A_Ret_writes_bytes_t A_Ret_writes_bytes
#endif
#ifndef A_Ret_writes_maybenull_t
#define A_Ret_writes_maybenull_t A_Ret_writes_maybenull
#endif
#ifndef A_Ret_writes_maybenull_z_t
#define A_Ret_writes_maybenull_z_t A_Ret_writes_maybenull_z
#endif
#ifndef A_Ret_writes_bytes_maybenull_t
#define A_Ret_writes_bytes_maybenull_t A_Ret_writes_bytes_maybenull
#endif
#ifndef A_Ret_writes_to_t
#define A_Ret_writes_to_t A_Ret_writes_to
#endif
#ifndef A_Ret_writes_bytes_to_t
#define A_Ret_writes_bytes_to_t A_Ret_writes_bytes_to
#endif
#ifndef A_Ret_writes_to_maybenull_t
#define A_Ret_writes_to_maybenull_t A_Ret_writes_to_maybenull
#endif
#ifndef A_Ret_writes_bytes_to_maybenull_t
#define A_Ret_writes_bytes_to_maybenull_t A_Ret_writes_bytes_to_maybenull
#endif

#ifndef ASSUME
#if defined _MSC_VER
#define ASSUME(cond) __assume(!!(cond))
#elif defined __clang_analyzer__
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#elif defined __clang__
#define ASSUME(cond) __builtin_assume(!!(cond))
#elif defined __GNUC__ && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#else
#define ASSUME(cond) ((void)(cond)) /* assume condition is always true */
#endif
#endif /* ASSUME */

/*
 mark pointer as valid to avoid false analyzer warnings

 for example:

 struct A {
   struct B b;
   struct C c;
 };
 void B_destroy(A_Pre_valid A_Post_invalid struct B *b);
 void C_destroy(A_Pre_valid A_Post_invalid struct C *c);
 void A_destroy(A_Pre_valid A_Post_invalid struct A *a)
 {
   B_destroy(&a->b);
   C_destroy(&a->c); // here analyzer gives false warning C6001: Using uninitialized memory '*a'
 }

 to avoid analyzer warning, use A_Mark_ptr_valid():

 void A_destroy(A_Pre_valid A_Post_invalid struct A *a)
 {
   B_destroy(&a->b);
   A_Mark_ptr_valid(a);
   C_destroy(&a->c);
 }
*/
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:6101) /* Returning uninitialized memory '*a' */
#endif
A_Nonnull_all_args
static inline void A_Mark_ptr_valid(A_Pre_notnull A_Post_valid void *a)
{
	(void)a;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __NETBEANS_PREPROCESSING
#undef A_Force_inline_function
#define A_Force_inline_function
#endif

#endif /* SAL_DEFS_H_INCLUDED */
