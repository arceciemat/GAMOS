#ifndef SEAL_BASE_SYSAPI_SUB_PROCESS_H
# define SEAL_BASE_SYSAPI_SUB_PROCESS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <cerrno>
#  include <unistd.h>
#  if HAVE_SYS_WAIT_H
#   include <sys/wait.h>
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

#endif // SEAL_BASE_SYSAPI_SUB_PROCESS_H
