#ifndef DicomBeamDevice__HH
#define DicomBeamDevice__HH

#include "dcmtk/dcmdata/dcfilefo.h"

#include <vector>
#include <iostream>
class DicomBeamDeviceDevice;
class DicomBeamDeviceControlPoint;

#include "dcmtk/dcmrt/seq/drtblds1.h" // for BeamLimitingDeviceSequenceInRTBeamsModule
#include "dcmtk/dcmrt/seq/drtbldps.h"  // for BeamLimitingDevicePositionSequence

class DicomBeamDevice 
{ 
public:
  DicomBeamDevice(DRTBeamLimitingDeviceSequenceInRTBeamsModule::Item bldsItem);
  DicomBeamDevice(DRTBeamLimitingDevicePositionSequence::Item bldpsItem);
  ~DicomBeamDevice(){};

public:
  void SetSourceToBeamLimitingDeviceDistance(Float64 dat){
    theSourceToBeamLimitingDeviceDistance= dat;
  }
  Float64 GetSourceToBeamLimitingDeviceDistance() const {
    return theSourceToBeamLimitingDeviceDistance;
  }
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
  void Print( std::ostream& out );

private:
  OFString theType;
  Float64 theSourceToBeamLimitingDeviceDistance;
  Sint32  theNumberOfLeafJawPairs;
  std::vector<Float64> thePositionBoundaries;
};

#endif  
  
