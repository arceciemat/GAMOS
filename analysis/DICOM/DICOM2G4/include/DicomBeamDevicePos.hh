#ifndef DicomBeamDevicePos__HH
#define DicomBeamDevicePos__HH

#include "dcmtk/dcmrt/seq/drtbldps.h"  //DRTBeamLimitingDeviceSequence
#include "DicomVBeamDevice.hh"

class DicomBeamDevicePos : public DicomVBeamDevice
{ 
public:
  DicomBeamDevicePos(DRTBeamLimitingDevicePositionSequence::Item bldpsItem);
  ~DicomBeamDevicePos(){};

public:
  void DumpToFile( std::ofstream& out );

  void Print( std::ostream& out );

private:
};

#endif  
  
