#ifndef GmDigitizerFactory_H
#define GmDigitizerFactory_H

#include "GmVDigitizer.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVDigitizer
class GmDigitizerFactory : public seal::PluginFactory<GmVDigitizer *()>
{
public:
    static GmDigitizerFactory *get (void);

private:
    GmDigitizerFactory (void);
    static GmDigitizerFactory s_instance;
};


#define DEFINE_GAMOS_DIGITIZER(type) \
  DEFINE_SEAL_PLUGIN(GmDigitizerFactory, type,#type)

#endif // GmDigitizerFactory_H
