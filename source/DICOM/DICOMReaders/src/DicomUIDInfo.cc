#include "DicomUIDInfo.hh"

#include "dcmtk/dcmdata/dcdeftag.h"

DicomUIDInfo::DicomUIDInfo(DcmDataset* dset) : DicomVReader(DRM_OTHER)
{
  theInfos[DCM_MediaStorageSOPClassUID] = G4String(Read1DataStr(dset, DCM_MediaStorageSOPClassUID,false).c_str()); //=RTDoseStorage
  theInfos[DCM_MediaStorageSOPInstanceUID] = G4String(Read1DataStr(dset, DCM_MediaStorageSOPInstanceUID,false).c_str());  //GAMOS
  //- theInfos[DCM_TransferSyntaxUID] = G4String(Read1DataStr(dset, DCM_TransferSyntaxUID,false).c_str());
  theInfos[DCM_ImplementationClassUID] = G4String(Read1DataStr(dset, DCM_ImplementationClassUID,false).c_str());
  theInfos[DCM_ImplementationVersionName] = G4String(Read1DataStr(dset, DCM_ImplementationVersionName,false).c_str()); 
  theInfos[DCM_PrivateInformationCreatorUID] = G4String(Read1DataStr(dset, DCM_PrivateInformationCreatorUID,false).c_str());
  theInfos[DCM_PrivateInformation] = G4String(Read1DataStr(dset, DCM_PrivateInformation,false).c_str());
  theInfos[DCM_SpecificCharacterSet] = G4String(Read1DataStr(dset, DCM_SpecificCharacterSet,false).c_str()); 
  //(0008,0012) DA [20260516]                               #   8, 1 InstanceCreationDate
  //(0008,0013) TM [115147.000000]                          #  14, 1 InstanceCreationTime
  //  theInfos[DCM_SOPClassUID] = G4String(Read1DataStr(dset, DCM_SOPClassUID,false).c_str()); // = RTDoseStorage
  theInfos[DCM_SOPInstanceUID] = G4String(Read1DataStr(dset, DCM_SOPInstanceUID,false).c_str()); // GAMOS = MediaStorageSOPInstanceUID
  // (0008,0020) DA (no value available)                     #   0, 0 StudyDate   //whatever it corresponds
  // (0008,0021) DA [20260516]                               #   8, 1 SeriesDate
  // (0008,0023) DA [20260516]                               #   8, 1 ContentDate
  // (0008,0030) TM (no value available)                     #   0, 0 StudyTime
  // (0008,0031) TM [115147.000000]                          #  14, 1 SeriesTime
  // (0008,0033) TM [115147.000000]                          #  14, 1 ContentTime
  // (0008,0050) SH (no value available)                     #   0, 0 AccessionNumber
  theInfos[DCM_Modality] = G4String(Read1DataStr(dset, DCM_Modality,false).c_str()); //="RTDOSE"
  theInfos[DCM_Manufacturer] = G4String(Read1DataStr(dset, DCM_Manufacturer,false).c_str()); // ="GAMOS"
  theInfos[DCM_InstitutionName] = G4String(Read1DataStr(dset, DCM_InstitutionName,false).c_str()); // ="CIEMAT"
  // (0008,0090) PN (no value available)                     #   0, 0 ReferringPhysicianName
  theInfos[DCM_SeriesDescription] = G4String(Read1DataStr(dset, DCM_SeriesDescription,false).c_str());
  // (0008,1070) PN (no value available)                     #   0, 0 OperatorsName
  theInfos[DCM_ManufacturerModelName] = G4String(Read1DataStr(dset, DCM_ManufacturerModelName,false).c_str()); // ="GAMOS"
  theInfos[DCM_PatientName] = G4String(Read1DataStr(dset, DCM_PatientName,false).c_str());
  theInfos[DCM_PatientID] = G4String(Read1DataStr(dset, DCM_PatientID,false).c_str());
  theInfos[DCM_PatientBirthDate] = G4String(Read1DataStr(dset, DCM_PatientBirthDate,false).c_str());
  theInfos[DCM_PatientSex] = G4String(Read1DataStr(dset, DCM_PatientSex,false).c_str());
  // (0018,0050) DS [1]                                      #   2, 1 SliceThickness
  theInfos[DCM_SoftwareVersions] = G4String(Read1DataStr(dset, DCM_SoftwareVersions,false).c_str());  //="GAMOS.7.0
  theInfos[DCM_StudyInstanceUID] = G4String(Read1DataStr(dset, DCM_StudyInstanceUID,false).c_str());
  theInfos[DCM_SeriesInstanceUID] = G4String(Read1DataStr(dset, DCM_SeriesInstanceUID,false).c_str());
  // (0020,0010) SH (no value available)                     #   0, 0 StudyID
  theInfos[DCM_SeriesNumber] = G4String(Read1DataStr(dset, DCM_SeriesNumber,false).c_str()); //="1"
  theInfos[DCM_InstanceNumber] = G4String(Read1DataStr(dset, DCM_InstanceNumber,false).c_str()); //GAMOS 1:EFFECTIVE, 2: PHYSICAL 3: GAMOS
  // (0020,0032) DS [-201.5\-253.0971\42.5]                  #  22, 3 ImagePositionPatient
  // (0020,0037) DS [1\0\0\0\1\0]                            #  12, 6 ImageOrientationPatient
  theInfos[DCM_FrameOfReferenceUID] = G4String(Read1DataStr(dset, DCM_FrameOfReferenceUID,false).c_str());
  // (0020,1040) LO (no value available)                     #   0, 0 PositionReferenceIndicator
  theInfos[DCM_SamplesPerPixel] = G4String(Read1DataStr(dset, DCM_SamplesPerPixel,false).c_str());  //="1"
  theInfos[DCM_PhotometricInterpretation] = G4String(Read1DataStr(dset, DCM_PhotometricInterpretation,false).c_str());
  // (0028,0008) IS [410]                                    #   4, 1 NumberOfFrames
  theInfos[DCM_FrameIncrementPointer] = G4String(Read1DataStr(dset, DCM_FrameIncrementPointer,false).c_str());
  // (0028,0010) US 506                                      #   2, 1 Rows
  // (0028,0011) US 404                                      #   2, 1 Columns
  // (0028,0030) DS [1\1]                                    #   4, 2 PixelSpacing
  // (0028,0100) US 16                                       #   2, 1 BitsAllocated
  // (0028,0101) US 16                                       #   2, 1 BitsStored
  // (0028,0102) US 15                                       #   2, 1 HighBit
  // (0028,0103) US 0                                        #   2, 1 PixelRepresentation
  // (3004,0002) CS [GY]                                     #   2, 1 DoseUnits
  theInfos[DCM_DoseType] = G4String(Read1DataStr(dset, DCM_DoseType,false).c_str()); // ="GAMOS"
  // (3004,0006) LO (no value available)                     #   0, 0 DoseComment
  theInfos[DCM_DoseSummationType] = G4String(Read1DataStr(dset, DCM_DoseSummationType,false).c_str());
  theInfos[DCM_GridFrameOffsetVector] = G4String(Read1DataStr(dset, DCM_GridFrameOffsetVector,false).c_str()); 
  // (3004,000e) DS [0.0009620191]                           #  12, 1 DoseGridScaling
  theInfos[DCM_TissueHeterogeneityCorrection] = G4String(Read1DataStr(dset, DCM_TissueHeterogeneityCorrection,false).c_str()); //="IMAGE\ROI_OVERRIDE"

  theInfos[DCM_ReferencedSOPClassUID] = G4String(Read1DataStrFromSequence(dset, DCM_ReferencedRTPlanSequence, DCM_ReferencedSOPClassUID,false).c_str()); //=RTDoseStorage
  theInfos[DCM_ReferencedSOPInstanceUID] = G4String(Read1DataStrFromSequence(dset, DCM_ReferencedRTPlanSequence, DCM_ReferencedSOPInstanceUID,false).c_str());  

  /*
     (300c,0002) SQ (Sequence with explicit length #=1)      #  98, 1 ReferencedRTPlanSequence
     (fffe,e000) na (Item with explicit length #=2)          #  90, 1 Item
     (0008,1150) UI =RTIonPlanStorage                        #  30, 1 ReferencedSOPClassUID
     (0008,1155) UI [1.2.752.243.1.1.20260516115147208.3700.42416] #  44, 1 ReferencedSOPInstanceUID
     (fffe,e00d) na (ItemDelimitationItem for re-encoding)   #   0, 0 ItemDelimitationItem
   */
  // (7fe0,0010) OW 0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000... # 167627680, 1 PixelData
    
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

