#include "DicomUIDInfo.hh"

#include "dcmtk/dcmdata/dcdeftag.h"

DicomUIDInfo::DicomUIDInfo(DcmDataset* dset) : DicomVReader(DRM_OTHER)
{
  theInfos[DCM_MediaStorageSOPClassUID] = G4String(Read1DataStr(dset, DCM_MediaStorageSOPClassUID,false).c_str());
  theInfos[DCM_MediaStorageSOPInstanceUID] = G4String(Read1DataStr(dset, DCM_MediaStorageSOPInstanceUID,false).c_str());
  theInfos[DCM_TransferSyntaxUID] = G4String(Read1DataStr(dset, DCM_TransferSyntaxUID,false).c_str());
  theInfos[DCM_ImplementationClassUID] = G4String(Read1DataStr(dset, DCM_ImplementationClassUID,false).c_str());
   theInfos[DCM_InstanceCreatorUID] = G4String(Read1DataStr(dset, DCM_InstanceCreatorUID,false).c_str()); theInfos[DCM_SOPClassUID] = G4String(Read1DataStr(dset, DCM_SOPClassUID,false).c_str());
  theInfos[DCM_SOPInstanceUID] = G4String(Read1DataStr(dset, DCM_SOPInstanceUID,false).c_str());
  theInfos[DCM_StudyInstanceUID] = G4String(Read1DataStr(dset, DCM_StudyInstanceUID,false).c_str());
  theInfos[DCM_SeriesInstanceUID] = G4String(Read1DataStr(dset, DCM_SeriesInstanceUID,false).c_str());
  theInfos[DCM_FrameOfReferenceUID] = G4String(Read1DataStr(dset, DCM_FrameOfReferenceUID,false).c_str());
  //  theInfos[DCM_RelatedGeneralSOPClassUID] = G4String(Read1DataStr(dset, DCM_RelatedGeneralSOPClassUID,false).c_str());
  
}


void DicomUIDInfo::AddInfo(DcmTagKey tagKey, G4String value)
{
  theInfos[tagKey] = value;
}

G4String DicomUIDInfo::GetInfo(DcmTagKey tagKey, G4bool bMustExist) const
{
  std::map<DcmTagKey,G4String>::const_iterator ite = theInfos.find(tagKey);
  if( ite == theInfos.end() ) {
    if( bMustExist ) {
std::ostringstream stream;
     stream << std::internal // fill between the prefix and the number
	    << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec; 
     G4Exception("DicomUIDInfo::GetInfo",
		 "",
		  FatalException,
		  ("UID info does not exist " + stream.str()).c_str());
    } else {
      return "NONE";
    }
  }

  return ite->second;
  
}

