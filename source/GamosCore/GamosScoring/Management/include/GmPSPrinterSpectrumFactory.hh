#ifndef GmPSPrinterSpectrumFactory_H
#define GmPSPrinterSpectrumFactory_H

#include "GmVPSPrinterSpectrum.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"
#include <vector>

/// Plug-in factory for objects of type GmVPSPrinterSpectrum
class GmPSPrinterSpectrumFactory : public seal::PluginFactory<GmVPSPrinterSpectrum *(G4String)>
{
public:
    static GmPSPrinterSpectrumFactory *get (void);

private:
    GmPSPrinterSpectrumFactory (void);
    static GmPSPrinterSpectrumFactory s_instance;
};

#define DEFINE_GAMOS_SCORER_SPECTRUM_PRINTER(type) \
  DEFINE_SEAL_PLUGIN(GmPSPrinterSpectrumFactory, type,#type)

#endif // GmPSPrinterSpectrumFactory_H
