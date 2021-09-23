#ifndef GmPrimitiveScorerFactory_H
#define GmPrimitiveScorerFactory_H

#include "GmVPrimitiveScorer.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"
#include <vector>

/// Plug-in factory for objects of type GmVPrimitiveScorer
//class GmPrimitiveScorerFactory : public seal::PluginFactory<G4VPrimitiveScorer *(const G4String& )>
class GmPrimitiveScorerFactory : public seal::PluginFactory<GmVPrimitiveScorer *(G4String)>
{
public:
    static GmPrimitiveScorerFactory *get (void);

private:
    GmPrimitiveScorerFactory (void);
    static GmPrimitiveScorerFactory s_instance;
};

#define DEFINE_GAMOS_SCORER(type) \
  DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory, type,#type)

#endif // GmPrimitiveScorerFactory_H
