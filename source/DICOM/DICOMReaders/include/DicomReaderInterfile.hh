#ifndef DicomReaderInterfile__HH
#define DicomReaderInterfile__HH

class DicomVImage;

#include "globals.hh"
#include "DicomVReaderImage.hh"
class GmInterfileHeader;

class DicomReaderInterfile : public DicomVReaderImage
{ 
public:
  DicomReaderInterfile(G4String fName, G4String name = "");
  DicomReaderInterfile( DRModality mod );
  ~DicomReaderInterfile(){};
  
  virtual void ReadHeaderAndPixels( G4bool bReadPixelData = true );

  std::pair<int,std::string> getIdSVal(std::string stemp);
  std::pair<int,double> getIdDVal(std::string stemp);
  std::string getSVal(std::string stemp);
  double getDVal(std::string stemp);

  void ReadHeader();
  virtual void ReadPixelData();
  GmInterfileHeader* theInterfileHeader;

protected:

  std::string theHeaderFileName;
  std::string theDataFileName;
  int theNBytesPerPixel;
  std::string theNumberFormat;
};

#endif
  
