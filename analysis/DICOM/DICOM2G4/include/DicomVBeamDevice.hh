#ifndef DicomVBeamDevice__HH
#define DicomVBeamDevice__HH

#include "dcmtk/dcmdata/dcfilefo.h"

#include <vector>
#include <iostream>
class DicomBeamDevice;
class DicomBeamRTIonControlPoint;

class DicomVBeamDevice 
{ 
protected:
  DicomVBeamDevice();
  ~DicomVBeamDevice(){};

public:
  void SetNumberOfLeafJawPairs(Sint32 dat){ 
    theNumberOfLeafJawPairs= dat;
  }
  Sint32 GetNumberOfLeafJawPairs() const {
    return theNumberOfLeafJawPairs;
  }
  void SetType(OFString dat){ 
    theType = dat;
  }
  OFString GetType() const {
    return theType;
  }
  void AddPositionBoundary( Float64 dat ){
    thePositionBoundaries.push_back(dat);
  }
  Float64 GetPositionBoundary( size_t ii ) {
    return thePositionBoundaries[ii];
  }

  virtual void DumpToFile( std::ofstream& out ) = 0;

  virtual void Print( std::ostream& out ) = 0;

protected:
  OFString theType;
  Sint32  theNumberOfLeafJawPairs;
  std::vector<Float64> thePositionBoundaries;
};

#endif  
  
