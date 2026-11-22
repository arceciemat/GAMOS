#ifndef GmClassifierFactory_hh
#define GmClassifierFactory_hh

#include "GmVClassifier.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"
#include <vector>

/// Plug-in factory for objects of type GmVClassifier
class GmClassifierFactory : public seal::PluginFactory<GmVClassifier *(G4String)>
{
public:
    static GmClassifierFactory *get (void);

private:
    GmClassifierFactory (void);
    static GmClassifierFactory s_instance;
};

#define DEFINE_GAMOS_CLASSIFIER(type) \
  DEFINE_SEAL_PLUGIN(GmClassifierFactory, type,#type)

#endif // GmClassifierFactory_hh
