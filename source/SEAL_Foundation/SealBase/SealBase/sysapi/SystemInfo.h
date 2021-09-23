#ifndef SEAL_BASE_SYSAPI_SYSTEM_INFO_H
# define SEAL_BASE_SYSAPI_SYSTEM_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/param.h>
#  include <sys/utsname.h>
#  if HAVE_SYS_SYSCTL_H			// *BSD, Darwin, Linux
#    include <sys/sysctl.h>
#    if HAVE_VM_VM_PARAM_H		// *BSD
#      include <vm/vm_param.h>
#    endif
#    if HAVE_SYS_VMMETER_H		// Darwin
#      include <sys/vmmeter.h>
#    endif
#  endif
#  if HAVE_SYS_PROCESSOR_H		// Solaris
#    include <sys/processor.h>
#  endif
#  include <cstdio>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // SEAL_BASE_SYSAPI_SYSTEM_INFO_H
