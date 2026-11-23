
#ifndef GmPSDose_LET_h
#define GmPSDose_LET_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSDose_LET : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSDose_LET(G4String name);
      virtual ~GmPSDose_LET();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  private:
};
#endif
