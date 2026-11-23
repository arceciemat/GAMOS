#ifndef SEAL_BASE_SAVE_ERRNO_H
# define SEAL_BASE_SAVE_ERRNO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# ifdef _WIN32
#  include <windows.h>
# endif
# include <cerrno>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A class to save and restore @c errno over while the object exists.
    On Windows saves both @c errno and @c GetLastError() value. */
class SaveErrno
{
public:
    SaveErrno (void);
    ~SaveErrno (void);
    // implicit copy constructor
    // implicit assignment operator

private:
    int		m_errno;
    int		m_winerr;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Save the current value of @c errno.  */
inline
SaveErrno::SaveErrno (void)
{
    m_errno = errno;
# ifdef _WIN32
    m_winerr = GetLastError ();
# endif
}

/** Restore the saved value of @c errno.  */
inline
SaveErrno::~SaveErrno (void)
{
    errno = m_errno;
# ifdef _WIN32
    SetLastError (m_winerr);
# endif
}

} // namespace seal
#endif // SEAL_BASE_SAVE_ERRNO_H
