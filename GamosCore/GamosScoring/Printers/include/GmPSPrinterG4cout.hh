#ifndef GmPSPrinterG4cout_hh
#define GmPSPrinterG4cout_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"

class GmPSPrinterG4cout : public GmVPSPrinter
{
public: // with description
  GmPSPrinterG4cout(G4String);
  virtual ~GmPSPrinterG4cout(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4double theUnit;
  G4String theUnitName;
};

#endif
