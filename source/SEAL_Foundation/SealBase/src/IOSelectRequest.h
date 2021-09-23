#ifndef SEAL_BASE_IOSELECT_REQUEST_H
# define SEAL_BASE_IOSELECT_REQUEST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/sysapi/IOTypes.h"
# include "SealBase/IOSelectCB.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Structure for recording information about the client requests.
    Remembers the file descriptor and the owner channel (the two
    represent identical information for convenience of the actual
    selection method), the event selection mask and the client
    callback.  This structure is only for internal use.  */
struct IOSelectRequest
{
    IOFD	fd;		//< The file descriptor
    IOChannel	*channel;	//< The source channel
    unsigned	mask;		//< Event interest selection mask
    IOSelectCB	cb;		//< Notification callback
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_IOSELECT_REQUEST_H
