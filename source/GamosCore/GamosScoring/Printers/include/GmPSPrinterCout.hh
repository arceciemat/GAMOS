#ifndef GmPSPrinterCout_hh
#define GmPSPrinterCout_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"

class GmPSPrinterCout : public GmVPSPrinter
{
public: // with description
  GmPSPrinterCout(G4String);
  virtual ~GmPSPrinterCout(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4double theUnit;
  G4String theUnitName;
};

#endif
