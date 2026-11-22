#ifndef Dicom2G4FileMgr__HH
#define Dicom2G4FileMgr__HH
#include <vector>
#include <map>
#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "DICOM/DICOMReaders/include/DicomVReader.hh"
class DicomReaderCT;
class DicomReaderNM;
class DicomReaderRTDose;
class DicomReaderRTStruct;
class DicomReaderRTPlan;
class DicomReaderRTIonPlan;
class DicomReaderInterfile;
class DicomVReaderImage;
class DicomReaderMgr;
class DicomMgr;

class DcmDataset;

//typedef std::multimap<OFString,DicomVReader*> msd;
typedef std::map<G4double,DicomVReaderImage*> mdvri;

class Dicom2G4FileMgr 
{
public:
  static Dicom2G4FileMgr* GetInstance();
  ~Dicom2G4FileMgr(){};

private:
  Dicom2G4FileMgr();

public:
  void Convert( G4String fFileName );

  void SetCompression( G4String fComp );
  void AddFile( G4String fComp );
  void AddFileInterfile( G4String fComp );

  void ProcessFiles();
  void DumpToTextFile();
  
  G4String GetFileOutName() const {
    return theFileOutName;
  }

  void SetControlPointMetersets();
  
private:
  void CheckNColumns(std::vector<G4String> wl, size_t vsizeTh );

public:
  G4bool bFileOutByUser;

private:
  static Dicom2G4FileMgr* theInstance;

  G4String theFileOutName;
  mdvri theCTReaders; // ordered by Z (to be checked and merged)
  DicomReaderCT* theCTReaderAll = 0;
  DicomReaderNM* theNMReaderAll = 0;
  DicomReaderRTDose* theRTDoseReaderAll = 0;
  DicomReaderRTStruct* theRTStructReader = 0;
  DicomReaderRTPlan* theRTPlanReader = 0;
  DicomReaderRTIonPlan* theRTIonPlanReader = 0;
  DicomReaderInterfile* theIFReaderAll = 0;
  
  //  DicomVReaderImeDICOM* theCTFileAll;
  //  DicomVReaderImage* theNMFileAll;
  //  DicomVReaderImage* theRTDoseFileAll;
  G4int theStructureNCheck;
  
  DicomMgr* theDicomMgr;
  DicomReaderMgr* theReaderMgr;
  G4int fCompression;
  G4bool bIsInterfile = false; 
};

#endif
  
