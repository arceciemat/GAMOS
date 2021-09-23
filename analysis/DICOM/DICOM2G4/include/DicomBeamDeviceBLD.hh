#ifndef DicomBeamDevicePosRef__HH
#define DicomBeamDevicePosRef__HH

#include "DicomVBeamDevice.hh"
#include "dcmtk/dcmrt/seq/drtiblds.h"  //DRTIonBeamLimitingDeviceSequence
#include "dcmtk/dcmrt/seq/drtblds1.h"  //DRTBeamLimitingDeviceSequenceInRTBeamsModule

class DicomBeamDeviceBLD : public DicomVBeamDevice
{
public:
  DicomBeamDeviceBLD(DRTBeamLimitingDeviceSequenceInRTBeamsModule::Item bldsItem);
  DicomBeamDeviceBLD(DRTIonBeamLimitingDeviceSequence::Item bldsItem);
  ~DicomBeamDeviceBLD(){};

public:
  void SetSourceToBeamLimitingDeviceDistance(Float64 dat){
    theSourceToBeamLimitingDeviceDistance= dat;
  }
  Float64 GetSourceToBeamLimitingDeviceDistance() const {
    return theSourceToBeamLimitingDeviceDistance;
  }

  void DumpToFile( std::ofstream& out );

  void Print( std::ostream& out );

private:
  Float64 theSourceToBeamLimitingDeviceDistance;
};

#endif  
  
