#include "GmInterfileHeader.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
GmInterfileHeader::GmInterfileHeader()
{
  theNoEvent = 1;
  theDataType = "test";
  theNoBytesPerPixel = 4;
  theNDimensions = 3;
  theNumberFormat = "float";
  theMatrixAxisLabels[1] = "x";
  theMatrixAxisLabels[2] = "y";
  theMatrixAxisLabels[3] = "z";
}

//-----------------------------------------------------------------------
G4bool GmInterfileHeader::operator==(const GmInterfileHeader& right) const
{
  G4bool equ = 1;
  if( theNoVoxelsX != right.GetNoVoxelsX() ||
      theNoVoxelsY != right.GetNoVoxelsY() ||
      theNoVoxelsZ != right.GetNoVoxelsZ() ||
      theVoxelDimX != right.GetVoxelDimX() ||
      theVoxelDimY != right.GetVoxelDimY() ||
      theVoxelDimZ != right.GetVoxelDimZ() ||
      theMinX != right.GetMinX() ||
      theMinY != right.GetMinY() ||
      theMinZ != right.GetMinZ() ){
    equ = 0;
  }
  return equ;

}

//-----------------------------------------------------------------------
G4bool GmInterfileHeader::operator!=(const GmInterfileHeader& right) const
{
  return !( *this == right );
}
//-----------------------------------------------------------------------
void GmInterfileHeader::Read( G4String fileName )
{
  std::ifstream* fin = new std::ifstream(fileName);
  Read(fin);
}

//-----------------------------------------------------------------------
void GmInterfileHeader::Read( std::ifstream* fin )
{
  theNoEvent = 1;
  const G4int NMAXLIN = 10000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters
  std::pair<int,std::string> idSVal;
  std::pair<int,double> idDVal;
  std::string thePatientOrientation = "head_in";
  std::string thePatientRotation = "supine";
  thePatientPosition = "HFS";
  
  for( int ii = 0; ii < 100 ;ii++ ) {
    fin->getline( ltemp, NMAXLIN );
    if( fin->eof() ) break;
    G4cout << "GmInterfileHeader::Read ltemp " << ltemp << G4endl; //GDEB
    std::string stemp(ltemp);

#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(-infoVerb) )G4cout << ii << " GmInterfileHeader::Read " << stemp << G4endl;
#endif  
    if( stemp.find("matrix axis label [") != std::string::npos ) {
      idSVal = getIdSVal(stemp);
      if( theMatrixAxisLabels[idSVal.first] != idSVal.second ) {
	G4Exception("GmInterfileHeader::Read",
		    "",
		    FatalException,
		    (" !!! axis labels are not 1:x 2:y 3:z , but "+GmGenUtils::itoa(idSVal.first)+":"+idSVal.second+":").c_str());	
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
	theMinX = idDVal.second;
      } else if( idDVal.first == 2 ) {
	theMinY = idDVal.second;
      } else if( idDVal.first == 3 ) {
	theMinZ = idDVal.second;
      }
    } else if( stemp.find("number of bytes per pixel") != std::string::npos ) {
      theNoBytesPerPixel = getDVal(stemp); // TO BE IMPLEMENTED
    } else if( stemp.find("name of data file") != std::string::npos ) {
      theDataFileName = getSVal(stemp);
    } else if( stemp.find("!number format") != std::string::npos ) {
      theNumberFormat = getSVal(stemp);
    }
  }
  //--- Figure is always centered at (0,0,0). Construct limits
  theMinX = -theVoxelDimX*theNoVoxelsX/2.+theMinX;
  theMinY = -theVoxelDimY*theNoVoxelsY/2.+theMinY;
  theMinZ = -theVoxelDimZ*theNoVoxelsZ/2.+theMinZ;
  //  G4cout << " MINY = " << theMinY << " = " <<  -theVoxelDimY << " * " << theNoVoxelsY/2. << " + "<< theMinY<< G4endl; //GDEB
  if( thePatientOrientation == "head_in" ) {
    if( thePatientRotation == "supine" ) {
      thePatientPosition = "HFS";
    } else if( thePatientRotation == "prone" ) {
      thePatientPosition = "HFS";
    } else { // "other"
      G4Exception("GmInterfileHeader::Read",
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
      G4Exception("GmInterfileHeader::Read",
		  "",
		  JustWarning,
		  G4String("!!!! patient rotation is not supported: "+thePatientRotation+" PatientPosition will be set to HFS").c_str()); 
    }
  } else { // "other
    G4Exception("GmInterfileHeader::Read",
		"",
		JustWarning,
		("!!!! patient orientation is not supported: "+thePatientOrientation+" PatientPosition will be set to HFS").c_str()); 
  }

  if( theDataFileName == "" ) {
    G4Exception("GmInterfileHeader::Read",
		"",
		FatalException,
		"!!!! No 'name of data file' tag found");
  }
  
  Print(std::cout);

}

//-----------------------------------------------------------------------
/*void GmInterfileHeader::ConvertParametersIntoData()
  {*/
//-----------------------------------------------------------------------
void GmInterfileHeader::Print( G4String foutName )
{

  std::ofstream fout(foutName);
  Print(fout);
}
//-----------------------------------------------------------------------
void GmInterfileHeader::Print( std::ostream& fout )
{
  fout << "!INTERFILE  := " << G4endl;
  fout << "name of data file := " << theDataFileName << G4endl;
  fout << "!GENERAL DATA := " << G4endl;
  fout << "!GENERAL IMAGE DATA := " << G4endl;
  fout << "!version of keys := 3.3 " << G4endl;
  fout << "!type of data := " << theDataType << G4endl;
  fout << "imagedata byte order := littleendian " << G4endl;
  fout << "!number format := " << theNumberFormat << G4endl;
  fout << "!number of bytes per pixel := " << theNoBytesPerPixel << G4endl;
  fout << "number of dimensions := " << theNDimensions << G4endl;
  fout << "matrix axis label [1] := " << theMatrixAxisLabels[1] << G4endl;
  fout << "!matrix size [1] := " << theNoVoxelsX << G4endl;
  fout << "scaling factor (mm/pixel) [1] := " <<theVoxelDimX << G4endl;
  fout << "offset (mm) [1] := " << theMinX << G4endl;
  fout << "matrix axis label [2] := " << theMatrixAxisLabels[2] << G4endl;
  fout << "!matrix size [2] := " << theNoVoxelsY << G4endl;
  fout << "scaling factor (mm/pixel) [2] := " <<theVoxelDimY << G4endl;
  fout << "offset (mm) [2] := " << theMinY << G4endl;
  fout << "matrix axis label [3] := " << theMatrixAxisLabels[3] << G4endl;
  fout << "!matrix size [3] := " << theNoVoxelsZ << G4endl;
  fout << "scaling factor (mm/pixel) [3] := " <<theVoxelDimZ << G4endl;
  fout << "offset (mm) [3] := " << theMinZ << G4endl;
  fout << "number of time frames := 1 " << G4endl;
  fout << "image scaling factor[1] := 1 " << G4endl;
  fout << "data offset in bytes[1] := 0 " << G4endl;
  fout << "quantification units := 1 " << G4endl;
  fout << "!END OF INTERFILE :=  " << G4endl;
  //  fprintf (theOutFileHV, "!version of keys := 3.3\n");
  //  fprintf (theOutFileHV, "!data offset in bytes := 0\n");
  //  fprintf (file_stream, "patient id := N_DIR: %d SPAN %d, F_NORMA %f \n", Nx, 0, 0.0);
  //fprintf (theOutFileHV, "!PET STUDY (General) :=\n");
  // fprintf (theOutFileHV, "!PET data type := 3D-Sinogram\n");
  //fprintf (theOutFileHV, "process status := Reconstructed\n");
  
}

//-----------------------------------------------------------------------
void GmInterfileHeader::Displace( G4ThreeVector theDisp ) 
{
  G4double precision = 1e-4;
  
  G4double dispX = theDisp.x();
  if( dispX != 0 ) {
    G4double voxelWidthX = GetVoxelHalfX()*2.;
    if( (dispX/voxelWidthX - int(dispX/voxelWidthX)) > voxelWidthX*precision ){
      G4Exception("GmSqdose::Displace",
		  "",
		  FatalException,
		  G4String("X displacement= "+GmGenUtils::ftoa(dispX)
			   +" is not a multiple of X voxel_width= "+GmGenUtils::ftoa(voxelWidthX)).c_str());
    }
    /*t    for( size_t ix = 0; ix < theVoxelLimitsX.size(); ix++ ) {
      theVoxelLimitsX[ix] += dispX;
      }*/
    
  }
  
  G4double dispY = theDisp.y();
  if( dispY != 0 ) {
    G4double voxelWidthY = GetVoxelHalfY()*2.;
    if( (dispY/voxelWidthY - int(dispY/voxelWidthY)) > voxelWidthY*precision ){
      G4Exception("GmSqdose::Displace",
		  "",
		  FatalException,
		  G4String("Y displacement= "+GmGenUtils::ftoa(dispY)
			   +" is not a multiple of Y voxel_width= "+GmGenUtils::ftoa(voxelWidthY)).c_str());
    }
    /*t    for( size_t iy = 0; iy < theVoxelLimitsY.size(); iy++ ) {
      theVoxelLimitsY[iy] += dispY;
      }*/
  }
  G4double dispZ = theDisp.z();
  if( dispZ != 0 ) {
    G4double voxelWidthZ = GetVoxelHalfZ()*2.;
    if( (dispZ/voxelWidthZ - int(dispZ/voxelWidthZ)) > voxelWidthZ*precision ){
      G4Exception("GmSqdose::Displace",
		  "",
		  FatalException,
		  G4String("Z displacement= "+GmGenUtils::ftoa(dispZ)
			   +" is not a multiple of Z voxel_width= "+GmGenUtils::ftoa(voxelWidthZ)).c_str());
    }
    /*    for( size_t iz = 0; iz < theVoxelLimitsZ.size(); iz++ ) {
      theVoxelLimitsZ[iz] += dispZ;
      }*/
  }
  
  G4cout << " GmInterfileHeader::Displace Xmin/max " << GetMinX()<<"/"<<GetMaxX()
	 << " Ymin/max " << GetMinY()<<"/"<<GetMaxY()
	 << " Zmin/max " << GetMinZ()<<"/"<<GetMaxZ() << G4endl;
}
  
//----------------------------------------------------------------------------
std::pair<int,std::string> GmInterfileHeader::getIdSVal(std::string stemp)
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
  //  G4cout << ifn1 << " GmInterfileHeader::getIdVal( Z1Z" << sid << "Z2Z" << sval << "Z3Z" << G4endl; //GDEB
  return idSVal;
}

//----------------------------------------------------------------------------
std::pair<int,double> GmInterfileHeader::getIdDVal(std::string stemp)
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
  //  G4cout << ifn1 << " GmInterfileHeader::getIdVal( Z1Z" << sid << "Z2Z" << sval << "Z3Z" << G4endl; //GDEB
  return idDVal;
}

//----------------------------------------------------------------------------
double GmInterfileHeader::getDVal(std::string stemp)
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
std::string GmInterfileHeader::getSVal(std::string stemp)
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
void GmInterfileHeader::ChangeVoxelLimits( G4int ixmin, G4int ixmax, G4int iymin, G4int iymax, G4int izmin, G4int izmax )
{
  theNoVoxelsX = ixmax-ixmin;
  theNoVoxelsY = iymax-iymin;
  theNoVoxelsZ = izmax-izmin;
  theMinX = theMinX + ixmin*theVoxelDimX;
  theMinY = theMinY + ixmin*theVoxelDimY;
  theMinZ = theMinZ + ixmin*theVoxelDimZ;
  
}
