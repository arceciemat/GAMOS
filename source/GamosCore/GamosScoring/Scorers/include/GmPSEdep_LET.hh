
#ifndef GmPSEdep_LET_h
#define GmPSEdep_LET_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSEdep_LET : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSEdep_LET(G4String name);
      virtual ~GmPSEdep_LET();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  private:
};
#endif
