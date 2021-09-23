//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/SocketPair.h"
#include "SealBase/NetworkError.h"
#include "SealBase/sysapi/SocketPair.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

SocketPair::SocketPair (bool create /* = true */)
{ if (create) open (); }

SocketPair::~SocketPair (void)
{}

IOChannel *
SocketPair::source (void)
{ return &m_source; }

IOChannel *
SocketPair::sink (void)
{ return &m_sink; }

} // namespace seal
