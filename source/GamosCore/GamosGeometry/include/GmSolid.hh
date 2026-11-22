#ifndef GmSolid_h
#define GmSolid_h

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
class G4VSolid;
class G4tgrSolid;

class GmSolid { 

 public:
  GmSolid(){};
  GmSolid( const G4VSolid*);
  GmSolid( const G4tgrSolid*);
  //  GmSolid( G4TouchableHistory* g4touch );
  ~GmSolid();

// get methods
 public:
  friend std::ostream& operator<<(std::ostream&, const GmSolid&);

  const G4String& GetName() const {
    return theName; }
  G4Point3D GetRandomPoint() const;

private: 
  //! The name 
  G4String theName; 
  G4String theType; 
  std::vector<G4double> theParams;

}; 

#endif
