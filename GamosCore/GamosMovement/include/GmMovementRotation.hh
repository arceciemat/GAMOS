///////////////////////////////////////////////////////////////////////////////
// File: GmMovementRotation.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementRotation_h
#define GmMovementRotation_h 1

#include "globals.hh"
#include <vector>
#include "GmVMovement.hh"

class GmMovementRotation :public GmVMovement
{
public:
  GmMovementRotation(std::vector<G4String>& words, GmMovementType mt );
  ~GmMovementRotation();
  
  virtual G4bool Move();
  
};

#endif
