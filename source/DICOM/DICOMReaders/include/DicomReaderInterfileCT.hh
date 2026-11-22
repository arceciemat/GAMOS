#ifndef DicomReaderInterfileCT__HH
#define DicomReaderInterfileCT__HH

class DicomVImage;

#include "globals.hh"
#include "DicomReaderInterfile.hh"
class GmInterfileHeader;

class DicomReaderInterfileCT : public DicomReaderInterfile
{ 
public:
  DicomReaderInterfileCT(G4String fName, G4String name = "");
  ~DicomReaderInterfileCT(){};
  
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

  virtual void ReadPixelData();

protected:

};

#endif
  
