#ifndef GmSensDetFactory_H
#define GmSensDetFactory_H

#include "G4VSensitiveDetector.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type G4VSensitiveDetector
class GmSensDetFactory : public seal::PluginFactory<G4VSensitiveDetector *(const G4String& )>
{
public:
    static GmSensDetFactory *get (void);

private:
    GmSensDetFactory (void);
    static GmSensDetFactory s_instance;
};

#define DEFINE_GAMOS_SENSDET(type) \
  DEFINE_SEAL_PLUGIN(GmSensDetFactory, type,#type)

#endif // GmSensDetFactory_H
