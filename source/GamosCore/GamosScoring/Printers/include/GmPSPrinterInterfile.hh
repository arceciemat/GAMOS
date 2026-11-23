#ifndef GmPSPrinterInterfile_hh
#define GmPSPrinterInterfile_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
class G4PhantomParameterisation;
class GmInterfileHeader;

// class description:
//
// This class fill a dose file with sum of weights and sum of weights square, to be able to mere dose files

class GmPSPrinterInterfile : public GmVPSPrinter, GmIObinMgr
{
public: // with description
  GmPSPrinterInterfile(G4String);
  virtual ~GmPSPrinterInterfile(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  GmInterfileHeader* BuildInterfileHeader();
  void WriteInterfile( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  std::vector<G4PhantomParameterisation*> thePhantomParams;

};

#endif
