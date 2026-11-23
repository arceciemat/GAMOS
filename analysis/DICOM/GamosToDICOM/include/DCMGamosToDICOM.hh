#ifndef DCMGamosToDICOM__HH
#define DCMGamosToDICOM__HH

class GmSqdose;
class DicomParameterMgr;
class DicomReaderMgr;
class DicomMgr;
class DicomUIDInfo;
#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "dcmtk/dcmdata/dcdatset.h"

class DCMGamosToDICOM
{
public:
  DCMGamosToDICOM();
  ~DCMGamosToDICOM(){};

    void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ConvertImages();
  
private:
  void fillImageToCT( DicomVImage* image, DcmDataset* dataset, DIModality modality, G4int iFirstSlice, G4int iLastSlice );
  OFCondition addElementToDataset(OFCondition &result,
				  DcmItem *dataset,
				  DcmElement *element,
				  const OFString &vm,
				  const OFString &type,
				  const char *moduleName);

  OFBool checkElementValue(DcmElement &element,
			   const OFString &vm,
				   const OFString &type,
       			   const OFCondition &searchCond,
			   const char *moduleName);
  
  virtual void PrintHelp();

private:
  void ReadConversion( G4String fileName );

  DicomVImage* theInputImage;
  G4String theExeName;
  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomUIDInfo* theDicomUIDInfo;
  G4bool bUIDfromRT;
  G4String thePatientPosition;
  DicomMgr* theDicomMgr;
  std::map<G4double,G4int> theDensityToHU;
  G4String theOutputFN;
  G4double theSmallestValue;
  G4double theBiggestValue;
  G4bool bSaveEachZSlice;
  G4String theSuffix;
  
};

#endif
  
  
