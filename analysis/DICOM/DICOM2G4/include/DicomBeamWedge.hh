#ifndef DicomBeamWedge__HH
#define DicomBeamWedge__HH

#include "dcmtk/dcmrt/seq/drtws.h"  //  DRTWedgeSequence
#include "dcmtk/dcmrt/seq/drtiws.h"  //  DRTIonWedgeSequence
#include "globals.hh"
#include <vector>
#include <iostream>
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"

class DicomBeamWedge : public DicomVObjectWithParams
{ 
public:
  DicomBeamWedge(DRTWedgeSequence::Item bcompItem);
  DicomBeamWedge(DRTIonWedgeSequence::Item bcompItem);
  ~DicomBeamWedge(){};

public:

  void DumpToFile( std::ofstream& out );
  
private:

};

#endif  
  
