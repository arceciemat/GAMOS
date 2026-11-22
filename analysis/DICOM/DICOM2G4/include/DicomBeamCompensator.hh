#ifndef DicomBeamCompensator__HH
#define DicomBeamCompensator__HH

#include "dcmtk/dcmrt/seq/drtcos.h"  // DRTCompensatorSequence
#include "dcmtk/dcmrt/seq/drtircs.h"  //DRTIonRangeCompensatorSequence
#include "globals.hh"
#include <vector>
#include <iostream>
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"
 
class DicomBeamCompensator : public DicomVObjectWithParams
{ 
public:
  DicomBeamCompensator(DRTCompensatorSequence::Item bcompItem);
  DicomBeamCompensator(DRTIonRangeCompensatorSequence::Item bcompItem);
  ~DicomBeamCompensator(){};

public:

  void DumpToFile( std::ofstream& out );

private:
  Sint32 theCompensatorColumns;
  Sint32 theCompensatorRows;

};

#endif  
  
