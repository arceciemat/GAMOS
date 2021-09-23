
#ifndef GmG4PSTermination_h
#define GmG4PSTermination_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"


//////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Number of Steps in the cell.
// 
///////////////////////////////////////////////////////////////////////////////


class GmG4PSTermination : public GmVPrimitiveScorer
{
 
 public: // with description
      GmG4PSTermination(G4String name);

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public:
      virtual ~GmG4PSTermination();

};



#endif
