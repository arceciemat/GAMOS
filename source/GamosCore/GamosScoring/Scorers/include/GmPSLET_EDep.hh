
#ifndef GmPSLET_EDep_h
#define GmPSLET_EDep_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSLET_EDep : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSLET_EDep(G4String name);
      virtual ~GmPSLET_EDep();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

};
#endif
