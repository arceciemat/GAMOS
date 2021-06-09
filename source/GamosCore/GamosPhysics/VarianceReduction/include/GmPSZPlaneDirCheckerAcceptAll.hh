//
// Class Description
//
// This class checks if the track direction is pointing inside a Z plane

#ifndef GmPSZPlaneDirCheckerAcceptAll_h 
#define GmPSZPlaneDirCheckerAcceptAll_h 1

#include "GmPSZPlaneDirChecker.hh"

class GmPSZPlaneDirCheckerAcceptAll : public GmPSZPlaneDirChecker
{
  public: // with description
  GmPSZPlaneDirCheckerAcceptAll();

  public: 
  //  destructor 
  virtual ~GmPSZPlaneDirCheckerAcceptAll(){};

  public:
  virtual G4bool IsInPlane( const G4ThreeVector dir, const G4ThreeVector pos );
  virtual G4bool IsInPlane( const G4Track* track );

};

#endif
