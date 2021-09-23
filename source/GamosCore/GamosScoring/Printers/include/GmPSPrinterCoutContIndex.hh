#ifndef GmPSPrinterCoutContIndex_hh
#define GmPSPrinterCoutContIndex_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"

class GmPSPrinterCoutContIndex : public GmVPSPrinter
{
public: // with description
  GmPSPrinterCoutContIndex(G4String);
  virtual ~GmPSPrinterCoutContIndex(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  G4int theFirstIndex;
  G4int theLastIndex;
};

#endif
