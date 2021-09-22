#include "DicomReaderInterfile.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

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
  std::ifstream fin(theHeaderFileName);
  if( !fin.is_open()) {
    G4Exception("DicomReaderInterfile::ReadHeader",
		  "",
		  FatalException,
		  (G4String("!!!! Input file does not exist: ")+theHeaderFileName).c_str());
  }
  G4double theOffsetX = 0, theOffsetY = 0., theOffsetZ = 0.;
  //  G4cout << " OPENING HEADER FILE " << theHeaderFileName << G4endl; //GDEB
  for( int ii = 0; ii < 100 ;ii++ ) {
    fin.getline( ltemp, NMAXLIN );
    if( fin.eof() ) break;
    std::string stemp(ltemp);
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) )G4cout << ii << " DicomReaderInterfile::ReadHeader " << stemp << G4endl;
#endif  
    if( stemp.find("matrix axis label [") != std::string::npos ) {
      idSVal = getIdSVal(stemp);
      if( axisIds[idSVal.first] != idSVal.second ) {
	G4Exception("DicomReaderInterfile",
		    "",
		    FatalException,
		    ("FILE "+theHeaderFileName+" !!! axis labels are not 1:x 2:y 3:z , but "+GmGenUtils::itoa(idSVal.first)+":"+idSVal.second+":").c_str());	
      }
    } else if( stemp.find("matrix size [") != std::string::npos ){
      idDVal = getIdDVal(stemp);
      if( idDVal.first == 1 ) {
	theNoVoxelsX = idDVal.second;
      } else if( idDVal.first == 2 ) {
	theNoVoxelsY = idDVal.second;
      } else if( idDVal.first == 3 ) {
	theNoVoxelsZ = idDVal.second;
      }
    } else if( stemp.find("scaling factor (mm/pixel) [") != std::string::npos ) {
      idDVal = getIdDVal(stemp);
      if( idDVal.first == 1 ) {
	theVoxelDimX = idDVal.second;
      } else if( idDVal.first == 2 ) {
	theVoxelDimY = idDVal.second;
      } else if( idDVal.first == 3 ) {
	theVoxelDimZ = idDVal.second;
      }      
    } else if( stemp.find("offset [") != std::string::npos ) {
      idDVal = getIdDVal(stemp);
      if( idDVal.first == 1 ) {
	theOffsetX = idDVal.second;
      } else if( idDVal.first == 2 ) {
	theOffsetY = idDVal.second;
      } else if( idDVal.first == 3 ) {
	theOffsetZ = idDVal.second;
      }
    } else if( stemp.find("imagedata byte order") != std::string::npos ) {
      theByteOrder = getSVal(stemp); // TO BE IMPLEMENTED
    } else if( stemp.find("number of bytes per pixel") != std::string::npos ) {
      theNBytesPerPixel = getDVal(stemp); // TO BE IMPLEMENTED
    } else if( stemp.find("data encode") != std::string::npos ) {
      sVal = getSVal(stemp);
      if( sVal != "none" )  {
	G4Exception("DicomReaderInterfile::ReadHeader",
		    "",
		    FatalException,
		    G4String("!!!! data encode is not supported: "+sVal).c_str());
      }
    } else if( stemp.find("number format") != std::string::npos ) {
      sVal = getSVal(stemp);
      if( sVal != "float" && sVal != "signed integer"
	  && sVal != "signed integer" )  {
	G4Exception("DicomReaderInterfile::ReadHeader",
		    "",
		    FatalException,
		    G4String("!!! number format not supported "+sVal).c_str());
      }
      //!number format := <ASCIIlist> unsigned integer
      //        signed integer|unsigned integer
      //        |long float|short float|bit|ASCII
    } else if( stemp.find("data compression") != std::string::npos ) {
      sVal = getSVal(stemp);
      if( sVal != "none" )  {
	G4Exception("DicomReaderInterfile::ReadHeader",
		    "",
		    FatalException,
		    G4String("!!!! data compression is not supported: "+sVal).c_str());
      }
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
  }
  //--- Figure is always centered at (0,0,0). Construct limits
  theMinX = -theVoxelDimX*theNoVoxelsX/2.+theOffsetX;
  theMaxX = theVoxelDimX*theNoVoxelsX/2.+theOffsetX;
  theMinY = -theVoxelDimY*theNoVoxelsY/2.+theOffsetY;
  theMaxY = theVoxelDimY*theNoVoxelsY/2.+theOffsetY;
  theMinZ = -theVoxelDimZ*theNoVoxelsZ/2.+theOffsetZ;
  theMaxZ = theVoxelDimZ*theNoVoxelsZ/2.+theOffsetZ;
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

  if( theDataFileName == "" ) {
    G4Exception("DicomReaderInterfile::ReadHeader",
		"",
		FatalException,
		"!!!! No name of data file tag found");
  }
  
  Print(std::cout);

  
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

  FILE* inputV=fopen((theDataFileName).c_str(), "rb");
  if( inputV == 0 ) {
    G4Exception("DicomReaderInterfile::ReadPixelData",
		"",
		FatalException,
		("File "+theDataFileName+" NOT FOUND").c_str());
  }
  size_t nVoxels = GetNoVoxels();
  float *fData;
  //theData = calloc( nVoxels,sizeof(float));
  fData = (float*) malloc(nVoxels*sizeof(float));
  
  fread(fData,1,nVoxels*theNBytesPerPixel, inputV);

  theVoxelData = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    theVoxelData->push_back(fData[ii]*theRescaleSlope + theRescaleIntercept);
  }
  theDicomImage = CreateImage("Interfile", DIM_Interfile, true, true );
  
}

