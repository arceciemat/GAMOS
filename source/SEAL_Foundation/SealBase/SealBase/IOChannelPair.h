#ifndef SEAL_BASE_IO_CHANNEL_PAIR_H
# define SEAL_BASE_IO_CHANNEL_PAIR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/sysapi/IOTypes.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for paired I/O channels such as pipes, socket pairs,
    terminals etc.  (FIXME) */
class IOChannelPair
{
public:
    virtual ~IOChannelPair (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual IOChannel *	sink (void) = 0;
    virtual IOChannel *	source (void) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_IO_CHANNEL_PAIR_H
