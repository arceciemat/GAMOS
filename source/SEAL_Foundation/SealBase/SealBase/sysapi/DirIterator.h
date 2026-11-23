#ifndef SEAL_BASE_SYSAPI_DIR_ITERATOR_H
# define SEAL_BASE_SYSAPI_DIR_ITERATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
# else
#  // FIXME: On SCO, use libs `dir' and `x'.
#  if HAVE_DIRENT_H
#   include <dirent.h>
#   define DIRENT_NAMLEN(dirent) strlen((dirent)->d_name)
#  else
#   define dirent direct
#   define DIRENT_NAMLEN(dirent) (dirent)->d_namlen
#   if HAVE_SYS_NDIR_H
#    include <sys/ndir.h>
#   endif
#   if HAVE_SYS_DIR_H
#    include <sys/dir.h>
#   endif
#   if HAVE_NDIR_H
#    include <ndir.h>
#   endif
#  endif
#  if CLOSEDIR_VOID
#   define closedir(x) (closedir(x),0)
#  endif
# endif

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct DirIteratorData
{
#ifdef _WIN32
    HANDLE		m_dir;
    WIN32_FIND_DATA	m_dirent;
#else
    DIR			*m_dir;
    dirent		*m_dirent;
#endif
    unsigned int	m_refs;
    bool		m_last;
    bool		m_valid;
    bool		m_dots;
    std::string		*m_last_name;
};

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_SYSAPI_DIR_ITERATOR_H
