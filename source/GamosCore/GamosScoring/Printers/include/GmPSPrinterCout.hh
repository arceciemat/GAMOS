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

private:

};

#endif
