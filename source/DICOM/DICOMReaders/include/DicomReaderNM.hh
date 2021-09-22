#ifndef DicomReaderNM__HH
#define DicomReaderNM__HH

class DicomVImage;

#include "globals.hh"
#include "DicomVReaderImage.hh"

class DicomReaderNM : public DicomVReaderImage
{ 
public:
  DicomReaderNM(DcmDataset* dset);
  DicomReaderNM(G4String fileName );
  DicomReaderNM();
  virtual ~DicomReaderNM(){};
  
  virtual void DICOMCreateImages();

protected:
  DicomVImage* theSqdoseImage;
  DicomVImage* theSqdoseSqImage;
  DicomVImage* theSqdoseErrImage;
};

#endif
  
