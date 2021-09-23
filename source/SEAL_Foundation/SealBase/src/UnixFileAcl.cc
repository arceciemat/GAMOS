//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/FileAcl.h"
#include "SealBase/sysapi/FileAcl.h"
#include "SealBase/sysapi/IOStatus.h"
#ifndef _WIN32

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int
FileAcl::portable (void) const
{
    // On unix m_perms is native.  The low bits are portable.
    int val = m_perms & 07777;

    if (S_ISCDF (m_perms))
	val |= Hidden;

    return val;
}

int
FileAcl::addPortable (int mask)
{
    if (mask & Hidden)
	m_perms |= S_IFCDF;

    return m_perms;
}

int
FileAcl::native (void) const
{ return m_perms; }

int FileAcl::addNative (int mask)
{ return m_perms |= mask; }

} // namespace seal
#endif // !_WIN32
