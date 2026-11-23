#ifndef DCMSqToRT__HH
#define DCMSqToRT__HH

class GmSqdose;
class DicomParameterMgr;
class DicomReaderMgr;
class DicomUIDInfo;
#include "dcmtk/dcmdata/dcdatset.h"
#include "globals.hh"

class DCMSqToRT
{
public:
  DCMSqToRT();
  ~DCMSqToRT(){};

    void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ConvertImages();
  
private:
  
  void fillRTDoseFromSqdose( GmSqdose* sqdose, DcmDataset* dataset );
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
  
  OFCondition SetFromReference( DcmItem *dataset, const DcmTag &tag,
			       const OFString &vm,
			       const OFString &type,
			       const char *moduleName,
			       G4String elemType = "UniqId");
  
  virtual void PrintHelp();

private:
  G4String theSqDoseInFN = "";
  G4String theRTDoseOutFN = "";
  G4String theRTDoseReferenceFN = "";
  G4String theExeName;
  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomUIDInfo* theDicomUIDInfo;
  G4bool bUIDfromRT;
  G4String thePatientPosition;
};

#endif
  
  
