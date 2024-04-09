#ifndef DicomLine__HH
#define DicomLine__HH

#include "DicomVLine.hh"
#include "G4ThreeVector.hh"
#include <map>
class DicomVImage;

class DicomLine : public DicomVLine
{ 
public:
  DicomLine( G4String name, DPOrientation ori );
  DicomLine( G4ThreeVector point, G4ThreeVector dir, G4String name, DPOrientation ori );
  
  ~DicomLine(){};

  std::map<G4double,G4double> FindValues( DicomVImage* image, G4double step = -DBL_MAX);
  std::map<G4double,G4double> FindValuesInVoxels( DicomVImage* image);
  std::map<G4double,G4double> FindValuesInSteps( DicomVImage* image, G4double step);
  G4ThreeVector GetPos0() const {
    return thePos0;
  }
  void SetPos0( G4ThreeVector lp0 );

private:
  G4ThreeVector thePos0; // position to mark 0 value in GetDistanceTo0()
  G4bool bUsePos0;
};

#endif
  
