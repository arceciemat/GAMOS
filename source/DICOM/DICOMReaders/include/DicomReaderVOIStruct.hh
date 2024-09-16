#ifndef DicomReaderVOIStruct__HH
#define DicomReaderVOIStruct__HH
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <iostream>

class DicomReaderVOIStruct : public DicomReaderRTStruct
{ 
public:
  DicomReaderVOIStruct(G4String fileName );
  ~DicomReaderVOIStruct(){};

public:
  virtual void ReadData(); // fill theROIs
private:
  G4bool JumpLine( size_t nLines );
  void CheckLine(size_t iword, G4String text, G4bool bFullText = true );
  void PrintLine( std::ostream& out );
    
private:
  GmFileIn theFileIn;

  std::vector<G4String> wl;

};

#endif
  
