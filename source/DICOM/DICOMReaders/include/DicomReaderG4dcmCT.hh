#ifndef DicomReaderG4dcmCT__HH
#define DicomReaderG4dcmCT__HH

class DicomVImage;

#include <map>
#include "globals.hh"
#include "DicomVReaderImage.hh"
#include "DICOM/DICOMBase/include/DicomVImageStr.hh"

class DicomReaderG4dcmCT : public DicomVReaderImage
{ 
public:
  DicomReaderG4dcmCT(G4String fName, G4String name = "");
  ~DicomReaderG4dcmCT(){};
  
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

  std::map<G4int,G4String> GetMaterialNames() const {
    return theMaterialNames;
  }
  G4String GetMaterialName(G4int id) const;
  void AddMaterialName(G4int mateId, G4String mateName);

  std::map<G4int,G4String> GetStructNames() const {
    return theStructNames;
  }
  G4String GetStructName(G4int id) const;
  DicomVImage* GetMateIDImage() const {
    return theMateIDImage;
  }
  DicomVImage* GetMateDensImage() const {
    return  theMateDensImage;
  }
  DicomVImageStr* GetStructIDImage() const {
    return theStructIDImage;
  }
  G4int GetStructID( G4String stName );
  G4bool HasStructID() const {
    return (theStructIDImage!=0);
  }
  void DumpHeaderToTextFile(std::ofstream& fout);

  void DumpStructureList(std::ofstream& fout);

protected:
  std::map<G4int,G4String> theMaterialNames;
  std::map<G4int,G4String> theStructNames;
  DicomVImage* theMateIDImage;
  DicomVImage* theMateDensImage;
  DicomVImageStr* theStructIDImage;
};

#endif
  
