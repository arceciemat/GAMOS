#ifndef GmPhysicsFactory_H
#define GmPhysicsFactory_H

#include "G4VUserPhysicsList.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type G4VUserPhysicsList
class GmPhysicsFactory : public seal::PluginFactory<G4VUserPhysicsList *()>
{
public:
    static GmPhysicsFactory *get (void);

private:
    GmPhysicsFactory (void);
    static GmPhysicsFactory s_instance;
};


#define DEFINE_GAMOS_PHYSICS(type) \
  DEFINE_SEAL_PLUGIN(GmPhysicsFactory, type,#type)

#endif // GmPhysicsFactory_H
