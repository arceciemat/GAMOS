#ifndef PLUGIN_MANAGER_PLUGIN_CAPABILITIES_H
# define PLUGIN_MANAGER_PLUGIN_CAPABILITIES_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/PluginFactory.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class PluginCapabilities : public PluginFactory<void>
{
public:
    static PluginCapabilities *get (void);

private:
    PluginCapabilities (void);
    static PluginCapabilities s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_PLUGIN_CAPABILITIES_H
