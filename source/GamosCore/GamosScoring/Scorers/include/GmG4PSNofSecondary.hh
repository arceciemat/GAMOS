#ifndef GmG4PSNofSecondary_h
#define GmG4PSNofSecondary_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include "G4ParticleTable.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Number of particles
// generated in the geometry.
// 
// 
///////////////////////////////////////////////////////////////////////////////


class GmG4PSNofSecondary : public GmVPrimitiveScorer
{
 
 public: // with description
      GmG4PSNofSecondary(G4String name);

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public:
      virtual ~GmG4PSNofSecondary();


};



#endif
