#ifndef SEAL_BASE_SYSAPI_FILE_NAME_LOCK_H
# define SEAL_BASE_SYSAPI_FILE_NAME_LOCK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# include <cerrno>
# ifndef _WIN32
#  include <utmp.h>
#  if HAVE_SYS_SYSCTL_H
#   include <sys/sysctl.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // SEAL_BASE_SYSAPI_FILE_NAME_LOCK_H
