#ifndef DicomVLine__HH
#define DicomVLine__HH

#include "G4ThreeVector.hh"
#include <vector>
class DicomVImage;
enum DPOrientation { DPOrientXY, DPOrientXZ, DPOrientYZ, DPOrientNone };
class DicomVLineList;

class DicomVLine
{ 
public:
  DicomVLine( G4String name, DPOrientation ori);
  DicomVLine( std::vector<G4ThreeVector> points, G4String name, DPOrientation ori);
  DicomVLine( std::vector<G4ThreeVector> points, std::vector<G4ThreeVector> dirs, G4String name, DPOrientation ori);

  virtual ~DicomVLine(){};
  void DetectOrientation(G4int order = 0);
  DPOrientation GetOrientation() const {
    return theOrientation;
  }
  void SetOrientation( DPOrientation ori );

  static G4String GetOrientationName(DPOrientation orient );

  std::vector<G4ThreeVector> GetPoints() const {
    return thePoints;
  }
  std::vector<G4ThreeVector> GetDirections() const {
    return theDirections;
  }
  void SetPoints( std::vector<G4ThreeVector> points );
  void SetDirections( std::vector<G4ThreeVector> dirs ) {
    theDirections = dirs;
  }

  G4ThreeVector GetPoint( size_t ipt );
  G4ThreeVector GetDirection( size_t idir );
  
  G4String GetName() const {
    return theName;
  }
  size_t GetNumber() const;
  G4double GetID() const;

  void BuildDirections();

  void SetLineList( DicomVLineList* pl );
  
  G4double GetPlanePosition() const {
    return thePlanePosition;
  }
  void RotateXY180( G4double angle, G4ThreeVector rotPoint );
  
protected:
  G4String theName;
  size_t theNumber;
  std::vector<G4ThreeVector> thePoints;
  std::vector<G4ThreeVector> theDirections;
  DPOrientation theOrientation; // Plane orientation
  G4double thePlanePosition; // Points are in a plane
  DicomVLineList* theLineList;
};

#endif
  
