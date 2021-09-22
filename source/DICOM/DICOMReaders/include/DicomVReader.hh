#ifndef DicomVReader__HH
#define DicomVReader__HH
#include "dcmtk/dcmdata/dcfilefo.h"

#include "globals.hh"
#include <vector>

class DcmDataset;
class DicomReaderMgr;

enum DRModality{ DRM_CT, DRM_NM, DRM_RTStruct, DRM_RTDose, DRM_RTPlan, DRM_RTIonPlan, DRM_G4dcmCT, DRM_G4dcmNM, DRM_G4dcmDose, DRM_Sqdose, DRM_TextLines, DRM_RTImage, DRM_Interfile, DRM_OTHER };

class DicomVReader 
{
public:
  DicomVReader(DRModality mod);
  DicomVReader(DcmDataset* dset, DRModality mod);
  virtual ~DicomVReader();

  void SetFileName( G4String fName ) {
    theFileName = fName;
  }
  G4String GetFileName() const {
    return theFileName;
  }
  DRModality GetModality() const {
    return theModality;
  }
  static G4String GetModalityStr(DRModality mod);
  G4String GetName() const {
    return theName;
  }

  virtual void ReadData();

  DcmDataset * GetDataset() const {
    return theDataset;
  }

protected:

  virtual std::vector<G4double> Read1Data( DcmDataset * dset, DcmTagKey tagKey, G4int nData, G4bool bExists = true );
  virtual OFString Read1DataStr( DcmDataset * dset, DcmTagKey tagKey, G4bool bExists = true );

  DcmDataset * theDataset;

protected:
  DicomReaderMgr* theReaderMgr;
  G4String theFileName;
  G4String theName;
  DRModality theModality;
};

#endif
  
