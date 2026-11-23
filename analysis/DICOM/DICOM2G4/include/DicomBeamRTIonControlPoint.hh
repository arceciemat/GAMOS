#ifndef DicomBeamRTIonControlPoint__HH
#define DicomBeamRTIonControlPoint__HH

#include <vector>
#include <iostream>
#include <map>
#include "dcmtk/dcmrt/seq/drticps.h"    // for DRTIonControlPointSequence
class DicomBeamDevicePos;

#include "DicomVBeamRTControlPoint.hh"
class DicomBeamRTIonControlPoint : public DicomVBeamRTControlPoint
{ 
public:
  DicomBeamRTIonControlPoint(DRTIonControlPointSequence::Item cpItem, DicomBeamRTIonControlPoint* point0 );
  ~DicomBeamRTIonControlPoint(){};

  void SetParam( std::string name,  DicomBeamRTIonControlPoint* point0 );

public:
  virtual void DumpToFile( std::ofstream& out );

private:
  OFVector<Float32> theSpotPos;
  OFVector<Float32> theSpotWei;

};

#endif  

  
