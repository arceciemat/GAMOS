#ifndef SEAL_BASE_IOSELECT_EVENT_H
# define SEAL_BASE_IOSELECT_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Event to be sent to #IOSelector clients when a channel becomes
    ready for I/O activity.  */
struct IOSelectEvent
{
    IOChannel	*source; //< The channel that is ready for I/O.
    unsigned	events;  //< The kind of I/O #source is ready for.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_IOSELECT_EVENT_H
