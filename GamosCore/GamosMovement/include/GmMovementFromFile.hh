///////////////////////////////////////////////////////////////////////////////
// File: GmMovementFromFile.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementFromFile_h
#define GmMovementFromFile_h 1

#include "globals.hh"
#include <vector>
#include "GmVMovement.hh"

struct MovementFF{
  G4double DispValue;
  G4ThreeVector DispAxis;
  G4double RotValue;
  G4ThreeVector RotAxis;
};

class GmMovementFromFile :public GmVMovement
{
public:
  GmMovementFromFile(std::vector<G4String> wl );
  ~GmMovementFromFile();
  
  virtual G4bool Move();

  void AddMovement(std::vector<G4String> wl );

private:
  std::vector<MovementFF> theMovements;
  
};

#endif
