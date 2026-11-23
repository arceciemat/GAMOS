///////////////////////////////////////////////////////////////////////////////
// File: GmVMovement.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmVMovement_h
#define GmVMovement_h 1

#include "globals.hh"
#include <vector>
class G4VPhysicalVolume;
#include "G4ThreeVector.hh"

enum GmMovementType{ AfterTime, AfterNEvents };

class GmVMovement 
{
public:
  GmVMovement(){};
  GmVMovement(std::vector<G4String>& words, GmMovementType mt );
  ~GmVMovement();
  
  virtual G4bool IsNewMovement( G4double );
  virtual G4bool Move() = 0;

  void InitialiseLastValue();

  GmMovementType GetType() const{
    return theType; }
  G4double GetLastValue() const {
    return theLastValue;}
  
protected:
  std::vector<G4VPhysicalVolume*> theVolumes;
  GmMovementType theType; 
  G4double theValue;
  G4ThreeVector theAxis;
  G4double theInterval;
  G4double theOffset;
  G4int theNIntervals;
  G4int theNMoved;

  G4double theLastValue; // last value of time or nevents when a movement was made
};

#endif
