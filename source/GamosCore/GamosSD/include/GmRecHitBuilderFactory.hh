#ifndef GmRecHitBuilderFactory_H
#define GmRecHitBuilderFactory_H

#include "GmVRecHitBuilder.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVRecHitBuilder
class GmRecHitBuilderFactory : public seal::PluginFactory<GmVRecHitBuilder *()>
{
public:
    static GmRecHitBuilderFactory *get (void);

private:
    GmRecHitBuilderFactory (void);
    static GmRecHitBuilderFactory s_instance;
};


#define DEFINE_GAMOS_RECHIT_BUILDER(type) \
  DEFINE_SEAL_PLUGIN(GmRecHitBuilderFactory, type,#type)

#endif // GmRecHitBuilderFactory_H
