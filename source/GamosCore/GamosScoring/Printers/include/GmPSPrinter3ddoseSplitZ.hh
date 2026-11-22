#ifndef GmPSPrinter3ddoseSplitZ_hh
#define GmPSPrinter3ddoseSplitZ_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
class G4PhantomParameterisation;

// class description:
//
// This class fill a dose file in 3ddose (DOSXYZnrc) format

class GmPSPrinter3ddoseSplitZ : public GmVPSPrinter, GmIOtextMgr
{
public: // with description
  GmPSPrinter3ddoseSplitZ(G4String);
  virtual ~GmPSPrinter3ddoseSplitZ(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  void Write3ddoseHeader(unsigned int iz);
  void Write3ddose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer, unsigned int iz );

private:
  G4PhantomParameterisation* thePhantomParam;

};

#endif
