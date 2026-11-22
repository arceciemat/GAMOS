#ifndef GmPSPrinterSqdose_hh
#define GmPSPrinterSqdose_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
class G4PhantomParameterisation;
class GmSqdoseHeader;

// class description:
//
// This class fill a dose file with sum of weights and sum of weights square, to be able to mere dose files

class GmPSPrinterSqdose : public GmVPSPrinter, GmIObinMgr
{
public: // with description
  GmPSPrinterSqdose(G4String);
  virtual ~GmPSPrinterSqdose(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  GmSqdoseHeader* BuildSqdoseHeader();
  void WriteSqdose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  std::vector<G4PhantomParameterisation*> thePhantomParams;

};

#endif
