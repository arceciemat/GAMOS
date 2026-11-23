//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/Pipe.h"
#include "SealBase/IOError.h"
#include "SealBase/sysapi/Pipe.h"
#ifdef _WIN32

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
    // make the pipe handles inherited
    SECURITY_ATTRIBUTES	security; 
    security.nLength = sizeof (security); 
    security.bInheritHandle = TRUE; 
    security.lpSecurityDescriptor = NULL; 

    HANDLE fds [2];
    if (! CreatePipe (&fds[0], &fds [1], &security, 0))
	throw IOError ("CreatePipe()", GetLastError ());

    m_source.fd (fds [0]);
    m_sink.fd (fds [1]);
}

} // namespace seal
#endif // _WIN32
