#ifndef DicomReaderRTStruct__HH
#define DicomReaderRTStruct__HH
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@ @@@@@ NUMBER OF ReferenceSequences 1  (NOT USED)
//@@ FrameOfReferenceUID 1.3.12.2.1107.5.1.4.69815.30000016020908073587600000040
//@@ @@@@ NUMBER OF ReferenceStudySequences 1
//@@ @@@ NUMBER OF SeriesSequences 1
//@@ @@ NUMBER OF ContourImageSequences 144

//@@ @@@@@ NUMBER OF ROISequences 17  -> Build DicomROI
//@@  ROI: 1 REFERENCIA SEMIAUTOMATIC
//@@  ROI: 2 CENTRO SEMIAUTOMATIC
//@@
//@@@@@@@ NUMBER OF ROIContourSequences 17 (SAME AS N ROISeq's)
//@@ ROIDisplayColor 255
//@@ @@@@ NUMBER OF ContourSequences 1 -> Assoc. to ROI 1
//@@ @@@ NUMBER OF ContourImageSequences 1
//@@  ContourImageSequence C= 1.2.840.10008.5.1.4.1.1.2 I= 1.3.12.2.1107.5.1.4.69815.30000016020912092170700000075
//@@ @@@ GeomType POINT NPOINTS 1
//@@ (-4.42795,-262.55,-1142.69)
//@@ @@@@ NUMBER OF ContourSequences 173 -> Assoc. to ROI 2
//@@ @@@ NUMBER OF ContourImageSequences 1
//@@  ContourImageSequence C= 1.2.840.10008.5.1.4.1.1.2 I= 1.3.12.2.1107.5.1.4.69815.30000016020912092170700000021
//@@ @@@ GeomType CLOSED_PLANAR NPOINTS 89
//@@ (-32.7154,-172.714,-980.698)
//@@ (-31.7389,-173.691,-980.698)
//@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "DICOM/DICOMReaders/include/DicomVReader.hh"
#include "DicomROI.hh"
#include <map>

class DcmDataSet;
class DicomPolygonSet;
#include "DICOM/DICOMBase/include/DicomVImageStr.hh"

class DicomReaderRTStruct : public DicomVReader
{ 
public:
  DicomReaderRTStruct(DcmDataset* dset);
  DicomReaderRTStruct(G4String fileName );
  DicomReaderRTStruct();
  ~DicomReaderRTStruct(){};

public:
  std::map<G4int,DicomROI*> GetROIs() const {
    return theROIs;
  }
  virtual void ReadData(); // fill theROIs

  DicomPolygonSet* GetPolygonSet() const {
    return thePolygonSet;
  }

  void FindVoxelsInXY( DicomVImageStr* imageStruct, DicomPolygonSet* polygonSet );
  DicomVImage* GetStructImage() const {
    return theStructImage;
  }    
  
protected:
  void BuildPolygonSet();
  std::map<G4int,DicomROI*> theROIs;

  DicomPolygonSet* thePolygonSet;

  DicomVImageStr* theStructImage;
};

#endif
  
