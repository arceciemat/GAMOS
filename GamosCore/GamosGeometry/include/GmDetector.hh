#ifndef GmDetector_h
#define GmDetector_h

// Class that identifies a volume item ( = G4VTouchable with a format independent of G4 
// and some extra information)
// Pedro Arce
// 20.02.2001: Creation

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Point3D.hh"
class G4VPhysicalVolume;
class GmSolid;
class GmTouchable;
class G4tgrSolid;

class GmDetector { 

 public:
  GmDetector( GmSolid* sol, const G4ThreeVector& pos, const G4RotationMatrix& rotmat );
  GmDetector( GmSolid* sol, const G4Transform3D& transf );
  GmDetector( GmTouchable* so );

  //  GmDetector( G4TouchableHistory* g4touch );
  ~GmDetector();

// get methods
 public:
  const G4String& GetName() const {
    return theName; 
  }
  const G4ThreeVector GetPosition() const {
    return theTransform.getTranslation();
  }
  const G4RotationMatrix GetRotation() const { 
    return theTransform.getRotation();
}
  G4Point3D GetRandomPoint() const;
  G4Point3D GetCentrePoint() const;
  G4int GetID() const {
    return theID;
  }

private: 
  //! The name 
  G4String theName; 
  //! transformation 
  G4Transform3D theTransform;
  //! solid
  GmSolid* theSolid;
  G4int theID;
}; 

#endif
