
#ifndef GmPSLET_h
#define GmPSLET_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSLET : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSLET(G4String name);
      virtual ~GmPSLET();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

};
#endif
