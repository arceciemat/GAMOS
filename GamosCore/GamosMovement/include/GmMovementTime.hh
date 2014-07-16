///////////////////////////////////////////////////////////////////////////////
// File: GmMovementTime.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementTime_h
#define GmMovementTime_h 1

#include "globals.hh"
#include <vector>
class G4VPhysicalVolume;
#include "GmMovement.hh"

class GmMovementTime : public GmMovement
{
public:
  GmMovementTime(std::vector<G4String>& words);
  ~GmMovementTime();
  
  virtual G4bool IsNewMovement( G4double );
  virtual G4bool Move() = 0;
  
protected:
  G4double theLastValue;
};

#endif
