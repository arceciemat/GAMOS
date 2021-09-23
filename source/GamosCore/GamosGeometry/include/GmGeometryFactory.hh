#ifndef GmGeometryFactory_H
#define GmGeometryFactory_H

#include "G4VUserDetectorConstruction.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type G4VUserDetectorConstruction
class GmGeometryFactory : public seal::PluginFactory<G4VUserDetectorConstruction *()>
{
public:
    static GmGeometryFactory *get (void);

private:
    GmGeometryFactory (void);
    static GmGeometryFactory s_instance;
};

#define DEFINE_GAMOS_GEOMETRY(type) \
  DEFINE_SEAL_PLUGIN(GmGeometryFactory, type,#type)

#define DEFINE_GAMOSGEOMETRY(type) \
  DEFINE_SEAL_PLUGIN(GmGeometryFactory, type,#type)

#endif // GmGeometryFactory_H
