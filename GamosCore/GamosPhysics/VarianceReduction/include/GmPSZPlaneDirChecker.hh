//
// Class Description
//
// This class checks if the track direction is pointing inside a Z plane

#ifndef GmPSZPlaneDirChecker_h 
#define GmPSZPlaneDirChecker_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4Track;

class GmPSZPlaneDirChecker 
{
  public: // with description
  GmPSZPlaneDirChecker();

  public: 
  //  destructor 
  virtual ~GmPSZPlaneDirChecker(){};

  public:
  virtual G4bool IsInPlane( const G4ThreeVector dir, const G4ThreeVector pos );
  virtual G4bool IsInPlane( const G4Track* track );

private:
  G4double theWeight;
  G4double thePlaneX, thePlaneY, thePlaneZ;
  G4int iUseFOI;
};

#endif
