///////////////////////////////////////////////////////////////////////////////
// File: GmMovementNEvents.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementNEvents_h
#define GmMovementNEvents_h 1

#include "globals.hh"
#include <vector>
class G4VPhysicalVolume;
#include "GmVMovement.hh"

class GmMovementNEvents : public GmVMovement
{
public:
  GmMovementNEvents(std::vector<G4String>& words);
  ~GmMovementNEvents();
  
  virtual G4bool IsNewMovement( G4double );
  virtual G4bool Move() = 0;
  
protected:
  G4double theLastValue;
};

#endif
