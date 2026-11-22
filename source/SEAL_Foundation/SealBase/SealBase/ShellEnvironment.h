#ifndef SEAL_BASE_SHELL_ENVIRONMENT_H
# define SEAL_BASE_SHELL_ENVIRONMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Environment.h"
# include <string>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Process shell environment.  */
class ShellEnvironment : public Environment
{
public:
    using Environment::get;
    virtual bool		has (const std::string &name) const;
    virtual std::string		get (const std::string &name,
		    		     bool *have = 0) const;
    virtual void		unset (const std::string &name);
    virtual void		set (const std::string &name,
				     const std::string &value);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_SHELL_ENVIRONMENT_H
