#ifndef DicomVLineSet__HH
#define DicomVLineSet__HH

class DicomVImage;
#include "DicomVLineList.hh"
#include <vector>
#include <map>
#include "G4ThreeVector.hh"

class DicomVLineSet
{ 
public:
  DicomVLineSet( G4String name, DPOrientation ori);
  virtual ~DicomVLineSet();

  std::vector<DicomVLineList*> GetLineLists() const {
    return theLineLists;
  }

  DicomVLineList* GetLineList(size_t num ) const;

  void SetLineLists( std::vector<DicomVLineList*> lineLists );
  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name ) {
    theName = name;
  }
  DPOrientation GetOrientation() const {
    return theOrientation;
  }
  void SetOrientation( DPOrientation ori , G4bool bChildren );
  
  void AddLineList(DicomVLineList*);
  std::map<int, G4String> GetLineListNames();
  void RotateXY180( G4double angle, G4ThreeVector rotPoint );
  G4bool HasBeenRotatedXY180() const {
    return bHasBeenRotatedXY180; }
  void SetHasBeenRotatedXY180( bool bhbr ) {
    bHasBeenRotatedXY180 = bhbr;
  }

protected:
  std::vector<DicomVLineList*> theLineLists;
  G4String theName;
  G4bool bHasBeenRotatedXY180;
  DPOrientation theOrientation;
};

#endif
  
