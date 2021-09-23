#ifndef GmPSPrinterFactory_H
#define GmPSPrinterFactory_H

#include "GmVPSPrinter.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"
#include <vector>

/// Plug-in factory for objects of type GmVPSPrinter
class GmPSPrinterFactory : public seal::PluginFactory<GmVPSPrinter *(G4String)>
{
public:
    static GmPSPrinterFactory *get (void);

private:
    GmPSPrinterFactory (void);
    static GmPSPrinterFactory s_instance;
};

#define DEFINE_GAMOS_SCORER_PRINTER(type) \
  DEFINE_SEAL_PLUGIN(GmPSPrinterFactory, type,#type)

#endif // GmPSPrinterFactory_H
