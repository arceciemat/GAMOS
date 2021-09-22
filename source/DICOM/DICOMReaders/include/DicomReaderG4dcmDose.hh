#ifndef DicomReaderG4dcmDose__HH
#define DicomReaderG4dcmDose__HH

class DicomVImage;

#include <map>
#include "globals.hh"
#include "DicomVReaderImage.hh"

class DicomReaderG4dcmDose : public DicomVReaderImage
{ 
public:
  DicomReaderG4dcmDose(G4String fName, G4String name = "");
  ~DicomReaderG4dcmDose(){};

public:
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

};

#endif
  
