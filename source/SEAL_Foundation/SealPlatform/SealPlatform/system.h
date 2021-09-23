#ifndef SEAL_PLATFORM_SYSTEM_H
# define SEAL_PLATFORM_SYSTEM_H

# include "SealPlatform/config.h"
# include <sys/types.h>

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

# if HAVE_SOCKADDR_UN
#  define HAVE_LOCAL_SOCKETS 1
# endif

# if HAVE_U_STACK_TRACE \
     || HAVE_BACKTRACE_SYMBOLS_FD \
     || (HAVE_EXCPT_H && HAVE_RLD_INTERFACE_H && HAVE_PDSC_H) \
     || (HAVE_EXCEPTION_H && HAVE_RLD_INTERFACE_H) \
     || defined PROG_PSTACK \
     || defined _WIN32 \
     || __GNUC__ > 3 \
     || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  define HAVE_STACK_BACKTRACE 1
# endif

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

/* FIXME: Calibrated values.  */
# define SEAL_FASTEST_TYPE_INT 1
/* # define HAVE_COPYING_PUTENV 1 */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

#if !HAVE_NAMESPACE_STD
# define std
#endif

#if HAVE_CXX_STDC_HEADERS
# define STDC std
#else
# define STDC
#endif

# if !HAVE_BOOL
#  ifndef bool
    typedef int bool;
#  endif
#  ifndef true
#   define true 1
#  endif
#  ifndef false
#   define false 0
#  endif
# endif

# if !HAVE_TYPENAME
#  define typename
# endif

# if !HAVE_EXPLICIT
#  define explicit
# endif

# if !HAVE_RESTRICT
#  if HAVE_PRIVATE_RESTRICT
#   define restrict __restrict
#  else
#   define restrict
#  endif
# endif

# if !HAVE_USING
#  define USING(x)
# else
#  define USING(x) using x
# endif

# if HAVE_NEW_STYLE_CASTS
#  define STATIC_CAST(type,expr)	static_cast<type> (expr)
#  define CONST_CAST(type,expr)		const_cast<type> (expr)
#  define REINTERPRET_CAST(type,expr)	reinterpret_cast<type> (expr)
# else
#  define STATIC_CAST(type,expr)	((type)(expr))
#  define CONST_CAST(type,expr)		((type)(expr))
#  define REINTERPRET_CAST(type,expr)	((type)(expr))
# endif

# if HAVE_DYNAMIC_CAST && HAVE_TYPEID 
#  define HAVE_FUNCTIONAL_RTTI 1
# endif

# if HAVE_TEMPLATE_NULL_ARGS
#  define SEAL_TEMPLATE_NULL_ARGS <>
# else
#  define SEAL_TEMPLATE_NULL_ARGS
# endif

# if HAVE_TEMPLATE_NULL_SPEC
#  define SEAL_TEMPLATE_NULL_SPEC template<>
# else
#  define SEAL_TEMPLATE_NULL_SPEC
# endif

# undef SEAL_FUNCTION
# if HAVE_PRETTY_FUNCTION
#  define SEAL_FUNCTION	__PRETTY_FUNCTION__
# elif HAVE_FUNCTION
#  define SEAL_FUNCTION	__FUNCTION__
# elif HAVE_FUNC
#  define SEAL_FUNCTION	__func__
# endif

# if HAVE_ITERATOR
#  define ITERATOR_BASE(cat,val,diff)	: public std::iterator<std::cat##_tag,val,diff>
# elif HAVE_ITERATOR_TRAITS
#  define ITERATOR_BASE(cat,val,diff)
# else
#  define ITERATOR_BASE(cat,val,diff)	: public std::cat<val,diff>
# endif

# if HAVE_REVERSE_ITERATOR_STYLE == 1
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::reverse_iterator<iter>
# elif HAVE_REVERSE_ITERATOR_STYLE == 2
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::reverse_iterator<iter,std::tag,value,reference,pointer,difference>
# elif HAVE_REVERSE_ITERATOR_STYLE == 3
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::tname<iter,value,reference,pointer,difference>
# elif HAVE_REVERSE_ITERATOR_STYLE == 4
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::os_##tname<iter,value,reference,pointer,difference>
# else
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	I_don_t_know_how_to_define_reverse_iterator
# endif

# ifdef _MSC_VER
#  pragma warning(disable:4786)		/* '255' characters in the debug information */
#  pragma warning(disable:4305)		/* conversion from double to float (5.0) */
#  pragma warning(disable:4800)		/* forcing value to bool 'true' or 'false' (5.0) */
# endif /* _MSC_VER */

#endif /* SEAL_PLATFORM_SYSTEM_H */
