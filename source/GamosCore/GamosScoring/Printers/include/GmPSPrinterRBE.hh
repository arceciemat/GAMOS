#ifndef GmPSPrinterRBE_hh
#define GmPSPrinterRBE_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
class G4PhantomParameterisation;
class GmSqdoseHeader;
class GmScoringMgr;
class GmVprimitiveScorer;
class GmVPSRBEPhenom;
// class description:

//
// This class fill a dose file with sum of weights and sum of weights square, to be able to mere dose files

class GmPSPrinterRBE : public GmVPSPrinter, public GmUserRunAction
{
public: // with description
  GmPSPrinterRBE(G4String);
  virtual ~GmPSPrinterRBE(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& );
  virtual void BeginOfRunAction(const G4Run* );
private:
  G4double GetDoseNorm();  

private:
  GmScoringMgr* theScoringMgr;
  GmVPSPrinter* thePrinter;
  GmVPSRBEPhenom* theRBEScorer;
  G4String theDoseScorerName;
  GmVPrimitiveScorer* theDoseScorer;
  G4String theLETScorerName;
  GmVPrimitiveScorer* theLETScorer;
};

#endif
