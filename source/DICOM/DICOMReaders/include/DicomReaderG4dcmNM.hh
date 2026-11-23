#ifndef DicomReaderG4dcmNM__HH
#define DicomReaderG4dcmNM__HH

class DicomVImage;

#include <map>
#include "globals.hh"
#include "DicomVReaderImage.hh"

class DicomReaderG4dcmNM : public DicomVReaderImage
{ 
public:
  DicomReaderG4dcmNM(G4String fName, G4String name = "");
  ~DicomReaderG4dcmNM(){};

public:
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

};

#endif
  
