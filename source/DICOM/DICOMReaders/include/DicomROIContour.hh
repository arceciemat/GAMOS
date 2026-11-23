#ifndef DicomROIContour__HH
#define DicomROIContour__HH

#include "dcmtk/dcmdata/dcfilefo.h"
#include "G4ThreeVector.hh"
#include <iostream>
#include <vector>

class DicomROIContour 
{ 
public:
  DicomROIContour();
  ~DicomROIContour(){};

public:
  void AddImageIUID( OFString ima ) {
    theImageIUIDs.push_back(ima);
  }
  OFString GetGeomType() const {
    return theGeomType;
  }
  void SetGeomType( OFString gt ) {
    theGeomType = gt;
  }
  std::vector<G4ThreeVector> GetPoints() const {
    return thePoints;
  }
  std::vector<G4ThreeVector> GetDirections() {
    return theDirections;
  }
  G4double GetZ();
  void SetData( std::vector<G4ThreeVector> data );

  void AddPoints( std::vector<G4ThreeVector> points );

  void Print(std::ostream& out);
  
private:
  std::vector<OFString> theImageIUIDs;
  // ?  std::vector<OFString> theImageCUIDs;
  OFString theGeomType;
  std::vector<G4ThreeVector> thePoints;
  std::vector<G4ThreeVector> theDirections;

};

#endif
  
