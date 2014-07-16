#ifndef GmLine_HH
#define GmLine_HH
#include "G4ThreeVector.hh"

class GmLine 
{

public:  // with description

  // Constructor & destructor.
  GmLine(){};
  GmLine(const G4ThreeVector& pnt, const G4ThreeVector& dir);
  ~GmLine(){};

  friend std::ostream& operator<<(std::ostream&, const GmLine&);

  G4double GetDist( const G4ThreeVector& ) const;

  // Get methods
  inline G4ThreeVector GetPoint() const{ return thePoint; }
  inline G4ThreeVector GetDir() const{ return theDir; }
  G4ThreeVector GetDistPoint( const G4ThreeVector& pt ) const;

private:
  
  G4ThreeVector thePoint;
  G4ThreeVector theDir;
 
};

#endif
