#ifndef DicomReaderRTDose__HH
#define DicomReaderRTDose__HH

class DicomVImage;

#include <map>
#include "globals.hh"
#include "DicomVReaderImage.hh"

class DicomReaderRTDose : public DicomVReaderImage
{ 
public:
  DicomReaderRTDose(DcmDataset* dset);
  DicomReaderRTDose(G4String fileName );
  DicomReaderRTDose();
  ~DicomReaderRTDose(){};
  
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );
  virtual void DICOMCreateImages();

  DicomVImage* GetDoseImage() const {
    return theDicomImage;
  }
  DicomVImage* GetDoseSqImage() const {
    return theDoseSqImage;
  }
  DicomVImage* GetDdoseErrImage() const {
    return theDoseErrImage;
  }

protected:
  DicomVImage* theDoseSqImage;
  DicomVImage* theDoseErrImage;
};

#endif
  
