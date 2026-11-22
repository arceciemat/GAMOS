#ifndef SEAL_BASE_FILE_MAPPING_ERROR_H
# define SEAL_BASE_FILE_MAPPING_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/IOError.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** File-system related exception class. */
class FileMappingError : public IOError
{
public:
    FileMappingError (const char *context, int code = 0);
    FileMappingError (const char *context, Error *chain);
    FileMappingError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_FILE_MAPPING_ERROR_H
