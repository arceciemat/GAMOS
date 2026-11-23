#ifndef SEAL_BASE_IO_ERROR_H
# define SEAL_BASE_IO_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Error.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** I/O-operation related exception class. */
class IOError : public Error
{
public:
    IOError (const char *context, int code = 0);
    IOError (const char *context, Error *chain);
    IOError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

protected:
    virtual std::string doexplain (const char *type) const;

private:
    std::string		m_context;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_IO_ERROR_H
