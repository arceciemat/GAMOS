#include "DicomReaderInterfile.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfile.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfileHeader.hh"

#include <fstream>
#include <map>

//-----------------------------------------------------------------------------
DicomReaderInterfile::DicomReaderInterfile(G4String fName, G4String name ) : DicomVReaderImage(DRM_Interfile)
{
  theFileName = fName;
  theHeaderFileName = fName;
  theName = name;
  //  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theModality = DRM_Interfile;

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderInterfile::DicomReaderInterfile " << fName << " " << name << " " << DicomVReader::GetModalityStr(theModality) << G4endl;
#endif
  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
void DicomReaderInterfile::ReadHeaderAndPixels(G4bool bReadPixelData)
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderInterfile::ReadHeaderAndPixels " << G4endl;
#endif

  ReadHeader();

  if( !bReadPixelData ) {
  //    G4cout << " bReadPixelData " << bReadPixelData << G4endl; //GDEB
    return;
  }

  
  ReadPixelData();
}

//----------------------------------------------------------------------------
void DicomReaderInterfile::ReadHeader()
{
  // https://www.gsp.com/cgi-bin/man.cgi?section=4&topic=m-intf

  std::map<int,std::string> axisIds;
  axisIds[1] = "x";
  axisIds[2] = "y";
  axisIds[3] = "z";

  std::pair<int,std::string> idSVal;
  std::pair<int,double> idDVal;
  std::string sVal;
  std::string thePatientOrientation = "head_in";
  std::string thePatientRotation = "supine";
  thePatientPosition = "HFS";
  std::string theByteOrder;
  theRescaleSlope = 1.;
  theRescaleIntercept = 0;
  const G4int NMAXLIN = 10000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters
  theInterfileHeader = new GmInterfileHeader();
  theInterfileHeader->Read(theHeaderFileName);
  //  G4double theOffsetX = 0, theOffsetY = 0., theOffsetZ = 0.;
  theNoVoxelsX = theInterfileHeader->GetNoVoxelsX();
  theNoVoxelsY = theInterfileHeader->GetNoVoxelsY();
  theNoVoxelsZ = theInterfileHeader->GetNoVoxelsZ();
  theVoxelDimX = theInterfileHeader->GetVoxelDimX();
  theVoxelDimY = theInterfileHeader->GetVoxelDimY();
  theVoxelDimZ = theInterfileHeader->GetVoxelDimZ();
  theMinX = theInterfileHeader->GetMinX();
  theMinY = theInterfileHeader->GetMinY();
  theMinZ = theInterfileHeader->GetMinZ();
  theMaxX = theMinX+theVoxelDimX*theNoVoxelsX;
  theMaxY = theMinY+theVoxelDimY*theNoVoxelsY;
  theMaxZ = theMinZ+theVoxelDimZ*theNoVoxelsZ;
  theNBytesPerPixel = theInterfileHeader->GetNoBytesPerPixel(); // TO BE IMPLEMENTED
  theNumberFormat = theInterfileHeader->GetNumberFormat();
  theDataFileName = theInterfileHeader->GetDataFileName();
  
  //        signed integer|unsigned integer
  //        |long float|short float|bit|ASCII
  /*
  //      theByteOrder = getSVal(stemp); // TO BE IMPLEMENTED
  } else if( stemp.find("data compression") != std::string::npos ) {
  G4Exception("DicomReaderInterfile::ReadHeader",
  "",
  FatalException,
  G4String("!!!! data compression is not supported: "+sVal).c_str());
  
  } else if( stemp.find("patient orientation") != std::string::npos ) {
  thePatientOrientation = getSVal(stemp);
  } else if( stemp.find("patient rotation") != std::string::npos ) {
  thePatientRotation = getSVal(stemp);
  } else if( stemp.find("rescale slope") != std::string::npos ) {
  theRescaleSlope = getDVal(stemp);
  } else if( stemp.find("rescale intercept") != std::string::npos ) {
  theRescaleIntercept = getDVal(stemp);
  } else if( stemp.find("name of data file") != std::string::npos ) {
  theDataFileName = getSVal(stemp);
  }
  G4cout << " MINY = " << theMinY << " = " <<  -theVoxelDimY << " * " << theNoVoxelsY/2. << " + "<< theOffsetY<< G4endl; //GDEB
  if( thePatientOrientation == "head_in" ) {
    if( thePatientRotation == "supine" ) {
      thePatientPosition = "HFS";
    } else if( thePatientRotation == "prone" ) {
      thePatientPosition = "HFS";
    } else { // "other"
      G4Exception("DicomReaderInterfile::ReadHeader",
		  "",
		  JustWarning,
		  G4String("!!!! patient rotation is not supported: "+thePatientRotation+" PatientPosition will be set to HFS").c_str()); 
    }
  } else if( thePatientOrientation == "feet_in" ) {
    if( thePatientRotation == "supine" ) {
      thePatientPosition = "HFS";
    } else if( thePatientRotation == "prone" ) {
      thePatientPosition = "HFS";
    } else { // "other"
      G4Exception("DicomReaderInterfile::ReadHeader",
		  "",
		  JustWarning,
		  G4String("!!!! patient rotation is not supported: "+thePatientRotation+" PatientPosition will be set to HFS").c_str()); 
    }
  } else { // "other
    G4Exception("DicomReaderInterfile::ReadHeader",
		"",
		JustWarning,
		("!!!! patient orientation is not supported: "+thePatientOrientation+" PatientPosition will be set to HFS").c_str()); 
  }

  Print(std::cout);
  */
  
}

 //----------------------------------------------------------------------------
std::pair<int,std::string> DicomReaderInterfile::getIdSVal(std::string stemp)
{
  //  std::remove( stemp.begin(), stemp.end(), '\r');
  if( stemp[stemp.length()-1] == '\r' ) {
    stemp = stemp.substr(0,stemp.length()-1);
  }
  size_t ifn1 = stemp.find("[");
  size_t ifn2 = stemp.find("]");
  std::string sid = stemp.substr(ifn1+1,ifn2-ifn1-1);
  //  G4cout << " ifn1 " << ifn1 << " ifn2 " << ifn2 << " " << sid <<  G4endl; //GDEB
  ifn1 = stemp.find(":=");
  std::string sval;
  sval = GmGenUtils::rtrim(stemp.substr(ifn1+3,stemp.length()));

       // suppress windows line feed
  std::pair<int,std::string> idSVal(GmGenUtils::GetInt(sid),sval);
  //  G4cout << ifn1 << " DicomReaderInterfile::getIdVal( Z1Z" << sid << "Z2Z" << sval << "Z3Z" << G4endl; //GDEB
  return idSVal;
}

//----------------------------------------------------------------------------
std::pair<int,double> DicomReaderInterfile::getIdDVal(std::string stemp)
{
  //  std::remove( stemp.begin(), stemp.end(), '\r');
  if( stemp[stemp.length()-1] == '\r' ) {
    stemp = stemp.substr(0,stemp.length()-1);
  }
  size_t ifn1 = stemp.find("[");
  size_t ifn2 = stemp.find("]");
  std::string sid = stemp.substr(ifn1+1,ifn2-ifn1-1);
  //  G4cout << " ifn1 " << ifn1 << " ifn2 " << ifn2 << " " << sid <<  G4endl; //GDEB
  ifn1 = stemp.find(":=");
  std::string sval;
  if( stemp[ifn1-1] == ' ' ) {
    sval = stemp.substr(ifn1+3,stemp.length()-ifn1-3);
  } else {
    sval = stemp.substr(ifn1+2,stemp.length()-ifn1-2);
  }
  
  // suppress windows line feed
  std::pair<int,double> idDVal(GmGenUtils::GetInt(sid),GmGenUtils::GetValue(sval));
  //  G4cout << ifn1 << " DicomReaderInterfile::getIdVal( Z1Z" << sid << "Z2Z" << sval << "Z3Z" << G4endl; //GDEB
  return idDVal;
}


//----------------------------------------------------------------------------
std::string DicomReaderInterfile::getSVal(std::string stemp)
{
  //  std::remove( stemp.begin(), stemp.end(), '\r');
  if( stemp[stemp.length()-1] == '\r' ) {
    stemp = stemp.substr(0,stemp.length()-1);
  }
  size_t ifn1 = stemp.find(":=");

  std::string sval = stemp.substr(ifn1+2,stemp.length());
  if( sval[0] == ' ' ) sval = sval.substr(1,sval.length());
  
  return sval;
}

//----------------------------------------------------------------------------
double DicomReaderInterfile::getDVal(std::string stemp)
{
  //  std::remove( stemp.begin(), stemp.end(), '\r');
  if( stemp[stemp.length()-1] == '\r' ) {
    stemp = stemp.substr(0,stemp.length()-1);
  }
  size_t ifn1 = stemp.find(":=");

  std::string sval = stemp.substr(ifn1+3,stemp.length());
  
  return GmGenUtils::GetValue(sval);
}
 
//----------------------------------------------------------------------------
void DicomReaderInterfile::ReadPixelData()
{
  GmInterfile* interfile = new GmInterfile();
  interfile->SetHeader(theInterfileHeader);
  interfile->Read(theDataFileName);

  size_t nVoxels = theInterfileHeader->GetNoVoxels();
  std::vector<float> DataRead = interfile->GetData();
  theVoxelData = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    theVoxelData->push_back(DataRead[ii]*theRescaleSlope + theRescaleIntercept);
     //    G4cout << " DicomReaderInterfile DATA " << ii << " : " << DataRead[ii]*theRescaleSlope + theRescaleIntercept << " " << DataRead[ii]<<"*"<<theRescaleSlope<<"+"<<theRescaleIntercept << G4endl; //GDEB
  }
  theDicomImage = CreateImage("Interfile", DIM_Interfile, true, true );
  
}

