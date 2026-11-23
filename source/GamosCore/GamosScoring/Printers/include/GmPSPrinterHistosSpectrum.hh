#ifndef GmPSPrinterHistosSpectrum_hh
#define GmPSPrinterHistosSpectrum_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinterSpectrum.hh"

class GmPSPrinterHistosSpectrum : public GmVPSPrinterSpectrum,
  public GmVHistoBuilder
{
public: // with description
  GmPSPrinterHistosSpectrum(G4String);
  virtual ~GmPSPrinterHistosSpectrum(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual G4bool IsSpectrum();
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4String theName;
  G4int theNBins;
  G4double theMin;
  G4double theMax;
};

#endif
