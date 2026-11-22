#ifndef PLUGIN_MANAGER_PLUGIN_ERROR_H
# define PLUGIN_MANAGER_PLUGIN_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "SealBase/Error.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Error object for the plug-in manager.  All lower-level errors
    are chained to this type before being passed upwards.  */
class PluginError : public Error
{
public:
    PluginError (Error *reason);
    
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_PLUGIN_ERROR_H
