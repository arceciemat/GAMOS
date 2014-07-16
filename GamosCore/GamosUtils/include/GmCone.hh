#ifndef GmCone_HH
#define GmCone_HH
#include "G4ThreeVector.hh"
#include <vector>
#include <iostream>
class GmLine;


#include "G4RotationMatrix.hh"

class GmCone 
{

public:  // with description

  // Constructor & destructor.
  GmCone(){};
  GmCone(const G4ThreeVector& pnt, const G4ThreeVector& dir, const G4double angle );
  ~GmCone(){};

  friend std::ostream& operator<<(std::ostream&, const GmCone&);

  G4double GetDist( const G4ThreeVector& ) const;

  std::vector<G4ThreeVector> Intersect( const GmLine& line ) const;

  // Get methods
  G4ThreeVector GetPoint() const{ return thePoint; }
  G4ThreeVector GetDir() const{ return theDir; }
  G4double GetAngle() const{ return theAngle; }
  
protected:
  
  G4ThreeVector thePoint;
  G4ThreeVector theDir;
  G4double theAngle;


  G4RotationMatrix* BuildG4RotMatrixFrom9( const G4ThreeVector& colx, const G4ThreeVector& coly, const G4ThreeVector& colz ) const;
 
};

#endif
