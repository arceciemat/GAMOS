#ifndef GmDistributionFactory_hh
#define GmDistributionFactory_hh

#include "GmVDistribution.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVDistribution
class GmDistributionFactory : public seal::PluginFactory<GmVDistribution *(G4String )>
{
public:
    static GmDistributionFactory *get (void);

private:
    GmDistributionFactory (void);
    static GmDistributionFactory s_instance;
};

#define DEFINE_GAMOS_DISTRIBUTION(type) \
  DEFINE_SEAL_PLUGIN(GmDistributionFactory, type,#type)

#endif // GmDistributionFactory_hh
