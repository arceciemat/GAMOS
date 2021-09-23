//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/Pipe.h"
#include "SealBase/IOError.h"
#include "SealBase/sysapi/Pipe.h"

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

Pipe::Pipe (bool create /* = true */)
{ if (create) open (); }

IOChannel *
Pipe::source (void)
{ return &m_source; }

IOChannel *
Pipe::sink (void)
{ return &m_sink; }

} // namespace seal
