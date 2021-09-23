#ifndef GmPSPrinter3ddose_hh
#define GmPSPrinter3ddose_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
class G4PhantomParameterisation;

// class description:
//
// This class fill a dose file in 3ddose (DOSXYZnrc) format

class GmPSPrinter3ddose : public GmVPSPrinter, GmIOtextMgr
{
public: // with description
  GmPSPrinter3ddose(G4String);
  virtual ~GmPSPrinter3ddose(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  void Write3ddoseHeader();
  void Write3ddose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  G4PhantomParameterisation* thePhantomParam;

};

#endif
