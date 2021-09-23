#ifndef SEAL_BASE_SYSAPI_IO_CHANNEL_H
# define SEAL_BASE_SYSAPI_IO_CHANNEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# include <errno.h>
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <fcntl.h>
#  if HAVE_SYS_UIO_H
#   include <sys/uio.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# if HAVE_SYS_UIO_H
#  ifndef HAVE_CADDR_T
#   define caddr_t void *
#  endif
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // SEAL_BASE_SYSAPI_IO_CHANNEL_H
