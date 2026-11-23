#ifndef GmPSPrinterHistos_hh
#define GmPSPrinterHistos_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

class GmPSPrinterHistos : 
  public GmVPSPrinter,
  public GmVHistoBuilder
{
public: // with description
  GmPSPrinterHistos(G4String);
  virtual ~GmPSPrinterHistos(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4bool b2D;
  G4String theNameX;
  G4int theNBinsX;
  G4double theMinX;
  G4double theMaxX;
  G4double theOffsetX;
  G4String theNameY;
  G4int theNBinsY;
  G4double theMinY;
  G4double theMaxY;
  G4double theOffsetY;
  G4int theNShift;
  G4bool bHistoEachRow;
};

#endif
