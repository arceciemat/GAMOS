#ifndef SEAL_BASE_SYSAPI_FILE_H
# define SEAL_BASE_SYSAPI_FILE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <utime.h>
#  include <limits.h>
#  include <cerrno>
#  include <cstdlib>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# if !defined O_SYNC && defined O_SYNCIO
#  define O_SYNC O_SYNCIO
# endif

# if !defined O_NONBLOCK && defined O_NDELAY
#  define O_NONBLOCK O_NDELAY
# endif

# ifndef O_NONBLOCK
#  define O_NONBLOCK 0
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // SEAL_BASE_SYSAPI_FILE_H
