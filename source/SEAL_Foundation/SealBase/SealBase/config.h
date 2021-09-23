#ifndef SEAL_BASE_CONFIG_H
# define SEAL_BASE_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealPlatform/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def SEAL_BASE_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #SEAL_BASE_BUILD_DLL is defined, imported
         if #SEAL_BASE_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def SEAL_BASE_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #SEAL_BASE_API should be exported.  */

/** @def SEAL_BASE_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #SEAL_BASE_API should be imported.  */

# ifndef SEAL_BASE_API
#  ifdef _WIN32
#    if defined SEAL_BASE_BUILD_DLL
#      define SEAL_BASE_API __declspec(dllexport)
#    elif ! defined SEAL_BASE_BUILD_ARCHIVE
#      define SEAL_BASE_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef SEAL_BASE_API
#  define SEAL_BASE_API
# endif

#endif // SEAL_BASE_CONFIG_H
