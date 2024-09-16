#ifndef DicomLine__HH
#define DicomLine__HH

#include "DicomVLine.hh"
#include "G4ThreeVector.hh"
#include <map>
class DicomVImage;
class DicomVImageStr;

class DicomLine : public DicomVLine
{ 
public:
  DicomLine( G4String name, DPOrientation ori );
  DicomLine( G4ThreeVector point, G4ThreeVector dir, G4String name, DPOrientation ori );
  
  ~DicomLine(){};

  std::map<G4double,G4double> FindValues( DicomVImage* image, G4double step = -DBL_MAX);
  std::map<G4double,G4String> FindValuesStr( DicomVImageStr* image, G4double step = -DBL_MAX);
  std::map<G4double,G4double> FindValuesInVoxels( DicomVImage* image);
  std::map<G4double,G4double> FindValuesInSteps( DicomVImage* image, G4double step);
  std::map<G4double,G4String> FindValuesInVoxelsStr( DicomVImageStr* imageStr );
  std::map<G4double,G4String> FindValuesInStepsStr( DicomVImageStr* imageStr, G4double step);

private:
  G4int CheckOutOfPhantom( G4ThreeVector pos, G4ThreeVector dir, std::vector<G4double> fMinV,  std::vector<G4double> fMaxV, std::vector<G4String> fAxisNameV );

};

#endif
  
