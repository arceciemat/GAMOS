#ifndef GmFilterFactory_hh
#define GmFilterFactory_hh

#include "GmVFilter.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVFilter
class GmFilterFactory : public seal::PluginFactory<GmVFilter *(G4String )>
{
public:
    static GmFilterFactory *get (void);

private:
    GmFilterFactory (void);
    static GmFilterFactory s_instance;
};

#define DEFINE_GAMOS_FILTER(type) \
  DEFINE_SEAL_PLUGIN(GmFilterFactory, type,#type)

#endif // GmFilterFactory_hh
