//ROTATIONS: http://dicom.nema.org/medical/dicom/2014c/output/chtml/part03/sect_C.31.3.html
// http://arxiv.org/pdf/1406.0014.pdf

#ifndef DicomReaderRTPlan__HH
#define DicomReaderRTPlan__HH

#include "DICOM/DICOMReaders/include/DicomVReader.hh"
#include "DICOM/DICOMBase/include/DicomVObjectWithParams.hh"
class DcmDataSet;
class DicomBeam;

class DicomReaderRTPlan : public DicomVReader, public DicomVObjectWithParams
{ 
public:
  DicomReaderRTPlan(DcmDataset* dset);
  ~DicomReaderRTPlan(){};

public:
  virtual void ReadData();
  void CheckData0(OFString title, Sint32 val);

  void DumpToFile();

  void SetControlPointMetersets();
  
private:
  std::vector<DicomBeam*> theBeams;
  Sint32 theNumberOfBeams;
};

#endif
  
  
