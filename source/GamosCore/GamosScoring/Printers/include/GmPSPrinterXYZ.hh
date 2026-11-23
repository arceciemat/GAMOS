#ifndef GmPSPrinterXYZ_hh
#define GmPSPrinterXYZ_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
class G4PhantomParameterisation;

// class description:
//
// This class fill a dose file in 3ddose (DOSXYZnrc) format

class GmPSPrinterXYZ : public GmVPSPrinter, GmIOtextMgr
{
public: // with description
  GmPSPrinterXYZ(G4String);
  virtual ~GmPSPrinterXYZ(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:
  void WriteHeader();
  void WriteValues( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  G4double theUnit;
  G4String theUnitName;
  G4PhantomParameterisation* thePhantomParam;

};

#endif
