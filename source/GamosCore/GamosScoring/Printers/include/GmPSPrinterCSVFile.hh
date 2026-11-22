#ifndef GmPSPrinterCSVFile_hh
#define GmPSPrinterCSVFile_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"

class GmPSPrinterCSVFile : 
  public GmVPSPrinter,
  public GmIOtextMgr
{
public: // with description
  GmPSPrinterCSVFile(G4String);
  virtual ~GmPSPrinterCSVFile(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:

  G4bool bPrintHeader;
};

#endif
