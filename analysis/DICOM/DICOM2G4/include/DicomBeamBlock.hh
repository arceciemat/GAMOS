#ifndef DicomBeamBlock__HH
#define DicomBeamBlock__HH

#include "dcmtk/dcmrt/seq/drtbl2.h" // DRTBlockSequenceInRTBeamsModule
#include "dcmtk/dcmrt/seq/drtibls.h" // DRTIonBlockSequence

#include <vector>
#include <iostream>
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"

class DicomBeamBlock : public DicomVObjectWithParams
{ 
public:
  DicomBeamBlock(DRTBlockSequenceInRTBeamsModule::Item bcompItem);
  DicomBeamBlock(DRTIonBlockSequence::Item bcompItem);
  ~DicomBeamBlock(){};

public:

  void DumpToFile( std::ofstream& out );

private:
  OFVector<Float64> theBlockData;

};

#endif  
  
