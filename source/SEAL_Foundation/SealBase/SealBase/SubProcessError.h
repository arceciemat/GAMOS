#ifndef SEAL_BASE_SUB_PROCESS_ERROR_H
# define SEAL_BASE_SUB_PROCESS_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Error.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** #SubProcess-related exception class. */
class SubProcessError : public Error
{
public:
    SubProcessError (const char *context, int code = 0);
    SubProcessError (const char *context, Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    std::string		m_context;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_SUB_PROCESS_ERROR_H
