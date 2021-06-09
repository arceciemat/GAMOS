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
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4double theUnit;
  G4String theUnitName;
};

#endif
