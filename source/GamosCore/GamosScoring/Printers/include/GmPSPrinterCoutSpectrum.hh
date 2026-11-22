#ifndef GmPSPrinterCoutSpectrum_hh
#define GmPSPrinterCoutSpectrum_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinterSpectrum.hh"

class GmPSPrinterCoutSpectrum : public GmVPSPrinterSpectrum
{
public: // with description
  GmPSPrinterCoutSpectrum(G4String);
  virtual ~GmPSPrinterCoutSpectrum(){ };
  
  virtual G4bool IsSpectrum();
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:

};

#endif
