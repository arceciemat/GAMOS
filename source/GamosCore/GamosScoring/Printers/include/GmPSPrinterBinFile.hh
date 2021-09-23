#ifndef GmPSPrinterBinFile_hh
#define GmPSPrinterBinFile_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

class GmPSPrinterBinFile : 
  public GmVPSPrinter,
  public GmIObinMgr
{
public: // with description
  GmPSPrinterBinFile(G4String);
  virtual ~GmPSPrinterBinFile(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:

};

#endif
