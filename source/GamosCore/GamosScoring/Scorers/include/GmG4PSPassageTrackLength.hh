#ifndef GmG4PSPassageTrackLength_h
#define GmG4PSPassageTrackLength_h 1

#include "GmG4PSTrackLength.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only track length.
//   The tracks which passed a geometry is taken into account.
// 
///////////////////////////////////////////////////////////////////////////////

class GmG4PSPassageTrackLength : public GmG4PSTrackLength
{
 
public: // with description
  GmG4PSPassageTrackLength(G4String name);
  virtual ~GmG4PSPassageTrackLength();
  
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      G4bool IsPassed(G4Step*);

  private:
      G4int fCurrentTrkID;
      G4double fTrackLength;

};

#endif
