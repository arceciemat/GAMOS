#ifndef SEAL_BASE_IO_OUTPUT_H
# define SEAL_BASE_IO_OUTPUT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/IOBuffer.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class for stream-oriented output targets. */
class IOOutput
{
public:
    virtual ~IOOutput (void);
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator

    IOSize		write (unsigned char byte);
    IOSize		write (IOBuffer from);
    virtual IOSize	write (const void *from, IOSize n) = 0;
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);

    IOSize		xwrite (const void *from, IOSize n);
    IOSize		xwrite (IOBuffer from);
    IOSize		xwritev (const IOBuffer *from, IOSize buffers);

protected:
    // Trap migration errors, these are undefined.  Use x/readv() instead.
    IOSize		write (IOBuffer *into, IOSize buffers);
    IOSize		xwrite (IOBuffer *into, IOSize buffers);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_IO_OUTPUT_H
