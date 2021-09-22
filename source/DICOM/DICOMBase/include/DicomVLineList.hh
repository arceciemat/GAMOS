#ifndef DicomVLineList__HH
#define DicomVLineList__HH

#include "G4ThreeVector.hh"
#include "DicomVLine.hh"
class DicomVImage;
class DicomVLineSet;

#include <vector>

class DicomVLineList
{ 
public:
  DicomVLineList(DPOrientation ori);
  DicomVLineList( G4String name, size_t num, DPOrientation ori );
  virtual ~DicomVLineList();

public:
  void AddLine( DicomVLine* line );
    
  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name ) {
    theName = name;
  }
  size_t GetNumber() const {
    return theNumber;
  }
  void SetNumber( size_t num ) {
    theNumber = num;
  }
  DPOrientation GetOrientation() const {
    return theOrientation;
  }
  void SetOrientation( DPOrientation ori, G4bool bChildren );
  
  std::vector<DicomVLine*> GetLines() const {
    return theLines;
  }
  void RotateXY180( G4double angle, G4ThreeVector rotPoint );
  void SetLineSet( DicomVLineSet* ls );

protected:
  std::vector<DicomVLine*> theLines;
  G4String theName;
  size_t theNumber;
  DPOrientation theOrientation;
  DicomVLineSet* theLineSet;
};

#endif
  
