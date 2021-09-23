#ifndef GmPSPrinterTextFile_hh
#define GmPSPrinterTextFile_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"

class GmPSPrinterTextFile : 
  public GmVPSPrinter,
  public GmIOtextMgr
{
public: // with description
  GmPSPrinterTextFile(G4String);
  virtual ~GmPSPrinterTextFile(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:

};

#endif
