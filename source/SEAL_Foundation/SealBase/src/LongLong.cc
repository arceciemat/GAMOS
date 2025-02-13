//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/LongLong.h"
#include "SealBase/BitTraits.h"
#include <cstdio>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

#if !HAVE_OSTREAM_LONG_LONG_OVERLOAD && ! defined HAVE_INT64
SEAL_BASE_API std::ostream &operator<< (std::ostream &s, long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Ld", n);  // FIXME: ll, L, or q?
    return out << buf;
}

SEAL_BASE_API std::ostream &operator<< (std::ostream &s, unsigned long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Lu", n);  // FIXME: ll, L, or q?
    return out << buf;
}
#endif

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

} // namespace seal
