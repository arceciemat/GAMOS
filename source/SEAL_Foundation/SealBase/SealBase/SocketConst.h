#ifndef SEAL_BASE_SOCKET_CONST_H
# define SEAL_BASE_SOCKET_CONST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"

namespace seal {
namespace SocketConst {

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/// Maximum queue length for listen().
SEAL_BASE_API extern const int MaxListen;		// SOMAXCONN

/// Shutdown close modes
SEAL_BASE_API extern const int CloseDisallowReceive;
SEAL_BASE_API extern const int CloseDisallowSend;
SEAL_BASE_API extern const int CloseDisallowAll;

/// Socket types
SEAL_BASE_API extern const int TypeStream;		// SOCK_STREAM
SEAL_BASE_API extern const int TypeDatagram;		// SOCK_DGRAM
SEAL_BASE_API extern const int TypeRaw;			// SOCK_RAW
// SEAL_BASE_API extern const int TypeRDM;		// SOCK_RDM
// SEAL_BASE_API extern const int TypeSeqPacket;	// SOCK_SEQPACKET

/// Socket options: the protocol level at which an option applies
SEAL_BASE_API extern const int OptLevelSocket;		// Socket level
SEAL_BASE_API extern const int OptLevelTCP;		// TCP level
SEAL_BASE_API extern const int OptLevelIP;		// IP level

/// Options for OptLevelSocket
SEAL_BASE_API extern const int OptSockDebug;		// SO_DEBUG		int boolean
SEAL_BASE_API extern const int OptSockListening;	// SO_ACCEPTCONN	int boolean
SEAL_BASE_API extern const int OptSockReuseAddress;	// SO_REUSEADDR		int boolean
SEAL_BASE_API extern const int OptSockKeepAlive;	// SO_KEEPALIVE		int boolean
SEAL_BASE_API extern const int OptSockDontRoute;	// SO_DONTROUTE		int boolean
SEAL_BASE_API extern const int OptSockBroadcast;	// SO_BROADCAST		int boolean
SEAL_BASE_API extern const int OptSockUseLoopback;	// SO_USELOOPBACK	int boolean
SEAL_BASE_API extern const int OptSockLinger;		// SO_LINGER		struct linger *
SEAL_BASE_API extern const int OptSockOOBInline;	// SO_OOBINLINE		int boolean
SEAL_BASE_API extern const int OptSockReusePort;	// SO_REUSEPORT		int boolean
SEAL_BASE_API extern const int OptSockSendBuffer;	// SO_SNDBUF		int (# of bytes)
SEAL_BASE_API extern const int OptSockReceiveBuffer;	// SO_RCVBUF		int (# of bytes)
SEAL_BASE_API extern const int OptSockSendLowater;	// SO_SNDLOWAT		int (# of bytes)
SEAL_BASE_API extern const int OptSockReceiveLowater;	// SO_RCVLOWAT		int (# of bytes)
SEAL_BASE_API extern const int OptSockSendTimeout;	// SO_SNDTIMEO		struct timeval *
SEAL_BASE_API extern const int OptSockReceiveTimeout;	// SO_RCVTIMEO		struct timeval *
SEAL_BASE_API extern const int OptSockError;		// SO_ERROR		int
SEAL_BASE_API extern const int OptSockType;		// SO_TYPE		int (TypeXyz)
SEAL_BASE_API extern const int OptSockPassCreds;	// SO_PASSCRED		int boolean
SEAL_BASE_API extern const int OptSockPeerCreds;	// SO_PEERCRED		struct ucred *
SEAL_BASE_API extern const int OptSockBindToDevice;	// SO_BINDTODEVICE	char * ([IFNAMESIZ])
SEAL_BASE_API extern const int OptSockPriority;		// SO_PRIORITY		int
// SEAL_BASE_API extern const int OptSockBSDCompat;	// SO_BSDCOMPAT		int boolean

/// Options for OptLevelTCP
SEAL_BASE_API extern const int OptTCPNoDelay;		// TCP_NODELAY		int boolean
SEAL_BASE_API extern const int OptTCPMaxSegmentSize;	// TCP_MAXSEG		int
SEAL_BASE_API extern const int OptTCPCork;		// TCP_CORK		int boolean

/// Options for OptLevelIP
// SEAL_BASE_API extern const int OptIPOptions;		// IP_OPTIONS		RFC 791
// SEAL_BASE_API extern const int OptIPPacketInfo;	// IP_PKTINFO
// SEAL_BASE_API extern const int OptIPReceiveTOS;	// IP_RECVTOS
// SEAL_BASE_API extern const int OptIPReceiveTTL;	// IP_RECVTTL
// SEAL_BASE_API extern const int OptIPReceiveOpts;	// IP_RECVOPTS
// SEAL_BASE_API extern const int OptIPReceiveRawOpts;	// IP_RETOPTS
// SEAL_BASE_API extern const int OptIPTOS;		// IP_TOS
// SEAL_BASE_API extern const int OptIPTTL;		// IP_TTL
// SEAL_BASE_API extern const int OptIPHeaderIncluded;	// IP_HDRINCL
// SEAL_BASE_API extern const int OptIPReceiveErrors;	// IP_RECVERR
// SEAL_BASE_API extern const int OptIPPMTUDiscovery;	// IP_PMTU_DISCOVER
// SEAL_BASE_API extern const int OptIPMTU;		// IP_MTU
// SEAL_BASE_API extern const int OptIPRouterAlert;	// IP_ROUTER_ALERT
// SEAL_BASE_API extern const int OptIPMulticastTTL;	// IP_MULTICAST_TTL	unsigned char
// SEAL_BASE_API extern const int OptIPMulticastLoop;	// IP_MULTICAST_LOOP	unsigned char
// SEAL_BASE_API extern const int OptIPAddMembership;	// IP_ADD_MEMBERSHIP	struct ip_mreq *
// SEAL_BASE_API extern const int OptIPDropMembership;	// IP_DROP_MEMBERSHIP	struct ip_mreq *
// SEAL_BASE_API extern const int OptIPMulticastInterface; // IP_MULTICAST_IF	struct in_addr *

/// Message send/receive options
SEAL_BASE_API extern const int MsgOutOfBand;		// MSG_OOB
SEAL_BASE_API extern const int MsgPeek;			// MSG_PEEK
SEAL_BASE_API extern const int MsgWaitAll;		// MSG_WAITALL
// SEAL_BASE_API extern const int MsgNoSignal;		// MSG_NOSIGNAL
// SEAL_BASE_API extern const int MsgTruncate;		// MSG_TRUNC
// SEAL_BASE_API extern const int MsgErrorQueue;	// MSG_ERRQUEUE
// SEAL_BASE_API extern const int MsgNoWait;		// MSG_DONTWAIT

// SEAL_BASE_API extern const int MsgRecord;		// MSG_EOR (output)
// SEAL_BASE_API extern const int MsgTruncated;		// MSG_TRUNC (output)
// SEAL_BASE_API extern const int MsgControlTruncated;	// MSG_CTRUNC (output)
//							// MSG_OOB (output)
//							// MSG_ERRQUEUE (output)
//							// MSG_DONTWAIT (output)

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace SocketConst
} // namespace seal

#endif // SEAL_BASE_SOCKET_CONST_H
