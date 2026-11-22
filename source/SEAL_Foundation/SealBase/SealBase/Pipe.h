#ifndef SEAL_BASE_PIPE_H
# define SEAL_BASE_PIPE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/IOChannelPair.h"
# include "SealBase/IOChannel.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Pair of file descriptors connected to each other.  */
class Pipe : public IOChannelPair
{
    // FIXME: block SIGPIPE -> EPIPE -> throw?  (write only, read -> eof)
public:
    Pipe (bool create = true);
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    virtual void	open (void);
    virtual IOChannel *	source (void);
    virtual IOChannel *	sink (void);

private:
    IOChannel		m_source;	//< Source channel
    IOChannel		m_sink;		//< Sink channel
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_PIPE_H
