#ifndef DicomVBeamRTControlPoint__HH
#define DicomVBeamRTControlPoint__HH

#include <vector>
#include <iostream>
#include <map>
#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
class DicomBeamDevicePos;

#include "G4ThreeVector.hh"
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"

class DicomVBeamRTControlPoint : public DicomVObjectWithParams
{ 
public:
  DicomVBeamRTControlPoint();
  ~DicomVBeamRTControlPoint(){};

public:
  void AddDevice(DicomBeamDevicePos* dbd){
    theDevices.push_back(dbd);
  }

  virtual void DumpToFile( std::ofstream& out ) = 0;
  
protected:
  std::vector<DicomBeamDevicePos *> theDevices;

};

#endif  

  
