#ifndef GmUserActionFactory_H
#define GmUserActionFactory_H

#include "GmUserAction.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type GmUserAction
class GmUserActionFactory : public seal::PluginFactory<GmUserAction* ()>
{
public:
    static GmUserActionFactory *get (void);

private:
    GmUserActionFactory (void);
    static GmUserActionFactory s_instance;
};

#define DEFINE_GAMOS_USER_ACTION(type) \
  DEFINE_SEAL_PLUGIN(GmUserActionFactory, type,#type)

#define DEFINE_GAMOSUSERACTION(type) \
  DEFINE_SEAL_PLUGIN(GmUserActionFactory, type,#type)

#endif // GmUserActionFactory_H
