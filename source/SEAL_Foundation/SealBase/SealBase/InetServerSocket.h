#ifndef SEAL_BASE_INET_SERVER_SOCKET_H
# define SEAL_BASE_INET_SERVER_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/InetSocket.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Internet domain socket set up to listen as a server.  */
class InetServerSocket : public InetSocket
{
public:
    InetServerSocket (const InetAddress &bindAddr,
		      int backlog = 1,
		      int type = SocketConst::TypeStream);

protected:
    virtual Socket *	copy (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_INET_SERVER_SOCKET_H
