#ifndef DicomBeamRTControlPoint__HH
#define DicomBeamRTControlPoint__HH

#include <vector>
#include <iostream>
#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
class DicomBeamDevicePos;

#include "G4ThreeVector.hh"

#include "DicomVBeamRTControlPoint.hh"
class DicomBeamRTControlPoint : public DicomVBeamRTControlPoint
{ 
public:
  DicomBeamRTControlPoint(DRTControlPointSequence::Item cpItem, DicomBeamRTControlPoint* point0 );
  ~DicomBeamRTControlPoint(){};

  void SetParamFromPoint( std::string name,  DicomBeamRTControlPoint* point0 );
  
public:
  
  virtual void DumpToFile( std::ofstream& out );
  
private:

};

#endif  

  
