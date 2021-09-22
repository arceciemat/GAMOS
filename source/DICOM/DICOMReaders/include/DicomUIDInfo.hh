#ifndef DicomUIDInfo__HH
#define DicomUIDInfo__HH

#include "globals.hh"
#include "dcmtk/dcmdata/dcdatset.h"
#include "DicomVReader.hh"
#include <map>

class DicomUIDInfo : public DicomVReader
{
public:
  DicomUIDInfo(DcmDataset* dset);
  ~DicomUIDInfo(){};

  void AddInfo(DcmTagKey tagKey, G4String value);
  
  G4String GetInfo(DcmTagKey tagKey, G4bool bMustExist = false) const;
  std::map<DcmTagKey,G4String> GetInfos() {
    return theInfos;
  }
  
private:

  std::map<DcmTagKey,G4String> theInfos;

};

#endif
  
