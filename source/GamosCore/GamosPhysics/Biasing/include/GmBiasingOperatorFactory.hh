#ifndef GmBiasingOperatorFactory_H
#define GmBiasingOperatorFactory_H

#include "GmVBiasingOperator.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"


/// Plug-in factory for objects of type GmVBiasingOperator
class GmBiasingOperatorFactory : public seal::PluginFactory<GmVBiasingOperator *(G4String)>
{
public:
    static GmBiasingOperatorFactory *get (void);

private:
    GmBiasingOperatorFactory (void);
    static GmBiasingOperatorFactory s_instance;
};


#define DEFINE_GAMOS_BIASINGOPERATOR(type) \
  DEFINE_SEAL_PLUGIN(GmBiasingOperatorFactory, type,#type)

#endif // GmBiasingOperatorFactory_H
