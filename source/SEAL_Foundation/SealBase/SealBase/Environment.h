#ifndef SEAL_BASE_ENVIRONMENT_H
# define SEAL_BASE_ENVIRONMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# include <string>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Utilities for handling environments. */
class Environment
{
public:
    virtual ~Environment (void);

    virtual bool		has (const std::string &name) const;
    virtual std::string		get (const std::string &name,
		    		     bool *have = 0) const = 0;
    bool			get (const std::string &name,
		    		     std::string &value) const;
    virtual void		set (const std::string &name,
				     const std::string &value) = 0;
    virtual void		unset (const std::string &name) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_ENVIRONMENT_H
