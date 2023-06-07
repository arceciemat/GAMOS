
#ifndef GmPSDose_LET_EDep_h
#define GmPSDose_LET_EDep_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSDose_LET_EDep : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSDose_LET_EDep(G4String name);
      virtual ~GmPSDose_LET_EDep();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  private:
};
#endif
