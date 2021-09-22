#ifndef DicomReaderSqdose__HH
#define DicomReaderSqdose__HH

class DicomVImage;

#include <map>
#include "globals.hh"
#include "DicomVReaderImage.hh"
class GmSqdose;

class DicomReaderSqdose : public DicomVReaderImage
{ 
public:
  DicomReaderSqdose(G4String fName, G4String name = "");
  ~DicomReaderSqdose(){};
  
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

  DicomVImage* GetDoseImage() const {
    return theDicomImage;
  }
  DicomVImage* GetDoseSqImage() const {
    return theDoseSqImage;
  }
  DicomVImage* GetDoseErrImage() const {
    return theDoseErrImage;
  }
  GmSqdose* GetSqdose() {
    return theSqdose;
  }

protected:
  GmSqdose* theSqdose;

  DicomVImage* theDoseSqImage;
  DicomVImage* theDoseErrImage;
};



#endif
  
