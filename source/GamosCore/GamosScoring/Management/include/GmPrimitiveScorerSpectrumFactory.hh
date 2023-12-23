#ifndef GmPrimitiveScorerSpectrumFactory_H
#define GmPrimitiveScorerSpectrumFactory_H

#include "GmVPrimitiveScorerSpectrum.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/PluginFactory.h"
#include <vector>

/// Plug-in factory for objects of type GmVPrimitiveScorerSpectrum
//class GmPrimitiveScorerSpectrumFactory : public seal::PluginFactory<G4VPrimitiveScorerSpectrum *(const G4String& )>
class GmPrimitiveScorerSpectrumFactory : public seal::PluginFactory<GmVPrimitiveScorerSpectrum *(G4String)>
{
public:
    static GmPrimitiveScorerSpectrumFactory *get (void);

private:
    GmPrimitiveScorerSpectrumFactory (void);
    static GmPrimitiveScorerSpectrumFactory s_instance;
};

#define DEFINE_GAMOS_SCORER_SPECTRUM(type) \
  DEFINE_SEAL_PLUGIN(GmPrimitiveScorerSpectrumFactory, type,#type)

#endif // GmPrimitiveScorerSpectrumFactory_H
