#include "DicomVReader.hh"
#include "DicomReaderMgr.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcpixel.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcpixseq.h"

//-----------------------------------------------------------------------------
DicomVReader::DicomVReader(DRModality mod)
{
  theReaderMgr = DicomReaderMgr::GetInstance();
  theReaderMgr->AddReader( this, mod );
  if( DicomVerb(infoVerb) ) {
    G4cout << " DicomVReader AddReader " << this->GetName() << " " << DicomVReader::GetModalityStr(mod) << G4endl;
  }
  theModality = mod;
}

//-----------------------------------------------------------------------------
DicomVReader::DicomVReader(DcmDataset* dset, DRModality mod) 
{
  
  theReaderMgr = DicomReaderMgr::GetInstance();
  theReaderMgr->AddReader( this, mod );
  if( DicomVerb(debugVerb) ) {
    G4cout << "a DicomVReader AddReader " << this->GetName() << " " << DicomVReader::GetModalityStr(mod) << G4endl;
  }
  theDataset = dset;
  theModality = mod;
}

//-----------------------------------------------------------------------------
DicomVReader::~DicomVReader()
{
  theReaderMgr->DeleteReader(this);
}

//-----------------------------------------------------------------------------
void DicomVReader::ReadData()
{
  G4Exception("DicomVReader::ReadData",
	      "",
	      FatalException,
	      "This method should not be called, please contact GAMOS authors");
}

//-----------------------------------------------------------------------------
std::vector<G4double> DicomVReader::Read1Data( DcmDataset * dset, DcmTagKey tagKey, G4int nData, G4bool bExists ) 
{
  std::vector<G4double> dataV;

  for(int ii=0; ii<nData; ++ii) {
    G4double data;
    Uint16 dataui;
    Sint16 datasi;
    // see  http://support.dcmtk.org/docs/classDcmItem.html for types
    if (dset->findAndGetFloat64(tagKey, data,ii).good() ) {
      //      G4cout <<"Read DCM Float64 (" << std::showbase // show the 0x prefix
      //	     << std::internal // fill between the prefix and the number
      //      << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec << ii << " = " << data << std::endl; 
      dataV.push_back(data);
    } else if (dset->findAndGetUint16(tagKey, dataui,ii).good() ) {
      //      if( DicomMgr::verbose >= debugVerb ) G4cout <<"Read DCM Uint16 (" << std::showbase // show the 0x prefix
      //	     << std::internal // fill between the prefix and the number
      //	     << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec << ii << " = " << dataui << std::endl; 
      dataV.push_back(dataui);
    } else if (dset->findAndGetSint16(tagKey, datasi,ii).good() ) {
      dataV.push_back(datasi);
    } else {
      if(bExists) {
	if( DicomVerb(debugVerb) ) {
	  G4cerr <<"ERROR  (" << std::showbase // show the 0x prefix
		 << std::internal // fill between the prefix and the number
		 << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec << ii << std::endl; 
	  G4Exception("DicomHandler::Read1Data",
		      "",
		      JustWarning,
		      ( std::to_string(data) +G4String(" Have not read (") + std::to_string(tagKey.getGroup())+","+std::to_string(tagKey.getElement())+")"+" : "+std::to_string(ii)).c_str());
	}
      }
    }
  }
  
  return dataV;
}


//-----------------------------------------------------------------------------
OFString DicomVReader::Read1DataStr( DcmDataset * dset, DcmTagKey tagKey, G4bool bExists )
{
  //  const char* data = "";
  OFString data;  
  // see  http://support.dcmtk.org/docs/classDcmItem.html for types
  //  G4cout << " Read1DataStr dset " << dset << G4endl; //GDEB
  if (dset->findAndGetOFString(tagKey, data).good() ) {
    //   G4cout <<"Read DCM String (" << std::showbase // show the 0x prefix
    //	     << std::internal // fill between the prefix and the number
    //	     << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec << " = " << data << std::endl; 
  } else {
    if(bExists) {
      if( DicomVerb(debugVerb) ) {
	G4cerr <<"ERROR  (" << std::showbase // show the 0x prefix
	       << std::internal // fill between the prefix and the number
	       << std::setfill('0') << std::hex << std::setw(4) << tagKey.getGroup() << "," << tagKey.getElement() << ") "<< std::dec << std::endl; 
	G4Exception("DicomHandler::Read1DataStr",
		    "",
		    JustWarning,
		    //		( G4String(data) + " Have not read (" + std::to_string(tagKey.getGroup())+","+std::to_string(tagKey.getElement())+")"+" : ").c_str());
		    (" Have not read (" + std::to_string(tagKey.getGroup())+","+std::to_string(tagKey.getElement())+")"+" : ").c_str());
      }
    }
  }

  return data.c_str();
}

//-----------------------------------------------------------------------------
G4String DicomVReader::GetModalityStr(DRModality mod)
{
  switch (mod) {
  case DRM_CT:
    return "DRM_CT";
  case DRM_NM:
    return "DRM_NM";
  case DRM_RTStruct:
    return "DRM_RTStruct";
  case DRM_RTDose:
    return "DRM_RTDose";
  case DRM_RTPlan:
    return "DRM_RTPlan";
  case DRM_RTIonPlan:
    return "DRM_RTIonPlan";
  case DRM_G4dcmCT:
    return "DRM_G4dcmCT";
  case DRM_G4dcmNM:
    return "DRM_G4dcmNM";
  case DRM_G4dcmDose:
    return "DRM_G4dcmDose";
  case DRM_Sqdose:
    return "DRM_Sqdose";
  case DRM_TextLines:
    return "DRM_TextLines";
  case DRM_RTImage:
    return "DRM_RTImage";
  case DRM_Interfile:
    return "DRM_Interfile";
  case DRM_OTHER:
    return "DRM_OTHER";
  }
  return "";
}
