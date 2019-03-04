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
/* NOTE: post-annotations are considered only if A_Success() condition is true or not specified */
#define A_Restrict                        __restrict
#define A_Noreturn_function               __declspec(noreturn)
#define A_Force_inline_function           __forceinline
#define A_Non_inline_function             __declspec(noinline)
#define A_Const_function                  /* gcc-specific */
#define A_Pure_function                   /* gcc-specific */
#define A_Empty                           /* empty, use to workaround bugs in analyzer */
#define A_Curr                            _Curr_
#define A_Param(i)                        _Param_(i)
#define A_Old                             _Old_
#define A_Const                           _Const_                           /* <- read                               */
#define A_In                              _In_                              /* <- !null valid (deref read)           */
#define A_In_opt                          _In_opt_                          /* <- null? valid (deref read)           */
#define A_In_z                            _In_z_                            /* <- !null valid (deref read) 0-term    */
#define A_In_opt_z                        _In_opt_z_                        /* <- null? valid (deref read) 0-term    */
#define A_Inout                           _Inout_                           /* <- !null valid        -> valid        */
#define A_Inout_opt                       _Inout_opt_                       /* <- null? valid        -> valid        */
#define A_Inout_z                         _Inout_z_                         /* <- !null 0-term valid -> valid 0-term */ _Pre_notnull_
#define A_Inout_opt_z                     _Inout_opt_z_                     /* <- null? 0-term valid -> valid 0-term */
#define A_Out                             _Out_                             /* <- !null w_e=1   -> valid             */
#define A_Out_opt                         _Out_opt_                         /* <- null? w_e=1   -> valid             */
#define A_Outptr                          _Outptr_                          /* <- !null w_e=1   -> valid (deref !null v_e=1)  */
#define A_Outptr_opt                      _Outptr_opt_                      /* <- null? w_e=1   -> valid (deref !null v_e=1)  */
#define A_Outptr_result_maybenull         _Outptr_result_maybenull_         /* <- !null w_e=1   -> valid (deref null? v_e=1)  */
#define A_Outptr_opt_result_maybenull     _Outptr_opt_result_maybenull_     /* <- null? w_e=1   -> valid (deref null? v_e=1)  */
#define A_Outptr_result_z                 _Outptr_result_z_                 /* <- !null w_e=1   -> valid (deref !null 0-term) */
#define A_Outptr_opt_result_z             _Outptr_opt_result_z_             /* <- null? w_e=1   -> valid (deref !null 0-term) */
#define A_Outptr_result_maybenull_z       _Outptr_result_maybenull_z_       /* <- !null w_e=1   -> valid (deref null? 0-term) */
#define A_Outptr_opt_result_maybenull_z   _Outptr_opt_result_maybenull_z_   /* <- null? w_e=1   -> valid (deref null? 0-term) */
#define A_In_reads(s)                     _In_reads_(s)                     /* <- !null valid v_e=s (deref read)              */
#define A_In_reads_opt(s)                 _In_reads_opt_(s)                 /* <- null? valid v_e=s (deref read)              */
#define A_In_reads_bytes(s)               _In_reads_bytes_(s)               /* <- !null valid v_b=s (deref read)              */
#define A_In_reads_bytes_opt(s)           _In_reads_bytes_opt_(s)           /* <- null? valid v_b=s (deref read)              */
#define A_In_reads_z(s)                   _In_reads_z_(s)                   /* <- !null valid v_e=s (deref read) 0-term       */
#define A_In_reads_opt_z(s)               _In_reads_opt_z_(s)               /* <- null? valid v_e=s (deref read) 0-term       */
#define A_In_reads_to_ptr(e)              _In_reads_to_ptr_(e)              /* <- !null valid v_e=pdiff(e) (deref read)       */
#define A_In_reads_to_ptr_opt(e)          _In_reads_to_ptr_opt_(e)          /* <- null? valid v_e=pdiff(e) (deref read)       */
#define A_Inout_updates(s)                _Inout_updates_(s)                /* <- !null valid w_e=s        -> valid           */
#define A_Inout_updates_opt(s)            _Inout_updates_opt_(s)            /* <- null? valid w_e=s        -> valid           */
#define A_Inout_updates_bytes(s)          _Inout_updates_bytes_(s)          /* <- !null valid w_b=s        -> valid           */
#define A_Inout_updates_bytes_opt(s)      _Inout_updates_bytes_opt_(s)      /* <- null? valid w_b=s        -> valid           */
#define A_Inout_updates_z(s)              _Inout_updates_z_(s)              /* <- !null valid w_e=s 0-term -> valid 0-term    */
#define A_Inout_updates_opt_z(s)          _Inout_updates_opt_z_(s)          /* <- null? valid w_e=s 0-term -> valid 0-term    */
#define A_Inout_updates_to(s,c)           _Inout_updates_to_(s,c)           /* <- !null valid v_e=s w_e=s  -> valid v_e=c     */
#define A_Inout_updates_to_opt(s,c)       _Inout_updates_to_opt_(s,c)       /* <- null? valid v_e=s w_e=s  -> valid v_e=c     */
#define A_Inout_updates_bytes_to(s,c)     _Inout_updates_bytes_to_(s,c)     /* <- !null valid v_b=s w_b=s  -> valid v_b=c     */
#define A_Inout_updates_bytes_to_opt(s,c) _Inout_updates_bytes_to_opt_(s,c) /* <- null? valid v_b=s w_b=s  -> valid v_b=c     */
#define A_Inout_updates_all(s)            _Inout_updates_all_(s)            /* <- !null valid v_e=old(s) w_e=old(s) -> valid v_e=old(s) */
#define A_Inout_updates_all_opt(s)        _Inout_updates_all_opt_(s)        /* <- null? valid v_e=old(s) w_e=old(s) -> valid v_e=old(s) */
#define A_Inout_updates_bytes_all(s)      _Inout_updates_bytes_all_(s)      /* <- !null valid v_b=old(s) w_b=old(s) -> valid v_b=old(s) */
#define A_Inout_updates_bytes_all_opt(s)  _Inout_updates_bytes_all_opt_(s)  /* <- null? valid v_b=old(s) w_b=old(s) -> valid v_b=old(s) */
#define A_Out_writes(s)                   _Out_writes_(s)                   /* <- !null w_e=s        -> valid            */
#define A_Out_writes_opt(s)               _Out_writes_opt_(s)               /* <- null? w_e=s        -> valid            */
#define A_Out_writes_bytes(s)             _Out_writes_bytes_(s)             /* <- !null w_b=s        -> valid            */
#define A_Out_writes_bytes_opt(s)         _Out_writes_bytes_opt_(s)         /* <- null? w_b=s        -> valid            */
#define A_Out_writes_z(s)                 _Out_writes_z_(s)                 /* <- !null w_e=s        -> valid 0-term     */
#define A_Out_writes_opt_z(s)             _Out_writes_opt_z_(s)             /* <- null? w_e=s        -> valid 0-term     */
#define A_Out_writes_to_ptr(e)            _Out_writes_to_ptr_(e)            /* <- !null w_e=pdiff(e) -> valid            */
#define A_Out_writes_to_ptr_opt(e)        _Out_writes_to_ptr_opt_(e)        /* <- null? w_e=pdiff(e) -> valid            */
#define A_Out_writes_to_ptr_z(e)          _Out_writes_to_ptr_z_(e)          /* <- !null w_e=pdiff(e) -> valid 0-term     */ _Post_z_
#define A_Out_writes_to_ptr_opt_z(e)      _Out_writes_to_ptr_opt_z_(e)      /* <- null? w_e=pdiff(e) -> valid 0-term     */ _Post_z_
#define A_Out_writes_to(s,c)              _Out_writes_to_(s,c)              /* <- !null w_e=s        -> valid v_e=c      */
#define A_Out_writes_to_opt(s,c)          _Out_writes_to_opt_(s,c)          /* <- null? w_e=s        -> valid v_e=c      */
#define A_Out_writes_bytes_to(s,c)        _Out_writes_bytes_to_(s,c)        /* <- !null w_b=s        -> valid v_b=s      */
#define A_Out_writes_bytes_to_opt(s,c)    _Out_writes_bytes_to_opt_(s,c)    /* <- null? w_b=s        -> valid v_b=s      */
#define A_Out_writes_all(s)               _Out_writes_all_(s)               /* <- !null w_e=old(s)   -> valid v_e=old(s) */
#define A_Out_writes_all_opt(s)           _Out_writes_all_opt_(s)           /* <- null? w_e=old(s)   -> valid v_e=old(s) */
#define A_Out_writes_bytes_all(s)         _Out_writes_bytes_all_(s)         /* <- !null w_b=old(s)   -> valid v_b=old(s) */
#define A_Out_writes_bytes_all_opt(s)     _Out_writes_bytes_all_opt_(s)     /* <- null? w_b=old(s)   -> valid v_b=old(s) */
#define A_Ret_null                        _Ret_null_                        /* -> null                     */
#define A_Ret_notnull                     _Ret_notnull_                     /* -> !null                    */
#define A_Ret_maybenull                   _Ret_maybenull_                   /* -> null?                    */
#define A_Ret_valid                       _Ret_valid_                       /* -> !null valid              */
#define A_Ret_opt_valid                   _Ret_opt_valid_                   /* -> null? valid              */
#define A_Ret_z                           _Ret_z_                           /* -> !null valid 0-term       */
#define A_Ret_maybenull_z                 _Ret_maybenull_z_                 /* -> null? valid 0-term       */
#define A_Ret_writes_z(s)                 _Ret_writes_z_(s)                 /* -> !null valid v_e=s 0-term */
#define A_Ret_writes_maybenull_z(s)       _Ret_writes_maybenull_z_(s)       /* -> null? valid v_e=s 0-term */
#define A_Ret_writes(s)                   _Ret_writes_(s)                   /* -> !null valid v_e=s        */
#define A_Ret_writes_maybenull(s)         _Ret_writes_maybenull_(s)         /* -> null? valid v_e=s        */
#define A_Ret_writes_bytes(s)             _Ret_writes_bytes_(s)             /* -> !null valid v_b=s        */
#define A_Ret_writes_bytes_maybenull(s)   _Ret_writes_bytes_maybenull_(s)   /* -> null? valid v_b=s        */
#define A_Ret_writes_to(s,c)                 _Ret_writes_to_(s,c)                 /* -> !null valid w_e=s v_e=c */
#define A_Ret_writes_to_maybenull(s,c)       _Ret_writes_to_maybenull_(s,c)       /* -> null? valid w_e=s v_e=c */
#define A_Ret_writes_bytes_to(s,c)           _Ret_writes_bytes_to_(s,c)           /* -> !null valid w_b=s v_b=c */
#define A_Ret_writes_bytes_to_maybenull(s,c) _Ret_writes_bytes_to_maybenull_(s,c) /* -> null? valid w_b=s v_b=c */
#define A_Check_return                    _Check_return_                    /* -> check */
#define A_Must_inspect_result             _Must_inspect_result_             /* -> check */
#define A_String_length(p)                _Inexpressible_(p)                /* as of MSVC v15.9.5: _String_length_("x") == 0 (sic!) */
#define A_Pre_ptrdiff_count(e)            _Pre_ptrdiff_count_(e)            /* <- !null valid pdiff(e)   */
#define A_Pre_opt_ptrdiff_count(e)        _Pre_opt_ptrdiff_count_(e)        /* <- null? valid pdiff(e)   */
#define A_In_range(l,h)                   _In_range_(l,h)                   /* <-       l <= param  <= h */
#define A_Out_range(l,h)                  _Out_range_(l,h)                  /* ->       l <= param  <= h */
#define A_Ret_range(l,h)                  _Ret_range_(l,h)                  /* ->       l <= return <= h */
#define A_Deref_in_range(l,h)             _Deref_in_range_(l,h)             /* <- deref l <= param  <= h  (null-ness?) */
#define A_Deref_out_range(l,h)            _Deref_out_range_(l,h)            /* -> deref l <= param  <= h  (null-ness?) */
#define A_Deref_ret_range(l,h)            _Deref_ret_range_(l,h)            /* -> deref l <= return <= h  (null-ness?) */
#define A_Pre_equal_to(v)                 _Pre_equal_to_(v)                 /* <- param == v */
#define A_Post_equal_to(v)                _Post_equal_to_(v)                /* -> param == v */
#define A_Unchanged(p)                    _Unchanged_(p)                    /* <- read -> p == old(p) */
#define A_Pre_satisfies(cond)             _Pre_satisfies_(cond)             /* <- cond != 0 */
#define A_Post_satisfies(cond)            _Post_satisfies_(cond)            /* -> cond != 0 */
#define A_Success(expr)                   _Success_(expr)                   /* -> expr != 0 */
#define A_Return_type_success(expr)       _Return_type_success_(expr)       /* -> expr != 0 */
#define A_Result_zeroonfailure            _Result_zeroonfailure_            /* (failed -> deref == 0) (null-ness?) */
#define A_Result_nullonfailure            _Result_nullonfailure_            /* (failed -> deref null) (null-ness?) */
#define A_Outptr_result_nullonfailure     _Outptr_result_nullonfailure_     /* _Outptr_     + _Result_nullonfailure_ */
#define A_Outptr_opt_result_nullonfailure _Outptr_opt_result_nullonfailure_ /* _Outptr_opt_ + _Result_nullonfailure_ */
#define A_Outptr_result_buffer(s)                    _Outptr_result_buffer_(s)                    /* <- !null w_e=1 -> valid (deref !null w_e=s)       */
#define A_Outptr_result_buffer_maybenull(s)          _Outptr_result_buffer_maybenull_(s)          /* <- !null w_e=1 -> valid (deref null? w_e=s)       */
#define A_Outptr_opt_result_buffer(s)                _Outptr_opt_result_buffer_(s)                /* <- null? w_e=1 -> valid (deref !null w_e=s)       */
#define A_Outptr_opt_result_buffer_maybenull(s)      _Outptr_opt_result_buffer_maybenull_(s)      /* <- null? w_e=1 -> valid (deref null? w_e=s)       */
#define A_Outptr_result_buffer_to(s,c)               _Outptr_result_buffer_to_(s,c)               /* <- !null w_e=1 -> valid (deref !null w_e=s v_e=c) */
#define A_Outptr_result_buffer_to_maybenull(s,c)     _Outptr_result_buffer_to_maybenull_(s,c)     /* <- !null w_e=1 -> valid (deref null? w_e=s v_e=c) */
#define A_Outptr_opt_result_buffer_to(s,c)           _Outptr_opt_result_buffer_to_(s,c)           /* <- null? w_e=1 -> valid (deref !null w_e=s v_e=c) */
#define A_Outptr_opt_result_buffer_to_maybenull(s,c) _Outptr_opt_result_buffer_to_maybenull_(s,c) /* <- null? w_e=1 -> valid (deref null? w_e=s v_e=c) */
#define A_Outptr_result_buffer_all(s)                _Outptr_result_buffer_all_(s)                /* <- !null w_e=1 -> valid (deref !null v_e=s)       */
#define A_Outptr_result_buffer_all_maybenull(s)      _Outptr_result_buffer_all_maybenull_(s)      /* <- !null w_e=1 -> valid (deref null? v_e=s)       */
#define A_Outptr_opt_result_buffer_all(s)            _Outptr_opt_result_buffer_all_(s)            /* <- null? w_e=1 -> valid (deref !null v_e=s)       */
#define A_Outptr_opt_result_buffer_all_maybenull(s)  _Outptr_opt_result_buffer_all_maybenull_(s)  /* <- null? w_e=1 -> valid (deref null? v_e=s)       */
#define A_Outptr_result_bytebuffer(s)                    _Outptr_result_bytebuffer_(s)                    /* <- !null w_e=1 -> valid (deref !null w_b=s)       */
#define A_Outptr_result_bytebuffer_maybenull(s)          _Outptr_result_bytebuffer_maybenull_(s)          /* <- !null w_e=1 -> valid (deref null? w_b=s)       */
#define A_Outptr_opt_result_bytebuffer(s)                _Outptr_opt_result_bytebuffer_(s)                /* <- null? w_e=1 -> valid (deref !null w_b=s)       */
#define A_Outptr_opt_result_bytebuffer_maybenull(s)      _Outptr_opt_result_bytebuffer_maybenull_(s)      /* <- null? w_e=1 -> valid (deref null? w_b=s)       */
#define A_Outptr_result_bytebuffer_to(s,c)               _Outptr_result_bytebuffer_to_(s,c)               /* <- !null w_e=1 -> valid (deref !null w_b=s v_b=c) */
#define A_Outptr_result_bytebuffer_to_maybenull(s,c)     _Outptr_result_bytebuffer_to_maybenull_(s,c)     /* <- !null w_e=1 -> valid (deref null? w_b=s v_b=c) */
#define A_Outptr_opt_result_bytebuffer_to(s,c)           _Outptr_opt_result_bytebuffer_to_(s,c)           /* <- null? w_e=1 -> valid (deref !null w_b=s v_b=c) */
#define A_Outptr_opt_result_bytebuffer_to_maybenull(s,c) _Outptr_opt_result_bytebuffer_to_maybenull_(s,c) /* <- null? w_e=1 -> valid (deref null? w_b=s v_b=c) */
#define A_Outptr_result_bytebuffer_all(s)                _Outptr_result_bytebuffer_all_(s)                /* <- !null w_e=1 -> valid (deref !null v_b=s)       */
#define A_Outptr_result_bytebuffer_all_maybenull(s)      _Outptr_result_bytebuffer_all_maybenull_(s)      /* <- !null w_e=1 -> valid (deref null? v_b=s)       */
#define A_Outptr_opt_result_bytebuffer_all(s)            _Outptr_opt_result_bytebuffer_all_(s)            /* <- null? w_e=1 -> valid (deref !null v_b=s)       */
#define A_Outptr_opt_result_bytebuffer_all_maybenull(s)  _Outptr_opt_result_bytebuffer_all_maybenull_(s)  /* <- null? w_e=1 -> valid (deref null? v_b=s)       */
#define A_Printf_format_string           _Printf_format_string_           /* <->                 (null-ness?, validity?)  */
#define e_A_Group(anns,empty)            _Group_(anns##empty)
#define A_Group(anns)                    e_A_Group(anns,A_Empty)
#define e_A_On_failure(anns,empty)       _On_failure_(anns##empty)
#define A_On_failure(anns)               e_A_On_failure(anns,A_Empty)
#define e_A_Always(anns,empty)           _Always_(anns##empty)
#define A_Always(anns)                   e_A_Always(anns,A_Empty)
#define A_At(at,anns)                    _At_(at,anns)
#define A_At_buffer(buf,i,cnt,anns)      _At_buffer_(buf,i,cnt,anns)      /* i - iterator name to use in anns (null-ness?, validity?) */
#define A_When(cond,anns)                _When_(cond,anns)
#define A_Pre                            _Pre_                            /* <-                  (null-ness?, validity?)           */
#define A_Post                           _Post_                           /*  ->                 (null-ness?, validity?)           */
#define A_Pre_z                          _Pre_z_                          /* <-  valid 0-term    (null-ness?)                      */
#define A_Post_z                         _Post_z_                         /*  -> valid 0-term    (null-ness?)                      */
#define A_Prepost_z                      _Prepost_z_                      /* <-  valid 0-term -> valid 0-term (null-ness?)         */
#define A_Post_maybez                    _Post_maybez_                    /*  -> 0-term?         (null-ness?, validity?)           */
#define A_Pre_null                       _Pre_null_                       /* <-  null                                              */
#define A_Post_null                      _Post_null_                      /*  -> null                                              */
#define A_Null                           _Null_                           /* <-> null                                              */
#define A_Pre_notnull                    _Pre_notnull_                    /* <-  !null           (validity?)                       */
#define A_Post_notnull                   _Post_notnull_                   /*  -> !null           (validity?)                       */
#define A_Notnull                        _Notnull_                        /* <-> !null           (validity?)                       */
#define A_Pre_maybenull                  _Pre_maybenull_                  /* <-  null?           (validity?)                       */
#define A_Post_maybenull                 _Post_maybenull_                 /*  -> null?           (validity?)                       */
#define A_Maybenull                      _Maybenull_                      /* <-> null?           (validity?)                       */
#define A_Pre_valid                      _Pre_valid_                      /* <-  !null valid                                       */
#define A_Pre_opt_valid                  _Pre_opt_valid_                  /* <-  null? valid                                       */
#define A_Post_valid                     _Post_valid_                     /*  -> valid           (null-ness?)                      */
#define A_Valid                          _Valid_                          /* <-> valid           (null-ness?)                      */
#define A_Pre_deref_invalid              _Pre_invalid_                    /* <-  deref invalid   (null-ness?)                      */
#define A_Post_deref_invalid             _Post_invalid_                   /*  -> deref invalid   (null-ness?)                      */
#define A_Deref_invalid                  _Pre_invalid_ _Post_invalid_     /* <-> deref invalid   (null-ness?)                      */
#define A_Notvalid                       _Notvalid_                       /* <-> invalid         (null-ness?)                      */
#define A_Post_ptr_invalid               _Post_ptr_invalid_               /*  -> invalid         (null-ness?)                      */
#define A_Pre_unknown                    _Pre_unknown_                    /* <-  valid?          (null-ness?)                      */
#define A_Maybevalid                     _Maybevalid_                     /* <-> valid?          (null-ness?)                      */
#define A_Use_decl_annotations           _Use_decl_annotations_
#define A_Readable_bytes(n)              _Readable_bytes_(n)              /* <-> r_b=n           (null-ness?, validity?)           */
#define A_Writable_bytes(n)              _Writable_bytes_(n)              /* <-> w_b=n           (null-ness?, validity?)           */
#define A_Readable_elements(n)           _Readable_elements_(n)           /* <-> r_e=n           (null-ness?, validity?)           */
#define A_Writable_elements(n)           _Writable_elements_(n)           /* <-> w_e=n           (null-ness?, validity?)           */
#define A_Null_terminated                _Null_terminated_                /* <-> 0-term          (null-ness?, validity?)           */
#define A_Pre_readable_size(s)           _Pre_readable_size_(s)           /* <-  valid v_e=s     (null-ness?)                      */
#define A_Post_readable_size(s)          _Post_readable_size_(s)          /*  -> valid v_e=s     (null-ness?)                      */
#define A_Pre_writable_size(s)           _Pre_writable_size_(s)           /* <-  w_e=s           (null-ness?, validity?)           */
#define A_Post_writable_size(s)          _Post_writable_size_(s)          /*  -> w_e=s           (null-ness?, validity?)           */
#define A_Pre_readable_byte_size(s)      _Pre_readable_byte_size_(s)      /* <-  valid v_b=s     (null-ness?)                      */
#define A_Post_readable_byte_size(s)     _Post_readable_byte_size_(s)     /*  -> valid v_b=s     (null-ness?)                      */
#define A_Pre_writable_byte_size(s)      _Pre_writable_byte_size_(s)      /* <-  w_b=s           (null-ness?, validity?)           */
#define A_Post_writable_byte_size(s)     _Post_writable_byte_size_(s)     /*  -> w_b=s           (null-ness?, validity?)           */
#define A_Struct_size_bytes(s)           _Struct_size_bytes_(s)           /* <-> w_b=s           (null-ness?, validity?)           */
#define A_Field_size(s)                  _Field_size_(s)                  /* <-> !null w_e=s       (validity?)                     */
#define A_Field_size_opt(s)              _Field_size_opt_(s)              /* <-> null? w_e=s       (validity?)                     */
#define A_Field_size_part(s,c)           _Field_size_part_(s,c)           /* <-> !null w_e=s r_e=c (validity?)                     */
#define A_Field_size_part_opt(s,c)       _Field_size_part_opt_(s,c)       /* <-> null? w_e=s r_e=c (validity?)                     */
#define A_Field_size_full(s)             _Field_size_full_(s)             /* <-> !null w_e=s r_e=s (validity?)                     */
#define A_Field_size_full_opt(s)         _Field_size_full_opt_(s)         /* <-> null? w_e=s r_e=s (validity?)                     */
#define A_Field_size_bytes(s)            _Field_size_bytes_(s)            /* <-> !null w_b=s       (validity?)                     */
#define A_Field_size_bytes_opt(s)        _Field_size_bytes_opt_(s)        /* <-> null? w_b=s       (validity?)                     */
#define A_Field_size_bytes_part(s,c)     _Field_size_bytes_part_(s,c)     /* <-> !null w_b=s r_b=c (validity?)                     */
#define A_Field_size_bytes_part_opt(s,c) _Field_size_bytes_part_opt_(s,c) /* <-> null? w_b=s r_b=c (validity?)                     */
#define A_Field_size_bytes_full(s)       _Field_size_bytes_full_(s)       /* <-> !null w_b=s r_b=c (validity?)                     */
#define A_Field_size_bytes_full_opt(s)   _Field_size_bytes_full_opt_(s)   /* <-> null? w_b=s r_b=c (validity?)                     */
#define A_Field_z                        _Field_z_                        /* <-> 0-term            (null-ness?, validity?)         */
#define A_Field_range(l,h)               _Field_range_(l,h)               /* <-> l <= field <= h                                   */
#define A_Ret_restrict                   __declspec(restrict) /* auto-mark pointer that accepts return value as A_Restrict         */
#define A_Ret_malloc                     A_Ret_restrict
#define A_Ret_never_null                 _Success_(1) _Always_(_Ret_notnull_) /* for a custom A_Success() use A_Ret_never_null_but */
#define A_Ret_never_null_but             _Always_(_Ret_notnull_)          /* for a custom expression of A_Success(expr)            */
#define A_Alloc_size(i)                  /* gcc-specific */
#define A_Nonnull_all_args               /* gcc-specific */
#define A_Nonnull_arg(i)                 _At_(_Param_(i), _Notnull_)
#define A_Printf_format_at(f,v)          _At_(_Param_(f), _Printf_format_string_)

#else /* !_MSC_VER || NO_SAL_ANNOTATIONS */

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define A_Restrict restrict                      /* standard keyword for c99 */
#elif defined _MSC_VER
#define A_Restrict                               __restrict
#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Restrict                               __restrict__
#else /* no GCC extensions */
#define A_Restrict
#endif /* no GCC extensions */

#ifdef _MSC_VER
#define A_Noreturn_function                      __declspec(noreturn)
#define A_Force_inline_function                  __forceinline
#define A_Non_inline_function                    __declspec(noinline)
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Noreturn_function                      __attribute__ ((noreturn))
#define A_Force_inline_function                  __attribute__ ((always_inline)) inline
#define A_Non_inline_function                    __attribute__ ((noinline))
#else /* no GCC extensions */
#define A_Noreturn_function                      /* function which never returns - calls exit()                                    */
#define A_Force_inline_function inline           /* forcibly inlined function                                                      */
#define A_Non_inline_function                    /* forbid inlining a function                                                     */
#endif /* no GCC extensions */

#if (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Const_function                         __attribute__ ((const))
#define A_Pure_function                          __attribute__ ((pure))
#define A_Check_return                           __attribute__ ((warn_unused_result))
#define A_Nonnull_all_args                       __attribute__ ((nonnull))
#define A_Nonnull_arg(i)                         __attribute__ ((nonnull(i,i)))
#define A_Printf_format_at(f,v)                  __attribute__ ((format(printf, f, v)))
#else /* no GCC extensions */
#define A_Const_function                         /* declare function without side effects, cannot access any memory by pointer     */
#define A_Pure_function                          /* declare function without side effects, may read but cannot write memory        */
#define A_Check_return                           /* caller must check function's return value                                      */
#define A_Nonnull_all_args                       /* all function arguments pointers are != NULL                                    */
#define A_Nonnull_arg(i)                         /* function argument number i is != NULL                                          */
#define A_Printf_format_at(f,v)                  /* f - 1-based index of printf format argument, v - index of va_arg argument      */
#endif /* no GCC extensions */

#ifdef _MSC_VER
#define A_Ret_restrict                           __declspec(restrict) /* auto-mark pointer that accepts return value as A_Restrict */
#else /* !_MSC_VER */
#define A_Ret_restrict                           /* function returns restricted pointer - assign it to A_Restrict variable         */
#endif /* !_MSC_VER */

#ifdef _MSC_VER
#define A_Ret_malloc                             A_Ret_restrict
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Ret_malloc                             __attribute__ ((malloc))
#else /* no GCC extensions */
#define A_Ret_malloc                             /* like A_Ret_restrict, but also returned memory do not contains valid pointers   */
#endif /* no GCC extensions */

#if (defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 9))) || \
  (defined(__clang__) && (__clang_major__ > 3 || (3 == __clang_major__  && __clang_minor__ >= 7)))
#define A_Ret_never_null                         __attribute__ ((returns_nonnull))
#define A_Ret_never_null_but                     __attribute__ ((returns_nonnull))
#else /* no GCC extensions */
#define A_Ret_never_null                         /* never returns NULL (if there is no A_Success() annotation)                     */
#define A_Ret_never_null_but                     /* never returns NULL, even if expr of A_Success(expr) is 0                       */
#endif /* no GCC extensions */

#if (defined(__GNUC__) && (__GNUC__ > 4 || (4 == __GNUC__ && __GNUC_MINOR__ >= 3))) || \
  (defined(__clang__) && (__clang_major__ >= 4))
#define A_Alloc_size(i)                          __attribute__ ((alloc_size(i)))
#else /* no GCC extensions */
#define A_Alloc_size(i)                          /* function argument number i is the size of allocated memory                     */
#endif /* no GCC extensions */

#define A_Curr                                   /* references current function parameter, like A_At(*A_Curr, A_Post_notnull)      */
#define A_Param(i)                               /* references function parameter number i, like A_At(A_Param(1), A_Notnull)       */
#define A_Old                                    /* old value, like A_Old(*A_Curr)                                                 */
#define A_Const                                  /* structure member is read-only: A_At(s->m, A_Const)                             */
#define A_In                                     /* !NULL ptr to read-only valid object                                            */
#define A_In_opt                                 /* NULL? ptr to read-only valid object                                            */
#define A_In_z                                   /* !NULL ptr to read-only valid 0-term string                                     */
#define A_In_opt_z                               /* NULL? ptr to read-only valid 0-term string                                     */
#define A_Inout                                  /* !NULL ptr to r/w pre/post valid object                                         */
#define A_Inout_opt                              /* NULL? ptr to r/w pre/post valid object                                         */
#define A_Inout_z                                /* !NULL ptr to r/w pre/post valid 0-term string                                  */
#define A_Inout_opt_z                            /* NULL? ptr to r/w pre/post valid 0-term string                                  */
#define A_Out                                    /* !NULL ptr to post-valid object                                                 */
#define A_Out_opt                                /* NULL? ptr to post-valid object                                                 */
#define A_Outptr                                 /* !NULL ptr to post-!NULL ptr to valid object                                    */
#define A_Outptr_opt                             /* NULL? ptr to post-!NULL ptr to valid object                                    */
#define A_Outptr_result_maybenull                /* !NULL ptr to post-NULL? ptr to valid object                                    */
#define A_Outptr_opt_result_maybenull            /* NULL? ptr to post-NULL? ptr to valid object                                    */
#define A_Outptr_result_z                        /* !NULL ptr to post-!NULL ptr to valid 0-term string                             */
#define A_Outptr_opt_result_z                    /* NULL? ptr to post-!NULL ptr to valid 0-term string                             */
#define A_Outptr_result_maybenull_z              /* !NULL ptr to post-NULL? ptr to valid 0-term string                             */
#define A_Outptr_opt_result_maybenull_z          /* NULL? ptr to post-NULL? ptr to valid 0-term string                             */
#define A_In_reads(s)                            /* !NULL ptr to read-only array of s valid elems                                  */
#define A_In_reads_opt(s)                        /* NULL? ptr to read-only array of s valid elems                                  */
#define A_In_reads_bytes(s)                      /* !NULL ptr to read-only memory of s valid bytes                                 */
#define A_In_reads_bytes_opt(s)                  /* NULL? ptr to read-only memory of s valid bytes                                 */
#define A_In_reads_z(s)                          /* !NULL ptr to read-only 0-term array of s valid characters                      */
#define A_In_reads_opt_z(s)                      /* NULL? ptr to read-only 0-term array of s valid characters                      */
#define A_In_reads_to_ptr(e)                     /* !NULL ptr to read-only array of valid elems until ptr e                        */
#define A_In_reads_to_ptr_opt(e)                 /* NULL? ptr to read-only array of valid elems until ptr e                        */
#define A_Inout_updates(s)                       /* !NULL ptr to pre/post valid array of s writable elems                          */
#define A_Inout_updates_opt(s)                   /* NULL? ptr to pre/post valid array of s writable elems                          */
#define A_Inout_updates_bytes(s)                 /* !NULL ptr to pre/post valid array of s writable bytes                          */
#define A_Inout_updates_bytes_opt(s)             /* NULL? ptr to pre/post valid array of s writable bytes                          */
#define A_Inout_updates_z(s)                     /* !NULL ptr to pre/post valid pre/post 0-term array of s writable chars          */
#define A_Inout_updates_opt_z(s)                 /* NULL? ptr to pre/post valid pre/post 0-term array of s writable chars          */
#define A_Inout_updates_to(s,c)                  /* !NULL ptr to array of s pre r/w valid elems, c elems are post-valid            */
#define A_Inout_updates_to_opt(s,c)              /* NULL? ptr to array of s pre r/w valid elems, c elems are post-valid            */
#define A_Inout_updates_bytes_to(s,c)            /* !NULL ptr to array of s pre r/w valid bytes, c bytes are post-valid            */
#define A_Inout_updates_bytes_to_opt(s,c)        /* NULL? ptr to array of s pre r/w valid bytes, c bytes are post-valid            */
#define A_Inout_updates_all(s)                   /* !NULL ptr to array of s pre r/w valid elems, old(s) elems are post-valid       */
#define A_Inout_updates_all_opt(s)               /* NULL? ptr to array of s pre r/w valid elems, old(s) elems are post-valid       */
#define A_Inout_updates_bytes_all(s)             /* !NULL ptr to array of s pre r/w valid bytes, old(s) bytes are post-valid       */
#define A_Inout_updates_bytes_all_opt(s)         /* NULL? ptr to array of s pre r/w valid bytes, old(s) bytes are post-valid       */
#define A_Out_writes(s)                          /* !NULL ptr to post-valid array of s writable elems                              */
#define A_Out_writes_opt(s)                      /* NULL? ptr to post-valid array of s writable elems                              */
#define A_Out_writes_bytes(s)                    /* !NULL ptr to post-valid array of s writable bytes                              */
#define A_Out_writes_bytes_opt(s)                /* NULL? ptr to post-valid array of s writable bytes                              */
#define A_Out_writes_z(s)                        /* !NULL ptr to post-valid post 0-term array of s writable chars                  */
#define A_Out_writes_opt_z(s)                    /* NULL? ptr to post-valid post 0-term array of s writable chars                  */
#define A_Out_writes_to_ptr(e)                   /* !NULL ptr to post-valid array of writable elems until ptr e                    */
#define A_Out_writes_to_ptr_opt(e)               /* NULL? ptr to post-valid array of writable elems until ptr e                    */
#define A_Out_writes_to_ptr_z(e)                 /* !NULL ptr to post-valid post 0-term array of writable chars until ptr e        */
#define A_Out_writes_to_ptr_opt_z(e)             /* NULL? ptr to post-valid post 0-term array of writable chars until ptr e        */
#define A_Out_writes_to(s,c)                     /* !NULL ptr to array of s writable elems, c elems are post-valid                 */
#define A_Out_writes_to_opt(s,c)                 /* NULL? ptr to array of s writable elems, c elems are post-valid                 */
#define A_Out_writes_bytes_to(s,c)               /* !NULL ptr to array of s writable bytes, c bytes are post-valid                 */
#define A_Out_writes_bytes_to_opt(s,c)           /* NULL? ptr to array of s writable bytes, c bytes are post-valid                 */
#define A_Out_writes_all(s)                      /* !NULL ptr to array of s writable elems, old(s) elems are post-valid            */
#define A_Out_writes_all_opt(s)                  /* NULL? ptr to array of s writable elems, old(s) elems are post-valid            */
#define A_Out_writes_bytes_all(s)                /* !NULL ptr to array of s writable bytes, old(s) bytes are post-valid            */
#define A_Out_writes_bytes_all_opt(s)            /* NULL? ptr to array of s writable bytes, old(s) bytes are post-valid            */
#define A_Ret_null                               /* returns NULL ptr                                                               */
#define A_Ret_notnull                            /* returns !NULL ptr                                                              */
#define A_Ret_maybenull                          /* returns NULL? ptr                                                              */
#define A_Ret_valid                              /* returns !NULL ptr to (recursively) valid object                                */
#define A_Ret_opt_valid                          /* returns NULL? ptr to (recursively) valid object                                */
#define A_Ret_z                                  /* returns !NULL ptr to valid 0-term string                                       */
#define A_Ret_maybenull_z                        /* returns NULL? ptr to valid 0-term string                                       */
#define A_Ret_writes_z(s)                        /* returns !NULL ptr to array of s valid 0-term chars                             */
#define A_Ret_writes_maybenull_z(s)              /* returns NULL? ptr to array of s valid 0-term chars                             */
#define A_Ret_writes(s)                          /* returns !NULL ptr to array of s valid elems                                    */
#define A_Ret_writes_maybenull(s)                /* returns NULL? ptr to array of s valid elems                                    */
#define A_Ret_writes_bytes(s)                    /* returns !NULL ptr to array of s valid bytes                                    */
#define A_Ret_writes_bytes_maybenull(s)          /* returns NULL? ptr to array of s valid bytes                                    */
#define A_Ret_writes_to(s,c)                     /* returns !NULL ptr to array of s writable elems, c elems are valid              */
#define A_Ret_writes_to_maybenull(s,c)           /* returns NULL? ptr to array of s writable elems, c elems are valid              */
#define A_Ret_writes_bytes_to(s,c)               /* returns !NULL ptr to array of s writable bytes, c bytes are valid              */
#define A_Ret_writes_bytes_to_maybenull(s,c)     /* returns NULL? ptr to array of s writable bytes, c bytes are valid              */
#define A_Must_inspect_result                    /* caller must check value of out-parameter (*ptr) (NULL-ness?, validity?)        */
#define A_String_length(p)                       /* length of C-string passed in parameter p, i.e. strlen(p)/wsclen(p)             */
#define A_Pre_ptrdiff_count(e)                   /* before the call: number of elements in array until ptr e                       */
#define A_Pre_opt_ptrdiff_count(e)               /* before the call: number of elements in optional array until ptr e              */
#define A_In_range(l,h)                          /* value is in given range (inclusive) before the call                            */
#define A_Out_range(l,h)                         /* value is in given range (inclusive) after the call, usable for C++ references  */
#define A_Ret_range(l,h)                         /* return value is in given range (inclusive)                                     */
#define A_Deref_in_range(l,h)                    /* pointed value is in given range (inclusive) before the call (NULL-ness?)       */
#define A_Deref_out_range(l,h)                   /* pointed value is in given range (inclusive) after the call  (NULL-ness?)       */
#define A_Deref_ret_range(l,h)                   /* returns pointer to value in given range (inclusive)         (NULL-ness?)       */
#define A_Pre_equal_to(v)                        /* shortcut to A_In_range(=,v)                                                    */
#define A_Post_equal_to(v)                       /* shortcut to A_Out_range(=,v), usable for C++ references                        */
#define A_Unchanged(v)                           /* value is not changed by a function: A_At(v, A_Const A_Post_equal_to(A_Old(v))) */
#define A_Pre_satisfies(cond)                    /* custom pre-condition                                                           */
#define A_Post_satisfies(cond)                   /* custom post-condition                                                          */
#define A_Success(expr)                          /* successful result condition -> check post-conditions if expr is non-zero       */
#define A_Return_type_success(expr)              /* for typedefs: typedef A_Return_type_success(return >= 0) int ret_t             */
#define A_Result_zeroonfailure                   /* on failure: (*ptr) == 0    (NULL-ness?)                                        */
#define A_Result_nullonfailure                   /* on failure: (*ptr) == NULL (NULL-ness?)                                        */
#define A_Outptr_result_nullonfailure            /* on success: A_Outptr,     on failure: (*ptr) == NULL                           */
#define A_Outptr_opt_result_nullonfailure        /* on success: A_Outptr_opt, on failure: (*ptr) == NULL                           */
#define A_Outptr_result_buffer(s)                    /* !NULL ptr to post-!NULL ptr to post-valid array of s writable elems              */
#define A_Outptr_result_buffer_maybenull(s)          /* !NULL ptr to post-NULL? ptr to post-valid array of s writable elems              */
#define A_Outptr_opt_result_buffer(s)                /* NULL? ptr to post-!NULL ptr to post-valid array of s writable elems              */
#define A_Outptr_opt_result_buffer_maybenull(s)      /* NULL? ptr to post-NULL? ptr to post-valid array of s writable elems              */
#define A_Outptr_result_buffer_to(s,c)               /* !NULL ptr to post-!NULL ptr to array of s writable elems, c elems are post-valid */
#define A_Outptr_result_buffer_to_maybenull(s,c)     /* !NULL ptr to post-NULL? ptr to array of s writable elems, c elems are post-valid */
#define A_Outptr_opt_result_buffer_to(s,c)           /* NULL? ptr to post-!NULL ptr to array of s writable elems, c elems are post-valid */
#define A_Outptr_opt_result_buffer_to_maybenull(s,c) /* NULL? ptr to post-NULL? ptr to array of s writable elems, c elems are post-valid */
#define A_Outptr_result_buffer_all(s)                /* !NULL ptr to post-!NULL ptr to array of s writable elems, s elems are post-valid */
#define A_Outptr_result_buffer_all_maybenull(s)      /* !NULL ptr to post-NULL? ptr to array of s writable elems, s elems are post-valid */
#define A_Outptr_opt_result_buffer_all(s)            /* NULL? ptr to post-!NULL ptr to array of s writable elems, s elems are post-valid */
#define A_Outptr_opt_result_buffer_all_maybenull(s)  /* NULL? ptr to post-NULL? ptr to array of s writable elems, s elems are post-valid */
#define A_Outptr_result_bytebuffer(s)                    /* !NULL ptr to post-!NULL ptr to post-valid array of s writable bytes              */
#define A_Outptr_result_bytebuffer_maybenull(s)          /* !NULL ptr to post-NULL? ptr to post-valid array of s writable bytes              */
#define A_Outptr_opt_result_bytebuffer(s)                /* NULL? ptr to post-!NULL ptr to post-valid array of s writable bytes              */
#define A_Outptr_opt_result_bytebuffer_maybenull(s)      /* NULL? ptr to post-NULL? ptr to post-valid array of s writable bytes              */
#define A_Outptr_result_bytebuffer_to(s,c)               /* !NULL ptr to post-!NULL ptr to array of s writable bytes, c bytes are post-valid */
#define A_Outptr_result_bytebuffer_to_maybenull(s,c)     /* !NULL ptr to post-NULL? ptr to array of s writable bytes, c bytes are post-valid */
#define A_Outptr_opt_result_bytebuffer_to(s,c)           /* NULL? ptr to post-!NULL ptr to array of s writable bytes, c bytes are post-valid */
#define A_Outptr_opt_result_bytebuffer_to_maybenull(s,c) /* NULL? ptr to post-NULL? ptr to array of s writable bytes, c bytes are post-valid */
#define A_Outptr_result_bytebuffer_all(s)                /* !NULL ptr to post-!NULL ptr to array of s writable bytes, s bytes are post-valid */
#define A_Outptr_result_bytebuffer_all_maybenull(s)      /* !NULL ptr to post-NULL? ptr to array of s writable bytes, s bytes are post-valid */
#define A_Outptr_opt_result_bytebuffer_all(s)            /* NULL? ptr to post-!NULL ptr to array of s writable bytes, s bytes are post-valid */
#define A_Outptr_opt_result_bytebuffer_all_maybenull(s)  /* NULL? ptr to post-NULL? ptr to array of s writable bytes, s bytes are post-valid */
#define A_Printf_format_string   /* pre/post printf-like format string   (NULL-ness?, validity?)                                  */
#define A_Group(anns)            /* group annotations, e.g.: A_Group(A_Post A_Null)                                               */
#define A_On_failure(anns)       /* check post-conditions if expr of A_Success(expr) is 0, e.g.: A_On_failure(A_Null)             */
#define A_Always(anns)           /* check post-conditions regardless of expr value of A_Success(expr)                             */
#define A_At(at,anns)            /* at location, e.g.: A_On_failure(A_At(*p, A_Post_null)) T **p                                  */
#define A_At_buffer(b,i,c,anns)  /* A_At_buffer(b,i,c,A_At(b[i],A_In_z)) foo(char *b[], unsigned c); (NULL-ness?, validity?)      */
#define A_When(cond,anns)        /* A_When(return != 0, A_Out_writes(size)) T p[]                                                 */
#define A_Pre                    /* marks pre-condition         (NULL-ness?, validity?)                                           */
#define A_Post                   /* marks post-condition        (NULL-ness?, validity?)                                           */
#define A_Pre_z                  /* pre valid and 0-term        (NULL-ness?)                                                      */
#define A_Post_z                 /* post valid 0-term           (NULL-ness?)                                                      */
#define A_Prepost_z              /* pre/post valid and 0-term   (NULL-ness?)                                                      */
#define A_Post_maybez            /* post 0-term?                (NULL-ness?, validity?)                                           */
#define A_Pre_null               /* pre NULL                                                                                      */
#define A_Post_null              /* post NULL                                                                                     */
#define A_Null                   /* pre/post NULL                                                                                 */
#define A_Pre_notnull            /* pre !NULL                   (validity?)                                                       */
#define A_Post_notnull           /* post !NULL                  (validity?)                                                       */
#define A_Notnull                /* pre/post !NULL              (validity?)  can be applied to a field: struct P {A_Notnull void *ptr;}; */
#define A_Pre_maybenull          /* pre NULL?                   (validity?)                                                       */
#define A_Post_maybenull         /* post NULL?                  (validity?)                                                       */
#define A_Maybenull              /* pre/post NULL?              (validity?)  can be applied to a field: struct S {A_Maybenull char *s;}; */
#define A_Pre_valid              /* pre !NULL and valid                        (down-recursive: valid pointer references only valid data) */
#define A_Pre_opt_valid          /* pre NULL? and valid                        (down-recursive: valid pointer references only valid data) */
#define A_Post_valid             /* post points to valid data     (NULL-ness?) (down-recursive: valid pointer references only valid data) */
#define A_Valid                  /* pre/post points to valid data (NULL-ness?) (down-recursive: valid pointer references only valid data) */
#define A_Pre_deref_invalid      /* pre points to invalid data      (NULL-ness?) (up-recursive: valid data cannot contain pointers to invalid one) */
#define A_Post_deref_invalid     /* post points to invalid data     (NULL-ness?) (up-recursive: valid data cannot contain pointers to invalid one) */
#define A_Deref_invalid          /* pre/post points to invalid data (NULL-ness?) (up-recursive: valid data cannot contain pointers to invalid one) */
#define A_Notvalid               /* pre/post invalid pointer (data) (NULL-ness?) (up-recursive: valid data cannot contain invalid pointers) */
#define A_Post_ptr_invalid       /* post invalid pointer            (NULL-ness?) (up-recursive: valid data cannot contain invalid pointers) */
#define A_Pre_unknown            /* pre valid?                      (NULL-ness?) (up-recursive: valid data cannot contain invalid pointers) */
#define A_Maybevalid             /* pre/post valid?                 (NULL-ness?) (up-recursive: valid data cannot contain invalid pointers) */
#define A_Use_decl_annotations   /* use annotations from function declaration for the function definition                  */
#define A_Readable_bytes(n)      /* pre/post ptr to n readable bytes    (NULL-ness?, validity?)                            */
#define A_Writable_bytes(n)      /* pre/post ptr to n writable bytes    (NULL-ness?, validity?)                            */
#define A_Readable_elements(n)   /* pre/post ptr to n readable elems    (NULL-ness?, validity?)                            */
#define A_Writable_elements(n)   /* pre/post ptr to n writable elems    (NULL-ness?, validity?)                            */
#define A_Null_terminated        /* pre/post ptr to 0-term string       (NULL-ness?, validity?)                            */
#define A_Pre_readable_size(s)       /* pre array with s valid elems       (NULL-ness?)                                    */
#define A_Post_readable_size(s)      /* post array with s valid elems      (NULL-ness?)                                    */
#define A_Pre_writable_size(s)       /* pre array with s writable elems    (NULL-ness?, validity?)                         */
#define A_Post_writable_size(s)      /* post array with s writable elems   (NULL-ness?, validity?)                         */
#define A_Pre_readable_byte_size(s)  /* pre array with s valid bytes       (NULL-ness?)                                    */
#define A_Post_readable_byte_size(s) /* post array with s valid bytes      (NULL-ness?)                                    */
#define A_Pre_writable_byte_size(s)  /* pre array with s writable bytes    (NULL-ness?, validity?)                         */
#define A_Post_writable_byte_size(s) /* post array with s writable bytes   (NULL-ness?, validity?)                         */
#define A_Struct_size_bytes(s)       /* pre/post array of s writable bytes       (NULL-ness?, validity?)                   */
#define A_Field_size(s)              /* pre/post !NULL array of s writable elems                 (validity?)               */
#define A_Field_size_opt(s)          /* pre/post NULL? array of s writable elems                 (validity?)               */
#define A_Field_size_part(s,c)       /* pre/post !NULL array of s writable and c readable elems  (validity?)               */
#define A_Field_size_part_opt(s,c)   /* pre/post NULL? array of s writable and c readable elems  (validity?)               */
#define A_Field_size_full(s)         /* pre/post !NULL array of s read/writable elems            (validity?)               */
#define A_Field_size_full_opt(s)     /* pre/post NULL? array of s read/writable elems            (validity?)               */
#define A_Field_size_bytes(s)            /* pre/post !NULL array of s writable bytes                 (validity?)           */
#define A_Field_size_bytes_opt(s)        /* pre/post NULL? array of s writable bytes                 (validity?)           */
#define A_Field_size_bytes_part(s,c)     /* pre/post !NULL array of s writable and c readable bytes  (validity?)           */
#define A_Field_size_bytes_part_opt(s,c) /* pre/post NULL? array of s writable and c readable bytes  (validity?)           */
#define A_Field_size_bytes_full(s)       /* pre/post !NULL array of s read/writable bytes            (validity?)           */
#define A_Field_size_bytes_full_opt(s)   /* pre/post NULL? array of s read/writable bytes            (validity?)           */
#define A_Field_z                    /* pre/post 0-term array of chars     (NULL-ness?, validity?)                         */
#define A_Field_range(l,h)           /* pre/post field value is in given range (inclusive)                                 */

#endif /* !_MSC_VER || NO_SAL_ANNOTATIONS */

#ifndef ASSUME
#if defined _MSC_VER
#define ASSUME(cond) __assume(!!(cond))
#elif defined __clang_analyzer__
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#elif defined __clang__
#define ASSUME(cond) __builtin_assume(!!(cond))
#elif defined __INTEL_COMPILER
#define ASSUME(cond) ((void)0) /* ICC compiles calls to __builtin_unreachable() as jumps somewhere... */
#elif defined __GNUC__ && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#define ASSUME(cond) ((void)(!(cond) ? __builtin_unreachable(), 0 : 1))
#else
#define ASSUME(cond) ((void)0) /* assume condition is always true */
#endif
#endif /* ASSUME */

/*
 mark pointer as valid to avoid false analyzer warnings

 for example:

 struct A {
   struct B b;
   struct C c;
 };
 void B_destroy(A_Pre_valid A_Post_deref_invalid struct B *b);
 void C_destroy(A_Pre_valid A_Post_deref_invalid struct C *c);
 void A_destroy(A_Pre_valid A_Post_deref_invalid struct A *a)
 {
   B_destroy(&a->b);
   C_destroy(&a->c); // here analyzer gives (false) warning C6001: Using uninitialized memory '*a'
 }

 to suppress analyzer warning, use A_Mark_valid():

 void A_destroy(A_Pre_valid A_Post_deref_invalid struct A *a)
 {
   B_destroy(&a->b);
   C_destroy(A_Mark_valid(&a->c));
 }
*/
#ifdef _MSC_VER

#pragma warning(push)
#pragma warning(disable:6101) /* Returning uninitialized memory '*p' */

#ifdef __cplusplus

template <class T>
A_Ret_valid A_Ret_range(==,p)
__forceinline T *A_Mark_valid(
	A_Notnull A_Pre_deref_invalid A_Post_valid T *const p/*!=NULL*/)
{
	return const_cast<T*>(p);
}

template <class T>
A_When(!p, A_Ret_null)
A_When(!!p, A_Ret_valid)
A_Ret_range(==,p)
__forceinline T *A_Mark_opt_valid(
	A_Maybenull A_Pre_deref_invalid A_Post_valid T *const p/*NULL?*/)
{
	return const_cast<T*>(p);
}

#else /* !__cplusplus */

A_Ret_valid A_Ret_range(==,p)
static __forceinline void *A_Mark_valid(
	A_Notnull A_Pre_deref_invalid A_Post_valid void *const p/*!=NULL*/)
{
	return p;
}

A_When(!p, A_Ret_null)
A_When(!!p, A_Ret_valid)
A_Ret_range(==,p)
static __forceinline void *A_Mark_opt_valid(
	A_Maybenull A_Pre_deref_invalid A_Post_valid void *const p/*NULL?*/)
{
	return (void*)p;
}

#endif /* !__cplusplus */

#pragma warning(pop)

#elif defined __GNUC__

A_Const_function A_Nonnull_all_args A_Ret_never_null
static inline void *A_Mark_valid_(void *const p/*!=NULL*/)
{
	return p;
}

A_Const_function
static inline void *A_Mark_opt_valid_(void *const p/*NULL?*/)
{
	return p;
}

#define A_Mark_valid(p)     ((__typeof__(p))A_Mark_valid_(p))
#define A_Mark_opt_valid(p) ((__typeof__(p))A_Mark_opt_valid_(p))

#else /* !__GNUC__ */

#define A_Mark_valid(p)     (p)
#define A_Mark_opt_valid(p) (p)

#endif /* !__GNUC__ */

#ifndef FALLTHROUGH
#if defined __cplusplus && __cplusplus >= 201703L
#define FALLTHROUGH [[fallthrough]]
#elif defined(__GNUC__) && (__GNUC__ >= 7)
#define FALLTHROUGH __attribute__ ((fallthrough))
#elif defined __cplusplus && defined(__clang__) && (__clang_major__ > 3 || \
  (3 == __clang_major__  && __clang_minor__ >= 6))
#define FALLTHROUGH [[clang::fallthrough]]
#else
#define FALLTHROUGH ((void)0)
#endif
#endif /* !FALLTHROUGH */

/* if all values of a switched enum are handled in cases,
  clang knows that 'default' case is not applicable here,
  use NO_DEFAULT_CASE to avoid -Wswitch-default gcc warning:
  enum E {A,B};
  enum E e = A;
  switch (e) {
   case A: break;
   case B: break;
   NO_DEFAULT_CASE
  } */
#ifndef NO_DEFAULT_CASE
#ifdef __clang__
#define NO_DEFAULT_CASE
#elif defined ASSERT
#define NO_DEFAULT_CASE default: ASSERT(0);
#else /* !ASSERT */
#define NO_DEFAULT_CASE default: ASSUME(0);
#endif /* !ASSERT */
#endif /* !NO_DEFAULT_CASE */

#ifdef __NETBEANS_PREPROCESSING
#undef A_Force_inline_function
#define A_Force_inline_function
#endif

#endif /* SAL_DEFS_H_INCLUDED */
