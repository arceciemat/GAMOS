#ifndef SEAL_BASE_SHARED_LIBRARY_ERROR_H
# define SEAL_BASE_SHARED_LIBRARY_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Error.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Error in a shared library operation. */
class SharedLibraryError : public Error
{
public:
    SharedLibraryError (const char *context, const std::string &cause);
    SharedLibraryError (const char *context, int code);
    SharedLibraryError (const char *context, Error *next);
    SharedLibraryError (Error *chain);

    // default destructor
    // default copy constructor
    // default assignment operator

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    std::string		m_context;
    std::string		m_cause;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_SHARED_LIBRARY_ERROR_H
