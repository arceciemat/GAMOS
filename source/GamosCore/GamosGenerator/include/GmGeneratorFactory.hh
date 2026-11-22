#ifndef GmGeneratorFactory_H
#define GmGeneratorFactory_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type G4VUserPrimaryGeneratorAction
class GmGeneratorFactory : public seal::PluginFactory<G4VUserPrimaryGeneratorAction *()>
{
public:
    static GmGeneratorFactory *get (void);

private:
    GmGeneratorFactory (void);
    static GmGeneratorFactory s_instance;
};

#define DEFINE_GAMOS_GENERATOR(type) \
  DEFINE_SEAL_PLUGIN(GmGeneratorFactory, type,#type)

#define DEFINE_GAMOSGENERATOR(type) \
  DEFINE_SEAL_PLUGIN(GmGeneratorFactory, type,#type)
 

#endif // GmGeneratorFactory_H
