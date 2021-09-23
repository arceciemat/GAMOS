#ifndef GmVerbosityFactory_hh
#define GmVerbosityFactory_hh

#include "GmVVerbosity.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVVerbosity
class GmVerbosityFactory : public seal::PluginFactory<GmVVerbosity *()>
{
public:
    static GmVerbosityFactory *get (void);

private:
    GmVerbosityFactory (void);
    static GmVerbosityFactory s_instance;
};

#define DEFINE_GAMOS_VERBOSITY(type) \
  DEFINE_SEAL_PLUGIN(GmVerbosityFactory, type,#type)

#define DEFINE_GAMOSVERBOSITY(type) \
  DEFINE_SEAL_PLUGIN(GmVerbosityFactory, type,#type)

#endif // GmVerbosityFactory_hh
