//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/Pipe.h"
#include "SealBase/IOError.h"
#include "SealBase/sysapi/Pipe.h"
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

void
Pipe::open (void)
{
    int fds [2];
    if (::pipe (fds) == -1)
	throw IOError ("pipe()", errno);

    m_source.fd (fds [0]);
    m_sink.fd (fds [1]);
}

} // namespace seal
#endif // !_WIN32
