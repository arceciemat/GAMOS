///////////////////////////////////////////////////////////////////////////////
// File: GmMovementDisplacement.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementDisplacement_h
#define GmMovementDisplacement_h 1

#include "globals.hh"
#include <vector>
#include "GmVMovement.hh"

class GmMovementDisplacement :public GmVMovement
{
public:
  GmMovementDisplacement(std::vector<G4String>& words, GmMovementType mt );
  ~GmMovementDisplacement();
  
  virtual G4bool Move();
  
};

#endif
