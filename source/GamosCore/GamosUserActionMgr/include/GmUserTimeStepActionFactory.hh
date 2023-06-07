#ifndef GmUserTimeStepActionFactory_H
#define GmUserTimeStepActionFactory_H

#include "G4UserTimeStepAction.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type GmUserAction
class GmUserTimeStepActionFactory : public seal::PluginFactory<G4UserTimeStepAction* ()>
{
public:
    static GmUserTimeStepActionFactory *get (void);

private:
    GmUserTimeStepActionFactory (void);
    static GmUserTimeStepActionFactory s_instance;
};

#define DEFINE_GAMOS_USERTIMESTEP_ACTION(type) \
  DEFINE_SEAL_PLUGIN(GmUserTimeStepActionFactory, type,#type)

#define DEFINE_GAMOSUSERTIMESTEPACTION(type) \
  DEFINE_SEAL_PLUGIN(GmUserTimeStepActionFactory, type,#type)

#endif // GmUserTimeStepActionFactory_H
