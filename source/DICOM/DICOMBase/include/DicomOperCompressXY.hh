#ifndef DicomOperCompressXY__HH
#define DicomOperCompressXY__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperCompressXY : public DicomVOperator
{ 
public:
  DicomOperCompressXY( G4int iCompress = 1, G4int iOrder = INT_MAX, G4String name = "DicomOperCompressXY");
  ~DicomOperCompressXY(){};

public:
  virtual void Operate( DicomVImage* );
  
private:
  G4int theCompression;
};

#endif
  
