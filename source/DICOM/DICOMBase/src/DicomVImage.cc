#include "DicomVImage.hh"
#include "DicomVerbosity.hh"
#include "DicomMgr.hh"
#include "DicomVLine.hh"
#include "DicomVLineList.hh"
#include "DicomVLineSet.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

//G4double DicomVImage::DP_PRECISION = 1.E-4;

//size_t DicomVImage::theStructureIdxShift = 100;
//-----------------------------------------------------------------------------
DicomVImage::DicomVImage(G4String name, DIModality mod ) : theName(name), theModality(mod)
{
  bAutomaticOperate = true; 
  DicomMgr::GetInstance()->AddImage(this);
  thePrecision = 1.e-4;
  theData = 0;
  theRotation = new G4RotationMatrix();
  //  G4cout << this << " DicomVImage1 " << name << " " << DicomVImage::GetModalityStr(mod) << " auto " << IsAutomaticOperate() << G4endl; //GDEB
  theOrientation = DIO_XY;
  //  G4cout << this << " 01IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << G4endl; //GDEB
  //  G4cout << " DicomVImage::DicomVImage " << name << " of modality " << DicomVImage::GetModalityStr(mod) << "   from " << name << G4endl; //GDEB

  bIsodosesNormalized = false;
  theIsodosesXY = 0;
  theIsodosesXZ = 0;
  theIsodosesYZ = 0;
}

//-----------------------------------------------------------------------------
DicomVImage::DicomVImage(DicomVImage* image, G4String name, DIModality modality, DIOrientation orient) : theName(name), theModality(modality), theOrientation(orient)
{
  //  G4cout << this << " DicomVImage2 " << name << " " << DicomVImage::GetModalityStr(modality) << G4endl; //GDEB
  theData = 0;
  SetHeader( image );
  theName = name;
  theModality = modality;
  theOrientation = orient;  

  DicomMgr::GetInstance()->AddImage(this);

  //  G4cout << this << " 0IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << G4endl; //GDEB 
  //  G4cout << this << " DicomVImage::DicomVImage " << name << " = " << theName << " of modality " << DicomVImage::GetModalityStr(theModality) << "   from image " << image->GetName() << " of modality " << DicomVImage::GetModalityStr(image->GetModality()) << G4endl; //GDEB

  bIsodosesNormalized = false;
  theIsodosesXY = 0;
  theIsodosesXZ = 0;
  theIsodosesYZ = 0;
  theRotation = new G4RotationMatrix();
  *theRotation = (*(image->GetRotation()));
}

//-----------------------------------------------------------------------------
DicomVImage::DicomVImage(const DicomVImage& rhs)
{
    //  G4cout << this << " DicomVImage2 " << name << " " << DicomVImage::GetModalityStr(modality) << G4endl; //GDEB
  theData = 0;
  SetHeader( &rhs );
  theName = "";
  theModality = DIM_OTHER;
  theOrientation = DIO_XY;
  
  DicomMgr::GetInstance()->AddImage(this);

  //  G4cout << this << " 0IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << G4endl; //GDEB 
  //  G4cout << this << " DicomVImage::DicomVImage " << name << " = " << theName << " of modality " << DicomVImage::GetModalityStr(theModality) << "   from image " << image->GetName() << " of modality " << DicomVImage::GetModalityStr(image->GetModality()) << G4endl; //GDEB

  bIsodosesNormalized = false;
  theIsodosesXY = 0;
  theIsodosesXZ = 0;
  theIsodosesYZ = 0;
  
  //----- Copy image data
  size_t nData = GetNoVoxels();
  std::vector<G4double>* data2 = rhs.GetData();
  for( size_t ii = 0; ii < nData; ii++ ) {
    theData->at(ii) = data2->at(ii);
  }

  theRotation = new G4RotationMatrix();
  *theRotation = (*(rhs.GetRotation()));

}

//-----------------------------------------------------------------------------
DicomVImage::~DicomVImage()
{
  theData->clear();
  delete theData;
  delete theRotation;
  /*  if( theIsodosesXY ) delete theIsodosesXY;
  if( theIsodosesXZ ) delete theIsodosesXZ;
  if( theIsodosesYZ ) delete theIsodosesYZ; */
}

//-----------------------------------------------------------------------------
void DicomVImage::SetHeader(const DicomVImage* image )
{
  bAutomaticOperate = image->IsAutomaticOperate();
  theNoVoxelsX = image->GetNoVoxelsX();
  theNoVoxelsY = image->GetNoVoxelsY();
  theNoVoxelsZ = image->GetNoVoxelsZ();

  theMinX = image->GetMinX();
  theMinY = image->GetMinY();
  theMinZ = image->GetMinZ();
  theMaxX = image->GetMaxX();
  theMaxY = image->GetMaxY();
  theMaxZ = image->GetMaxZ();

  if( theData ) theData->clear();
  theData = new std::vector<G4double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
  thePrecision = 1.e-4*std::min(std::min(GetVoxelDimX(),GetVoxelDimY()),GetVoxelDimZ());
  //  G4cout << GetName() << " IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
DicomVImage DicomVImage::operator*=( const G4double val )
{
  size_t nData = GetNoVoxels();
  //  G4cout << this << " DicomVImage::operator*= " << nData << " =? " << theData->size() << G4endl; //GDEB
  for( size_t ii = 0; ii < nData; ii++ ) {
    theData->at(ii) *= val;
  }
  return *this;
}

//-----------------------------------------------------------------------------
void DicomVImage::operator+=( const DicomVImage& rhs )
{
  *this = *this + rhs;
}

//-----------------------------------------------------------------------------
DicomVImage DicomVImage::operator+( const DicomVImage& rhs )
{
  CheckImageHeadersAreEqual( &rhs );

  //----- Build image adding first one
  size_t nData = GetNoVoxels();
  std::vector<G4double>* data2 = rhs.GetData();
  for( size_t ii = 0; ii < nData; ii++ ) {
    theData->at(ii) += data2->at(ii);
  }
  
  return *this;
}


//-----------------------------------------------------------------------------
void DicomVImage::CheckImageHeadersAreEqual( const DicomVImage* img2 )
{
  //----- Check that both images have the same dimensions
  if( GetNoVoxelsX() != img2->GetNoVoxelsX() ||
      GetNoVoxelsY() != img2->GetNoVoxelsY() ||
      GetNoVoxelsZ() != img2->GetNoVoxelsZ() ) {
    G4cerr << "DicomVImage error comparing two images:\
        !!! Different number of voxels: "
	   << "  X= " << GetNoVoxelsX() << " <> " << img2->GetNoVoxelsX()
	   << "  Y=  " << GetNoVoxelsY() << " <> " << img2->GetNoVoxelsY()
	   << "  Z=  " << GetNoVoxelsZ() << " <> " << img2->GetNoVoxelsZ()
	   << G4endl;
    G4Exception("DicomVImage::CheckImageHeadersAreEqual",
		"",FatalException,"");
  }
  //----- Check that both images have the same extensions  
  if( fabs(GetMinX() - img2->GetMinX()) > GetPrecision() || fabs(GetMaxX() - img2->GetMaxX()) > GetPrecision() ||
      fabs(GetMinY() - img2->GetMinY()) > GetPrecision() || fabs(GetMaxY() - img2->GetMaxY()) > GetPrecision() ||
      fabs(GetMinZ() - img2->GetMinZ()) > GetPrecision() || fabs(GetMaxZ() - img2->GetMaxZ()) > GetPrecision() ) {
    G4cerr << "DicomVImage error comparing two images:\
        !!! Different extensions: "
	   << "  Xmin= " << GetMinX() << " <> " << img2->GetMinX()
	   << "  Xmax= " << GetMaxX() << " <> " << img2->GetMaxX()
	   << "  Ymin= " << GetMinY() << " <> " << img2->GetMinY()
	   << "  Ymax= " << GetMaxY() << " <> " << img2->GetMaxY()
	   << "  Zmin= " << GetMinZ() << " <> " << img2->GetMinZ()
	   << "  Zmax= " << GetMaxZ() << " <> " << img2->GetMaxZ()
	   << G4endl;
    G4Exception("DicomVImage::operator+","",
		FatalException,"");
  }

  //----- Check that both images have the same orientation
  if( GetOrientation() != img2->GetOrientation() ) {
    G4cerr << "DicomVImage error comparing two images:\
       !!! Different orientations "
	   << "  Orientation= " << GetOrientationName() << " <> " << img2->GetOrientationName() 
	   << G4endl;
    G4Exception("DicomVImage::operator+","",
		FatalException,"");
  }

  //----- Check that both images have the same rotation
  G4RotationMatrix rot = GetRotation()->inverse() * *(img2->GetRotation());
  if( fabs(rot.xx()-1) > GetPrecision() ||
      fabs(rot.yy()-1) > GetPrecision() ||
      fabs(rot.zz()-1) > GetPrecision() ||
      fabs(rot.xy()) > GetPrecision() ||
      fabs(rot.xz()) > GetPrecision() ||
      fabs(rot.yx()) > GetPrecision() ||
      fabs(rot.yz()) > GetPrecision() ||
      fabs(rot.zx()) > GetPrecision() ||
      fabs(rot.zy()) > GetPrecision() ) {
    G4cerr << "DicomVImage error comparing two images:\
       !!! Different rotations    "
	   << "  Rotation = " << *GetRotation() << " <> " << *(img2->GetRotation()) 
	   << G4endl;
    G4Exception("DicomVImage::operator+","",
		FatalException,"");
  }

}

//-----------------------------------------------------------------------------
DicomVImage DicomVImage::operator=( const DicomVImage& rhs )
{
 if (this != &rhs) {   //  CheckImageHeadersAreEqual( &rhs );
  SetHeader( &rhs );
  theIsodosesXY = 0;
  theIsodosesXZ = 0;
  theIsodosesYZ = 0;
  
  //----- Copy image data
  size_t nData = GetNoVoxels();
  std::vector<G4double>* data2 = rhs.GetData();
  for( size_t ii = 0; ii < nData; ii++ ) {
    theData->at(ii) = data2->at(ii);
  }
 }  
  *theRotation = (*(rhs.GetRotation()));
 return *this;
}


//-----------------------------------------------------------------------------
G4bool DicomVImage::ReadDataFromTextFile( std::ifstream& fin, G4bool bReadHeader )
{
  if( bReadHeader ) {
    fin >> theNoVoxelsX >> theNoVoxelsY >> theNoVoxelsZ;
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage " << GetName() << " " << GetModalityStr() << " nVoxel X/Y/Z " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
    theData = new std::vector<G4double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
    fin >> theMinX >> theMaxX;
    fin >> theMinY >> theMaxY;
    fin >> theMinZ >> theMaxZ;
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage " << GetName() << " " << GetModalityStr()
				      << " min/max X " << theMinX << " " << theMaxX 
				      << " min/max X " << theMinY << " " << theMaxY 
				      << " min/max X " << theMinZ << " " << theMaxZ << G4endl;
  }
  G4double voxelDimX = (theMaxX-theMinX)/theNoVoxelsX;
  G4double voxelDimY = (theMaxY-theMinY)/theNoVoxelsY;
  G4double voxelDimZ = (theMaxZ-theMinZ)/theNoVoxelsZ;
  G4double data;
  //  size_t datai;
  //  for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
  //   for( size_t iy = 0; iy < theNoVoxelsY; iy++ ) {
  //    for( size_t ix = 0 ix < theNoVoxelsX; ix++ ) {
  size_t nVox = GetNoVoxels();
  for( size_t ii = 0; ii < nVox; ii++ ) {
    fin >> data;
    /*    for( int jj = 0; jj < log10(data)/2.; jj++ ) {
      G4cout << std::setprecision(20) << data << " DATA " << jj << " : " << size_t(data/pow(100,jj))%100 << G4endl; //GDEB
      } */
    theData->at(ii) = data;
    size_t ix = ii%theNoVoxelsX;
    size_t iy = ii/theNoVoxelsX%theNoVoxelsY;
    size_t iz = ii/theNoVoxelsX/theNoVoxelsY;
    double posX = theMinX+(ix+0.5)*voxelDimX;
    double posY = theMinY+(iy+0.5)*voxelDimY;
    double posZ = theMinZ+(iz+0.5)*voxelDimZ;
    G4ThreeVector pos(posX,posY,posZ);
    if( DicomVerb(debugVerb) ) G4cout << GetName() << " " << ii << " " << iz << " " << ix << " " << iy << " : " << pos << " DATA= " <<data << G4endl;  //GDEB
    //    G4ThreeVector posPrint(-50.,-192.242,-880.);
    //    G4ThreeVector posPrint(-50.1255,-192.242,-879.8);
    //    if( (pos-posPrint).mag() < 0.5 ) G4cout << " PRINT VOXEL " << GetName() << " " << (pos-posPrint).mag() << " " << pos << " : " << data << G4endl;  //GDEB
    //    if( DicomVerb(debugVerb) && ii%1000 == 0 ) G4cout << GetName() << " " << ii << " " << iz << " " << ix << " " << iy << " : " << data << G4endl;  //GDEB
    //    if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 )
    //      G4cout << pos << " DATA " << ii << " = " << data << G4endl; //GDEB
    if( DicomVerb(testVerb) && ii%1 == 0 ) G4cout << GetName() << " : " << ii << " " << ix << " " << iy << " " << iz << " = " << data << G4endl; 
  }

  thePrecision = 1.e-4*std::min(std::min(GetVoxelDimX(),GetVoxelDimY()),GetVoxelDimZ());
  //  G4cout << this << " 1IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << " EOF " << fin.eof() << G4endl; //GDEB

  if( fin.eof() ) {
    return 1;
  } else {
    return 0;
  }
}

//-----------------------------------------------------------------------------
void DicomVImage::PrintHeader(std::ostream& out )
{
  out << "IMAGE " << GetName()
      << "@ NoVoxels " << theNoVoxelsX << " * " << theNoVoxelsY << " * " << theNoVoxelsZ << " = " << theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ << G4endl;
  out << "@ DIM X: " << theMinX << " " << theMaxX
      << " Y: " << theMinY << " " << theMaxY
      << " Z: " << theMinZ << " " << theMaxZ << G4endl;
}

//-----------------------------------------------------------------------------
void DicomVImage::DumpHeaderToTextFile(std::ostream& fout)
{
  if( DicomVerb(warningVerb) ) G4cout << GetName() << " DumpHeaderToTextFile " << G4endl;

  fout << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << std::endl;
  fout << theMinX << " " << theMaxX << std::endl;
  fout << theMinY << " " << theMaxY << std::endl;
  fout << theMinZ << " " << theMaxZ << std::endl;

}


//-----------------------------------------------------------------------------
void DicomVImage::DumpDataToTextFile(std::ostream& fout, G4bool bFloat)
{
  if( DicomVerb(warningVerb) ) G4cout << GetName() << " DumpDataToTextFile " << G4endl;
  size_t copyNo = 0;
  for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
    size_t copyNoXY = 0;
    for( size_t iy = 0; iy < theNoVoxelsY; iy++ ) {
      //	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
      for( size_t ix = 0; ix < theNoVoxelsX; ix++ ) {
	//	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
	//	G4cout << ix+iy*theNoVoxelsX+iz*theNoVoxelsX*theNoVoxelsY << " : " << ix << " " << iy << " " << iz << " " << theMinX+GetVoxelDimX()*(ix+0.5) << " " << theMinY+GetVoxelDimY()*(iy+0.5) << " " << theMinZ+GetVoxelDimZ()*(iz+0.5) << " DATA_TO_TEXT " << theData->at(copyNo) << G4endl; //GDEB
	if( bFloat ) {
	  fout << std::setprecision(8) << std::setw(10) << theData->at(copyNo) << " ";
	} else {
	  fout << std::setprecision(10)  << std::setw(12) << theData->at(copyNo) << " "; // precision to write a size_t (=64 bits) (pow(2,64)=1,844674407e19
	}
	copyNo++;
	//-       	if( !bFloat ) fout << " " << copyNo << " D=" << *data++ << G4endl;
	//	if( ix != theNoVoxelsX-1) fout << " ";
	if( bFloat && copyNoXY%8 == 7 ) fout << G4endl;
	copyNoXY++;
      }
      if( ( bFloat && copyNo%8 != 0 ) || !bFloat ) fout << G4endl;
    }
  }

}

//-----------------------------------------------------------------------------
void DicomVImage::DumpToSqdoseFile(FILE* fout, DicomVImage* doseSqImage )
{
  if( DicomVerb(warningVerb) ) G4cout << GetName() << " DumpToSqdoseFile " << G4endl;

  //--- Check image types
  if( GetModality() != DIM_RTDose ) {
    G4Exception("DicomVImage::DumpToSqdoseFile"
		,"",
		JustWarning,
		("First Image should be of modality DIM_RTDose, not " + GetModalityStr()).c_str());
  }

  //--- Check if error image has same dimensions 
  if( doseSqImage != 0 ) {
    if( doseSqImage->GetModality() != DIM_Sqdose_sq ) {
      G4Exception("DicomVImage::DumpToSqdoseFile"
		  ,"",
		  FatalException,
		  ("Second Image has to be of modality DIM_Sqdose_sq, not " + doseSqImage->GetModalityStr()).c_str());
    }
    CheckImageHeadersAreEqual( doseSqImage );
  }
  
  //--- WRITE HEADER
  float theNoEvent = 1000; // ANY NUMBER != 0/1
  if( ExtraParamExists("NoEvent") ) theNoEvent = GetExtraParamNumeric("NoEvent");
  
  if(fwrite(&theNoEvent,sizeof(float),1,fout)!=1)
    printf("\n Error writing nEvent. \n");

  if(fwrite(&theNoVoxelsX,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelX. \n");
  if(fwrite(&theNoVoxelsY,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelY. \n");
  if(fwrite(&theNoVoxelsZ,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelZ. \n");
  if( DicomVerb(debugVerb) ) G4cout << " HEADER NEvent " << theNoEvent << " Nvoxels " << theNoVoxelsX << " " << theNoVoxelsY<< " " << theNoVoxelsZ << G4endl;

  float* vlimx = new float[theNoVoxelsX+1];
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    vlimx[ii] = theMinX+ii*GetVoxelDimX();
    //    G4cout << ii << " limx " << vlimx[ii] << " = " << fVoxelLimitsX[ii] << G4endl;
  }
  if(fwrite(vlimx, sizeof(float),theNoVoxelsX+1,fout)!=size_t(theNoVoxelsX+1))
    printf("\n Error writing VoxelLimitsX. \n");
  
  float* vlimy = new float[theNoVoxelsY+1];
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    vlimy[ii] = theMinY+ii*GetVoxelDimY();
    //    G4cout << ii << " limy " << vlimy[ii] << " = " << fVoxelLimitsY[ii] << G4endl;
  }
  if(fwrite(vlimy, sizeof(float),theNoVoxelsY+1,fout)!=size_t(theNoVoxelsY+1))
    printf("\n Error writing VoxelLimitsY. \n");
  
  float* vlimz = new float[theNoVoxelsZ+1];
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ){
    vlimz[ii] = theMinZ+ii*GetVoxelDimZ();
    //    G4cout << ii << " limz " << vlimz[ii] << " = " << fVoxelLimitsZ[ii] << G4endl;
  }
  if(fwrite(vlimz, sizeof(float),theNoVoxelsZ+1,fout)!=size_t(theNoVoxelsZ+1))
    printf("\n Error writing VoxelLimitsZ. \n");

  //  std::cout << " ROTATION EULER " << theRotation->getPhi() << " " << theRotation->getTheta() << " " << theRotation->getPsi() << G4endl; //GDEB
  /*  if( fabs(1.-fOrientationRows*G4ThreeVector(1.,0.,0.)) > 1.E-6 ) {
    G4Exception("DicomFileRTDose::DumpToSqdoseFile",
		"",
		FatalException,
		"Do not know how to treat doses with rotations, please contact GAMOS authors");
		} */
  
  float rotxx = 1.;
  if(fwrite(&rotxx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxx. \n");
  float rotxy = 0.;
  if(fwrite(&rotxy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxy. \n");
  float rotxz = 0.;
  if(fwrite(&rotxz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxz. \n");
  float rotyx = 0.;
  if(fwrite(&rotyx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyx. \n");
  float rotyy = 1.;
  if(fwrite(&rotyy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyy. \n");
  float rotyz = 0.;
  if(fwrite(&rotyz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyz. \n");
  float rotzx = 0.;
  if(fwrite(&rotzx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzx. \n");
  float rotzy = 0.;
  if(fwrite(&rotzy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzy. \n");
  float rotzz = 1.;
  if(fwrite(&rotzz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzz. \n");

  delete vlimx;
  delete vlimy;
  delete vlimz;
    
  //--- WRITE BODY
  SqdoseType fType = SqTALL;
  if(fwrite(&fType,
	    sizeof(size_t),1,fout)!=1){
    G4Exception("GmSqdose::Print",
		"Error",
		FatalException,
		"Error writing type");
  }
 
  size_t siz = theNoVoxelsX * theNoVoxelsY * theNoVoxelsZ;
  //  G4cout << " N VOXELS WRITE " << siz << G4endl;  //GDEB
  //----- Loop to all voxels and print dose
  for( size_t ii = 0; ii < siz; ii++ ) {
    float dose = theData->at(ii);
    if(fwrite((float *) &dose,
	      sizeof(float),1,fout)!=1) {
      G4Exception("GmSqdose::Print",
		  "Error",
		  FatalException,
		  "Error writing dose");
    }
  }

  if( !doseSqImage ) { // set error = 0
      //    G4double error = (theDoseSqs[ii]*nEvents - theDoses[ii]*theDoses[ii]) / (nEvents-1); 
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose2 = theData->at(ii) * theData->at(ii)/theNoEvent;
      //-      if( ii < 131200 ) G4cout << ii << " write " << dose2 << " = " << theData->at(ii) << " err " << dose2*theNoEvent - theData->at(ii)*theData->at(ii) << G4endl ;
      if(fwrite((float *) &dose2,
		sizeof(float),1,fout)!=1){
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing doseSq");
      }
    }
  } else {
    std::vector<G4double>* data2 = doseSqImage->GetData();
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose2 = data2->at(ii)*data2->at(ii);
      if(fwrite((float *) &dose2,
		sizeof(float),1,fout)!=1){
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing doseSq");
      }
    }
  }
}

//-----------------------------------------------------------------------------
G4String DicomVImage::GetModalityStr() const
{
  return DicomVImage::GetModalityStr(theModality);
}
    
//-----------------------------------------------------------------------------
G4String DicomVImage::GetModalityStr(DIModality mod)
{
  switch(mod) {
  case DIM_CT:
    return "DIM_CT";
  case DIM_NM:
    return "DIM_NM";
  case DIM_RTStruct:
    return "DIM_RTStruct";
  case DIM_RTDose:
    return "DIM_RTDose";
  case DIM_G4dcmCT_MateID:
    return "DIM_G4dcmCT_MateID";
  case DIM_G4dcmCT_MateDens:
    return "DIM_G4dcmCT_MateDens";
  case DIM_G4dcmCT_StructID:
    return "DIM_G4dcmCT_StructID";
  case DIM_G4dcmNM:
    return "DIM_G4dcmNM";
  case DIM_G4dcmDose:
    return "DIM_G4dcmDose";
  case DIM_Sqdose:
    return "DIM_Sqdose";
  case DIM_Sqdose_sq:
    return "DIM_Sqdose_sq";
  case DIM_Sqdose_err:
    return "DIM_Sqdose_err";
  case DIM_Sqdose_errRel:
    return "DIM_Sqdose_errRel";
  case DIM_GammaIndex:
    return "DIM_GammaIndex";
  case DIM_Interfile:
    return "DIM_Interfile";
  case DIM_OTHER:
    return "DIM_OTHER";
  }

  return "UNKNOWN";
}

//-----------------------------------------------------------------------------
DIModalityShort DicomVImage::GetModalityShort()
{
  return GetModalityShort(theModality);
}

//-----------------------------------------------------------------------------
DIModalityShort DicomVImage::GetModalityShort(DIModality mod)
{
  switch(mod) {
  case DIM_CT:
    return DIMS_CT;
  case DIM_NM:
    return DIMS_NM;
  case DIM_RTStruct:
    return DIMS_Struct;
  case DIM_RTDose:
    return DIMS_Dose;
  case DIM_G4dcmCT_MateID:
    return DIMS_CT_MateID;
  case DIM_G4dcmCT_MateDens:
    return DIMS_CT_MateDens;
  case DIM_G4dcmCT_StructID:
    return DIMS_Struct;
  case DIM_G4dcmNM:
    return DIMS_NM;
  case DIM_G4dcmDose:
    return DIMS_Dose;
  case DIM_Sqdose:
    return DIMS_Dose;
  case DIM_Sqdose_sq:
    return DIMS_Dose_sq;
  case DIM_Sqdose_err:
    return DIMS_Dose_err;
  case DIM_Sqdose_errRel:
    return DIMS_Dose_errRel;
  case DIM_GammaIndex:
    return DIMS_GammaIndex;
  case DIM_Interfile:
    return DIMS_Interfile;
  case DIM_OTHER:
    return DIMS_OTHER;
  }

  return DIMS_OTHER;
}


//-----------------------------------------------------------------------------
G4String DicomVImage::GetModalityShortStr() const
{
  return GetModalityShortStr(theModality);
}

//-----------------------------------------------------------------------------
G4String DicomVImage::GetModalityShortStr(DIModality mod)
{
  switch(mod) {
  case DIM_CT:
    return "DIMS_CT";
  case DIM_NM:
    return "DIMS_NM";
  case DIM_RTStruct:
    return "DIMS_Struct";
  case DIM_RTDose:
    return "DIMS_Dose";
  case DIM_G4dcmCT_MateID:
    return "DIMS_CT_MateID";
  case DIM_G4dcmCT_MateDens:
    return "DIMS_CT_MateDens";
  case DIM_G4dcmCT_StructID:
    return "DIMS_Struct";
  case DIM_G4dcmNM:
    return "DIMS_NM";
  case DIM_G4dcmDose:
    return "DIMS_Dose";
  case DIM_Sqdose:
    return "DIMS_Dose";
  case DIM_Sqdose_sq:
    return "DIMS_Dose_sq";
  case DIM_Sqdose_err:
    return "DIMS_Dose_err";
  case DIM_Sqdose_errRel:
    return "DIMS_Dose_errRel";
  case DIM_GammaIndex:
    return "DIMS_GammaIndex";
  case DIM_Interfile:
    return "DIMS_Intefile";
  case DIM_OTHER:
    return "DIMS_OTHER";
  }
  
  return "DIMS_OTHER";
}

//---------------------------------------------------------------------------
G4String DicomVImage::GetOrientationName() const
{
  switch( theOrientation ){
    case DIO_XY:
      return "DIO_XY";
    case DIO_XZ:
      return "DIO_XZ";
    case DIO_YZ:
      return "DIO_YZ";
  }
  return "DIO_NONE";
}

//---------------------------------------------------------------------------
void DicomVImage::AddExtraParam( G4String key, G4String value )
{
  theExtraParams[key] = value;
}

//---------------------------------------------------------------------------
G4String DicomVImage::GetExtraParam( const G4String& key )
{
  std::map<G4String,G4String>::const_iterator ite = theExtraParams.find(key);
  if( ite != theExtraParams.end() ) {
    return (*ite).second;
  } else {
    return "";
  }
}

//---------------------------------------------------------------------------
G4double DicomVImage::GetExtraParamNumeric( const G4String& key )
{
  std::map<G4String,G4String>::const_iterator ite = theExtraParams.find(key);
  if( ite != theExtraParams.end() ) {
    return GmGenUtils::GetValue((*ite).second);
  } else {
    return 0.;
  }
}

//---------------------------------------------------------------------------
G4bool DicomVImage::ExtraParamExists( const G4String& key )
{
  std::map<G4String,G4String>::const_iterator ite = theExtraParams.find(key);
  if( ite != theExtraParams.end() ) {
    return true;
  } else {
    return false;
  }

}

//---------------------------------------------------------------------------
std::vector<size_t> DicomVImage::GetNoVoxelsV()
{
  std::vector<size_t> nVoxelsV;
  nVoxelsV.push_back(GetNoVoxelsX());
  nVoxelsV.push_back(GetNoVoxelsY());
  nVoxelsV.push_back(GetNoVoxelsZ());
  return nVoxelsV;
}
//---------------------------------------------------------------------------
std::vector<G4double> DicomVImage::GetMinV()
{
  std::vector<G4double>fMinV;
  fMinV.push_back(GetMinX());
  fMinV.push_back(GetMinY());
  fMinV.push_back(GetMinZ());
  return fMinV;
}

//---------------------------------------------------------------------------
std::vector<G4double> DicomVImage::GetMaxV()
{
  std::vector<G4double> fMaxV;
  fMaxV.push_back(GetMaxX());
  fMaxV.push_back(GetMaxY());
  fMaxV.push_back(GetMaxZ());
  return fMaxV;
}

//---------------------------------------------------------------------------
std::vector<G4double> DicomVImage::GetVoxelDimV()
{
  std::vector<G4double> fVoxDimV;
  fVoxDimV.push_back(GetVoxelDimX());
  fVoxDimV.push_back(GetVoxelDimY());
  fVoxDimV.push_back(GetVoxelDimZ());
  return fVoxDimV;
}

//---------------------------------------------------------------------------
G4ThreeVector DicomVImage::GetCentre()
{
  G4ThreeVector centre( (theMaxX+theMinX)/2., (theMaxY+theMinY)/2., (theMaxZ+theMinZ)/2. );

  return centre;
}

//---------------------------------------------------------------------------
G4double DicomVImage::GetMaxValue()
{
  G4double maxV = -DBL_MAX;
  size_t nvox = GetNoVoxels();
  for( size_t ii = 0; ii < nvox; ii++ ) {
    G4double data = theData->at(ii);    
    if( data > maxV ) {
      if( DicomVerb(debugVerb) ) {
	size_t ix = ii%theNoVoxelsX;
	size_t iy = ii/theNoVoxelsX%theNoVoxelsY;
	size_t iz = ii/theNoVoxelsX/theNoVoxelsY;
	G4ThreeVector ptMax(theMinX+(ix+0.5)*GetVoxelDimX(),theMinY+(iy+0.5)*GetVoxelDimY(),theMinZ+(iz+0.5)*GetVoxelDimZ());
	G4cout << GetName() << " DicomVImage::GetMaxValue() " << maxV << " at " << ptMax << " " << ix<<":"<<iy<<":"<<iz << G4endl;
      }
      maxV = data;
    }
  }

  return maxV;
}

//---------------------------------------------------------------------------
std::pair<G4double,G4ThreeVector> DicomVImage::GetMaxValueAndPoint()
{
  G4double maxV = -DBL_MAX;
  G4int maxI = -1;
  size_t nvox = GetNoVoxels();
  for( size_t ii = 0; ii < nvox; ii++ ) {
    G4double data = theData->at(ii);
    if( data > maxV ) {
      maxV = data;
      maxI = ii;
    }
  }

  size_t ix = maxI%theNoVoxelsX;
  size_t iy = maxI/theNoVoxelsX%theNoVoxelsY;
  size_t iz = maxI/theNoVoxelsX/theNoVoxelsY;
  G4ThreeVector point( theMinX+(ix+0.5)*GetVoxelDimX(),
		       theMinY+(iy+0.5)*GetVoxelDimY(),
		       theMinZ+(iz+0.5)*GetVoxelDimZ());
  
  //  G4cout << "   DicomVImage::GetMaxValueAndPoint " << point << " " << ix << " " << iy << " " << iz << " " << *this << G4endl; //GDEB
  return std::pair<G4double,G4ThreeVector>(maxV,point);
}

//---------------------------------------------------------------------------
G4double DicomVImage::GetValueAtPoint(G4ThreeVector point, G4ThreeVector extension)
{
  //--- GET GEOMETRY DATA
  size_t nVoxXY = theNoVoxelsX*theNoVoxelsY;
  float fWidthX = GetVoxelDimX();
  float fWidthY = GetVoxelDimY();
  float fWidthZ = GetVoxelDimZ();
  // check point inside limits
  if( point.x()-extension.x() < theMinX || point.y()-extension.y() < theMinY || point.z()-extension.z() < theMinZ ||
      point.x()+extension.x() > theMaxX || point.y()+extension.y() > theMaxY || point.z()+extension.z() > theMaxZ ) {
    G4Exception("DicomVImage::GetValueAtPoint",
		"",
		FatalException,
		"Point out of image limits");
  }
  G4int nCVoxXMin = int((point.x()-extension.x()-theMinX)/fWidthX);
  //  G4cout << " nCVoxXMin " << nCVoxXMin << " " << (point.x()-extension.x()-theMinX)/fWidthX << " " << point.x() << " - " << extension.x() << " - " << theMinX << " )/ " << fWidthX << G4endl; //GDEB
  G4int nCVoxYMin = int((point.y()-extension.y()-theMinY)/fWidthY);
  //  G4cout << " nCVoxYMin " << nCVoxYMin << " " << (point.y()-extension.y()-theMinY)/fWidthY << " " << point.y() << " - " << extension.y() << " - " << theMinY << " )/ " << fWidthY << G4endl; //GDEB
  G4int nCVoxZMin = int((point.z()-extension.z()-theMinZ)/fWidthZ);
  G4int nCVoxXMax = int((point.x()+extension.x()-theMinX)/fWidthX);
  G4int nCVoxYMax = int((point.y()+extension.y()-theMinY)/fWidthY);
  G4int nCVoxZMax = int((point.z()+extension.z()-theMinZ)/fWidthZ);
  //  G4cout << " nCVoxZMin " << nCVoxZMin << " " << (point.z()-extension.z()-theMinZ)/fWidthZ << " " << point.z() << " - " << extension.z() << " - " << theMinZ << " )/ " << fWidthZ << G4endl; //GDEB

  G4int nofCVox = (nCVoxXMax-nCVoxXMin+1)*(nCVoxYMax-nCVoxYMin+1)*(nCVoxZMax-nCVoxZMin+1);
  G4double sumData = 0;
  for( G4int ix = nCVoxXMin; ix <= nCVoxXMax; ix++ ) {
    for( G4int iy = nCVoxYMin; iy <= nCVoxYMax; iy++ ) {
      for( G4int iz = nCVoxZMin; iz <= nCVoxZMax; iz++ ) {
	G4int copyNo = ix + iy*theNoVoxelsX + iz*nVoxXY;	
	sumData += GetData(copyNo);
	// G4cout << " DicomVImage::GetValueAtPoint copyNo " << copyNo << " = " << ix << ":" << iy << ":" << iz << " " << GetData(copyNo) << " : " << sumData << " / " << nofCVox << G4endl; //GDEB
      }
    }
  }
  sumData /= nofCVox;
  //  G4cout << this << " DicomVImage::GetValueAtPoint MAX_VALUE " << sumData << " AVERAGE VOLUME " << point-extension << "  " << point+extension << G4endl; //GDEB

  return sumData;
}

//----------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const DicomVImage& img)
{
  os << " IMAGE: " << img.GetName()
     << " NVoxels: " << img.GetNoVoxelsX() << " " << img.GetNoVoxelsY() << " " << img.GetNoVoxelsZ()
     << " XDIM: " << img.GetMinX() << " " << img.GetMaxX()
     << " YDIM: " << img.GetMinY() << " " << img.GetMaxY()
     << " ZDIM: " << img.GetMinZ() << " " << img.GetMaxZ()
     << G4endl;

  return os;
}

//----------------------------------------------------------------------
void DicomVImage::ClearData()
{
  if( theData ) theData->clear();
}


//----------------------------------------------------------------------
void DicomVImage::BuildIsodLinesXY(std::vector<G4double> isodoseValues)
{
  theIsodosesXY = new DicomVLineSet("IsodosesXY",DPOrientXY);

  //G4double maxValue = GetMaxValue();
  G4double voxelDimX = GetVoxelDimX();
  G4double voxelDimY = GetVoxelDimY();
  G4double voxelDimZ = GetVoxelDimZ();
  size_t nVoxXY = theNoVoxelsX*theNoVoxelsY;
  //  size_t nVoxXY2 = 100*100;
  size_t nVoxXY2 = theNoVoxelsX*theNoVoxelsY;
  G4double pp;
  G4double x1;
  G4double y1;
  G4double LL;
  G4double vx;
  G4double vy;
  G4double vz;
  G4double ss;
  G4double x2;
  G4double y2;
  /*  std::vector<G4double> isodoseValues;
  for( size_t iisod = 0; iisod < isodPerCents.size(); iisod++ ) {
    isodoseValues.push_back(isodPerCents[iisod]/100.*maxValue);
    if( DicomVerb(debugVerb) ) G4cout << " DicomVImage::BuildIsodLinesXY isodose " << iisod << " = " << isodPerCents[iisod]<< " = " << isodPerCents[iisod] << " * " << maxValue << std::endl;
    } */
  //--- Loop to Z planes
  for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
    double pointZ = theMinZ+(iz+0.5)*voxelDimZ;
    DicomVLineList* polyList = new DicomVLineList("isodoses_Z="+GmGenUtils::ftoa(pointZ),iz,DPOrientXY);
    //--- Loop to isodose values
    for( size_t iisod = 0; iisod < isodoseValues.size(); iisod++ ) {
      std::vector<ppidv> pointPairsIsod;
      G4double isodVal = isodoseValues[iisod];
      for( size_t iy = 0; iy < theNoVoxelsY-1; iy++ ) {
	G4double cy = theMinY+(iy+0.5)*voxelDimY;
	G4double cy1y = theMinY+(iy+1.5)*voxelDimY;
	for( size_t ix = 0; ix< theNoVoxelsX-1; ix++ ) {
	  size_t idxy = ix+theNoVoxelsX*iy;
	  G4double cx = theMinX+(ix+0.5)*voxelDimX;
	  G4double cx1x = theMinX+(ix+1.5)*voxelDimX;
	  G4double data = theData->at(ix + iy*theNoVoxelsX + iz*nVoxXY);
	  G4double data1x = theData->at((ix+1) + iy*theNoVoxelsX + iz*nVoxXY);
	  G4double data1y = theData->at(ix + (iy+1)*theNoVoxelsX + iz*nVoxXY);
	  G4double data1x1y = theData->at((ix+1) + (iy+1)*theNoVoxelsX + iz*nVoxXY);
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY " << ix << " : " << iy << " :" << iz << " CHECKING crossing WITH " << isodVal << " DATA " << data << " RIGHT " << data1x << " ABOVE " << data1y << " RIGHT_ABOVE " << data1x1y << std::endl;
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY " << ix << " : " << iy << " :" << iz << " CHECKING TRIANGLES IN SQUARE " << cx  << " " << cx1x << " " << cy << " " << cy1y << std::endl;

	  // SUPERIOR TRIANGLE (HERE - ABOVE - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL ABOVE AND RIGHT ABOVE VALUES
	  if ((data1y >= isodVal && data1x1y < isodVal)
	      || (data1y <= isodVal && data1x1y > isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed ABOVE - RIGHT_ABOVE " << data1y << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED
	    // CHECK DIAGONAL LINE
	    if ((data>=isodVal && data1x1y<isodVal)
		|| (data<isodVal && data1x1y>isodVal)) {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	      // OK: CALCULATE THE TWO POINTS
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x1=cx+pp*(isodVal-data1y);
	      //	      G4cout << " HORABOVE pp " << pp << " " <<  (cx1x-cx) << " / " << (data1x1y-data1y) << " X1=" << x1 << " = " << cx << " + " << pp << " * " << (isodVal-data1y) << G4endl; //GDEB
	      y1=cy1y;
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2) );
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;
		       
	      size_t id1 = nVoxXY2*0 + idxy+theNoVoxelsX;
	      size_t id2 = nVoxXY2*2 + idxy;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES HOR_ABOVE & DIAGONAL " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	      // IF NOT IN DIAGONAL IT MUST BE IN VERTICAL 
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1y-data);
	      x1=cx;
	      y1=cy+pp*(isodVal-data);
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x2=cx+pp*(isodVal-data1y);
	      y2=cy1y;
	      
	      size_t id1 = nVoxXY2*1 + idxy;
	      size_t id2 = nVoxXY2*0 + idxy+theNoVoxelsX;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES VER_LEFT & HOR_ABOVE " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	    }
	    
	    // CHECK ISODOSE IS BETWEEN VOXEL AND VOXEL ABOVE VALUES
	    // CHECK VERTICAL
	  } else if((data>=isodVal && data1y<isodVal)
		    || (data<isodVal && data1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - ABOVE " << data << " " << isodVal << " " << data1y << std::endl;
	    // CROSSING MUST BE IN DIAGONAL (AS IT WAS NOT THE HORIZONTAL)
                    
	    // PT VERTICAL
	    pp=(cy1y-cy)/(data1y-data);
	    y1=cy+pp*(isodVal-data);
	    x1=cx;
	    
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;

	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*1 + idxy;
	    size_t id2 = nVoxXY2*2 + idxy;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES VER_LEFT & DIAGONAL " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	  }

	  // INFERIOR TRIANGLE (HERE - RIGHT - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL RIGHT AND VOXEL RIGHT ABOVE VALUES
	  if( (data1x>=isodVal && data1x1y<isodVal)
	      || (data1x<isodVal && data1x1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed RIGHT - RIGHT_ABOVE " << data1x << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED  
	    // CHECK DIAGONAL
	    if((data>=isodVal && data1x1y<isodVal)
	       || (data<isodVal && data1x1y>isodVal)){
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;

	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;	
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;	
    
	      size_t id1 = nVoxXY2*1 + idxy+1;
	      size_t id2 = nVoxXY2*2 + idxy;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES VER_RIGHT & DIAGONAL " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	      // IF NOT IN DIAGONAL IT MUST BE IN HORIZONTAL      
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x-data);
	      x2=cx+pp*(isodVal-data);
	      y2=cy;
	      
	      size_t id1 = nVoxXY2*1 + idxy+1;
	      size_t id2 = nVoxXY2*0 + idxy;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES VER_RIGHT & HOR_BELOW " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	    }
	    // CHECK HORIZONTAL
	  } else if((data>=isodVal && data1x<isodVal)
		    || (data<isodVal && data1x>isodVal)) { // Voxel inferior
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	    
	    // OK: CALCULATE THE TWO POINTS   	      
	    // PT HORIZONTAL
	    pp=(cx1x-cx)/(data1x-data);
	    x1=cx+pp*(isodVal-data);
	    y1=cy;
	    	
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;
	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*0 + idxy;
	    size_t id2 = nVoxXY2*2 + idxy;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY "<< ix << ":" << iy << " " << idxy << " INSERTING POINTS IN LINES HOR_BELOW & DIAGONAL " << G4ThreeVector(x1,y1,pointZ) << " " << G4ThreeVector(x2,y2,pointZ) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,y1,pointZ)),pidv(id2,G4ThreeVector(x2,y2,pointZ))));
	  }
	}
      }

      if( pointPairsIsod.size() != 0 ) DrawIsodPoints(pointPairsIsod, polyList,isodoseValues[iisod],DPOrientXY);

    }
    
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXY ENDED ISODOSE FOR PLANE Z=" << pointZ << " N olys " << polyList->GetLines().size()  << G4endl; 
    theIsodosesXY->AddLineList(polyList);
    //    exit(1); //GDEB
  } // next Z plane
}


//----------------------------------------------------------------------
void DicomVImage::BuildIsodLinesXZ(std::vector<G4double> isodoseValues)
{
  theIsodosesXZ = new DicomVLineSet("IsodosesXZ",DPOrientXZ);

  G4double voxelDimX = GetVoxelDimX();
  G4double voxelDimY = GetVoxelDimY();
  G4double voxelDimZ = GetVoxelDimZ();
  size_t nVoxXY = theNoVoxelsX*theNoVoxelsY;  
  size_t nVoxXY2 = theNoVoxelsX*theNoVoxelsY;
  G4double pp;
  G4double x1;
  G4double y1;
  G4double LL;
  G4double vx;
  G4double vy;
  G4double vz;
  G4double ss;
  G4double x2;
  G4double y2;
  //--- Loop to Y planes
  for( size_t iy = 0; iy < theNoVoxelsY; iy++ ) {
    double pointY = theMinY+(iy+0.5)*voxelDimY;
    DicomVLineList* polyList = new DicomVLineList("isodoses_Y="+GmGenUtils::ftoa(pointY),iy,DPOrientXZ);
    //--- Loop to isodose values
    for( size_t iisod = 0; iisod < isodoseValues.size(); iisod++ ) {
      std::vector<ppidv> pointPairsIsod;
      G4double isodVal = isodoseValues[iisod];
      for( size_t iz = 0; iz < theNoVoxelsZ-1; iz++ ) {
	G4double cy = theMinZ+(iz+0.5)*voxelDimZ;
	G4double cy1y = theMinZ+(iz+1.5)*voxelDimZ;
	for( size_t ix = 0; ix< theNoVoxelsX-1; ix++ ) {
	  size_t idxz = ix+theNoVoxelsX*iz;
	  G4double cx = theMinX+(ix+0.5)*voxelDimX;
	  G4double cx1x = theMinX+(ix+1.5)*voxelDimX;
	  G4double data = theData->at(ix + iy*theNoVoxelsX + iz*nVoxXY);
	  G4double data1x = theData->at((ix+1) + iy*theNoVoxelsX + iz*nVoxXY);
	  G4double data1y = theData->at(ix + iy*theNoVoxelsX + (iz+1)*nVoxXY);
	  G4double data1x1y = theData->at((ix+1) + iy*theNoVoxelsX + (iz+1)*nVoxXY);
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ " << ix << " : " << iy << " :" << iz << " CHECKING crossing WITH " << isodVal << " DATA " << data << " RIGHT " << data1x << " ABOVE " << data1y << " RIGHT_ABOVE " << data1x1y << std::endl;
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ " << ix << " : " << iy << " :" << iz << " CHECKING TRIANGLES IN SQUARE " << cx  << " " << cx1x << " " << cy << " " << cy1y << std::endl;

	  // SUPERIOR TRIANGLE (HERE - ABOVE - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL ABOVE AND RIGHT ABOVE VALUES
	  if ((data1y >= isodVal && data1x1y < isodVal)
	      || (data1y <= isodVal && data1x1y > isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed ABOVE - RIGHT_ABOVE " << data1y << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED
	    // CHECK DIAGONAL LINE
	    if ((data>=isodVal && data1x1y<isodVal)
		|| (data<isodVal && data1x1y>isodVal)) {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	      // OK: CALCULATE THE TWO POINTS
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x1=cx+pp*(isodVal-data1y);
	      //	      G4cout << " HORABOVE pp " << pp << " " <<  (cx1x-cx) << " / " << (data1x1y-data1y) << " X1=" << x1 << " = " << cx << " + " << pp << " * " << (isodVal-data1y) << G4endl; //GDEB
	      y1=cy1y;
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2) );
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;
		       
	      size_t id1 = nVoxXY2*0 + idxz+theNoVoxelsX;
	      size_t id2 = nVoxXY2*2 + idxz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES HOR_ABOVE & DIAGONAL " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	      // IF NOT IN DIAGONAL IT MUST BE IN VERTICAL 
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1y-data);
	      x1=cx;
	      y1=cy+pp*(isodVal-data);
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x2=cx+pp*(isodVal-data1y);
	      y2=cy1y;
	      
	      size_t id1 = nVoxXY2*1 + idxz;
	      size_t id2 = nVoxXY2*0 + idxz+theNoVoxelsX;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES VER_LEFT & HOR_ABOVE " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	    }
	    
	    // CHECK ISODOSE IS BETWEEN VOXEL AND VOXEL ABOVE VALUES
	    // CHECK VERTICAL
	  } else if((data>=isodVal && data1y<isodVal)
		    || (data<isodVal && data1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - ABOVE " << data << " " << isodVal << " " << data1y << std::endl;
	    // CROSSING MUST BE IN DIAGONAL (AS IT WAS NOT THE HORIZONTAL)
                    
	    // PT VERTICAL
	    pp=(cy1y-cy)/(data1y-data);
	    y1=cy+pp*(isodVal-data);
	    x1=cx;
	    
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;

	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*1 + idxz;
	    size_t id2 = nVoxXY2*2 + idxz;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES VER_LEFT & DIAGONAL " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	  }

	  // INFERIOR TRIANGLE (HERE - RIGHT - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL RIGHT AND VOXEL RIGHT ABOVE VALUES
	  if( (data1x>=isodVal && data1x1y<isodVal)
	      || (data1x<isodVal && data1x1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed RIGHT - RIGHT_ABOVE " << data1x << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED  
	    // CHECK DIAGONAL
	    if((data>=isodVal && data1x1y<isodVal)
	       || (data<isodVal && data1x1y>isodVal)){
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;

	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;	
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;	
    
	      size_t id1 = nVoxXY2*1 + idxz+1;
	      size_t id2 = nVoxXY2*2 + idxz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES VER_RIGHT & DIAGONAL " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	      // IF NOT IN DIAGONAL IT MUST BE IN HORIZONTAL      
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x-data);
	      x2=cx+pp*(isodVal-data);
	      y2=cy;
	      
	      size_t id1 = nVoxXY2*1 + idxz+1;
	      size_t id2 = nVoxXY2*0 + idxz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES VER_RIGHT & HOR_BELOW " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	    }
	    // CHECK HORIZONTAL
	  } else if((data>=isodVal && data1x<isodVal)
		    || (data<isodVal && data1x>isodVal)) { // Voxel inferior
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	    
	    // OK: CALCULATE THE TWO POINTS   	      
	    // PT HORIZONTAL
	    pp=(cx1x-cx)/(data1x-data);
	    x1=cx+pp*(isodVal-data);
	    y1=cy;
	    	
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;
	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*0 + idxz;
	    size_t id2 = nVoxXY2*2 + idxz;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ "<< ix << ":" << iz << " " << idxz << " INSERTING POINTS IN LINES HOR_BELOW & DIAGONAL " << G4ThreeVector(x1,pointY,y1) << " " << G4ThreeVector(x2,pointY,y2) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(x1,pointY,y1)),pidv(id2,G4ThreeVector(x2,pointY,y2))));
	  }
	}
      }
      
      if( pointPairsIsod.size() != 0 ) DrawIsodPoints(pointPairsIsod, polyList,isodoseValues[iisod],DPOrientXZ);
    }
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ ENDED ISODOSE FOR PLANE Y=" << pointY << " N olys " << polyList->GetLines().size()  << G4endl; 
    theIsodosesXZ->AddLineList(polyList);
    //    exit(1); //GDEB
  } // next Y plane
}

//----------------------------------------------------------------------
void DicomVImage::BuildIsodLinesYZ(std::vector<G4double> isodoseValues)
{
  theIsodosesYZ = new DicomVLineSet("IsodosesYZ",DPOrientYZ);

  G4double voxelDimX = GetVoxelDimX();
  G4double voxelDimY = GetVoxelDimY();
  G4double voxelDimZ = GetVoxelDimZ();
  size_t nVoxXY = theNoVoxelsX*theNoVoxelsY;
  //  size_t nVoxXY2 = 100*100;
  size_t nVoxXY2 = theNoVoxelsX*theNoVoxelsY;
  G4double pp;
  G4double x1;
  G4double y1;
  G4double LL;
  G4double vx;
  G4double vy;
  G4double vz;
  G4double ss;
  G4double x2;
  G4double y2;

  //--- Loop to Z planes
  for( size_t ix = 0; ix < theNoVoxelsX; ix++ ) {
    double pointX = theMinX+(ix+0.5)*voxelDimX;
    DicomVLineList* polyList = new DicomVLineList("isodoses_X="+GmGenUtils::ftoa(pointX),ix,DPOrientYZ);
    //--- Loop to isodose values
    for( size_t iisod = 0; iisod < isodoseValues.size(); iisod++ ) {
      std::vector<ppidv> pointPairsIsod;
      G4double isodVal = isodoseValues[iisod];
      for( size_t iz = 0; iz < theNoVoxelsZ-1; iz++ ) {
	G4double cy = theMinZ+(iz+0.5)*voxelDimZ;
	G4double cy1y = theMinZ+(iz+1.5)*voxelDimZ;
	for( size_t iy = 0; iy< theNoVoxelsY-1; iy++ ) {
	  size_t idyz = iy+theNoVoxelsY*iz;
	  G4double cx = theMinY+(iy+0.5)*voxelDimY;
	  G4double cx1x = theMinY+(iy+1.5)*voxelDimY;
	  G4double data = theData->at(ix + iy*theNoVoxelsX + iz*nVoxXY);
	  G4double data1x = theData->at(ix + (iy+1)*theNoVoxelsX + iz*nVoxXY);
	  G4double data1y = theData->at(ix + iy*theNoVoxelsX + (iz+1)*nVoxXY);
	  G4double data1x1y = theData->at(ix + (iy+1)*theNoVoxelsX + (iz+1)*nVoxXY);
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ " << ix << " : " << iy << " :" << iz << " CHECKING crossing WITH " << isodVal << " DATA " << data << " RIGHT " << data1x << " ABOVE " << data1y << " RIGHT_ABOVE " << data1x1y << std::endl;
	  if( DicomVerb(testVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ " << ix << " : " << iy << " :" << iz << " CHECKING TRIANGLES IN SQUARE " << cx  << " " << cx1x << " " << cy << " " << cy1y << std::endl;

	  // SUPERIOR TRIANGLE (HERE - ABOVE - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL ABOVE AND RIGHT ABOVE VALUES
	  if ((data1y >= isodVal && data1x1y < isodVal)
	      || (data1y <= isodVal && data1x1y > isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed ABOVE - RIGHT_ABOVE " << data1y << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED
	    // CHECK DIAGONAL LINE
	    if ((data>=isodVal && data1x1y<isodVal)
		|| (data<isodVal && data1x1y>isodVal)) {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	      // OK: CALCULATE THE TWO POINTS
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x1=cx+pp*(isodVal-data1y);
	      //	      G4cout << " HORABOVE pp " << pp << " " <<  (cx1x-cx) << " / " << (data1x1y-data1y) << " X1=" << x1 << " = " << cx << " + " << pp << " * " << (isodVal-data1y) << G4endl; //GDEB
	      y1=cy1y;
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2) );
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;
		       
	      size_t id1 = nVoxXY2*0 + idyz+theNoVoxelsY;
	      size_t id2 = nVoxXY2*2 + idyz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< ix << ":" << iy << " " << idyz << " INSERTING POINTS IN LINES HOR_ABOVE & DIAGONAL " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	      // IF NOT IN DIAGONAL IT MUST BE IN VERTICAL 
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1y-data);
	      x1=cx;
	      y1=cy+pp*(isodVal-data);
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x1y-data1y);
	      x2=cx+pp*(isodVal-data1y);
	      y2=cy1y;
	      
	      size_t id1 = nVoxXY2*1 + idyz;
	      size_t id2 = nVoxXY2*0 + idyz+theNoVoxelsY;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< iy << ":" << iz << " " << idyz << " INSERTING POINTS IN LINES VER_LEFT & HOR_ABOVE " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	    }
	    
	    // CHECK ISODOSE IS BETWEEN VOXEL AND VOXEL ABOVE VALUES
	    // CHECK VERTICAL
	  } else if((data>=isodVal && data1y<isodVal)
		    || (data<isodVal && data1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - ABOVE " << data << " " << isodVal << " " << data1y << std::endl;
	    // CROSSING MUST BE IN DIAGONAL (AS IT WAS NOT THE HORIZONTAL)
                    
	    // PT VERTICAL
	    pp=(cy1y-cy)/(data1y-data);
	    y1=cy+pp*(isodVal-data);
	    x1=cx;
	    
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;

	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*1 + idyz;
	    size_t id2 = nVoxXY2*2 + idyz;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< iy << ":" << iz << " " << idyz << " INSERTING POINTS IN LINES VER_LEFT & DIAGONAL " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	  }

	  // INFERIOR TRIANGLE (HERE - RIGHT - RIGHT_ABOVE)
	  // CHECK ISODOSE IS BETWEEN VOXEL RIGHT AND VOXEL RIGHT ABOVE VALUES
	  if( (data1x>=isodVal && data1x1y<isodVal)
	      || (data1x<isodVal && data1x1y>isodVal)) {
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed RIGHT - RIGHT_ABOVE " << data1x << " " << isodVal << " " << data1x1y << std::endl;
	    // CHECK WHICH OF THE OTHER TWO LINES OF THE TRIANGLE IS CROSSED  
	    // CHECK DIAGONAL
	    if((data>=isodVal && data1x1y<isodVal)
	       || (data<isodVal && data1x1y>isodVal)){
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT_ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;

	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT DIAGONAL
	      LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	      vx=(cx-cx1x)/LL;
	      vy=(cy-cy1y)/LL;
	      vz=(data-data1x1y)/LL;
	      ss=(isodVal-data1x1y)/vz;	
	      x2=cx1x+vx*ss;
	      y2=cy1y+vy*ss;	
    
	      size_t id1 = nVoxXY2*1 + idyz+1;
	      size_t id2 = nVoxXY2*2 + idyz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< iy << ":" << iz << " " << idyz << " INSERTING POINTS IN LINES VER_RIGHT & DIAGONAL " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	      // IF NOT IN DIAGONAL IT MUST BE IN HORIZONTAL      
	    } else {
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	      // OK: CALCULATE THE TWO POINTS   	      
	      // PT VERTICAL
	      pp=(cy1y-cy)/(data1x1y-data1x);
	      y1=cy+pp*(isodVal-data1x);
	      x1=cx1x;
	      
	      // PT HORIZONTAL
	      pp=(cx1x-cx)/(data1x-data);
	      x2=cx+pp*(isodVal-data);
	      y2=cy;
	      
	      size_t id1 = nVoxXY2*1 + idyz+1;
	      size_t id2 = nVoxXY2*0 + idyz;
	      if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< iy << ":" << iz << " " << idyz << " INSERTING POINTS IN LINES VER_RIGHT & HOR_BELOW " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	      pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	    }
	    // CHECK HORIZONTAL
	  } else if((data>=isodVal && data1x<isodVal)
		    || (data<isodVal && data1x>isodVal)) { // Voxel inferior
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLineYZ crossed HERE - RIGHT " << data << " " << isodVal << " " << data1x << std::endl;
	    
	    // OK: CALCULATE THE TWO POINTS   	      
	    // PT HORIZONTAL
	    pp=(cx1x-cx)/(data1x-data);
	    x1=cx+pp*(isodVal-data);
	    y1=cy;
	    	
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ crossed HERE - RIGHT ABOVE " << data << " " << isodVal << " " << data1x1y << std::endl;
	    // PT DIAGONAL
	    LL=sqrt( std::pow(cx-cx1x,2)+std::pow(cy-cy1y,2)+std::pow(data-data1x1y,2));
	    vx=(cx-cx1x)/LL;
	    vy=(cy-cy1y)/LL;
	    vz=(data-data1x1y)/LL;
	    ss=(isodVal-data1x1y)/vz;
	    x2=cx1x+vx*ss;
	    y2=cy1y+vy*ss;
	    //	    G4cout << " diago=" << LL << " vx " << vx << " vy " << vy << " vz " << vz << " ss " << ss << " " << isodVal << " - " << data1x1y << G4endl; //GDEB
	    size_t id1 = nVoxXY2*0 + idyz;
	    size_t id2 = nVoxXY2*2 + idyz;
	    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ "<< iy << ":" << iz << " " << idyz << " INSERTING POINTS IN LINES HOR_BELOW & DIAGONAL " << G4ThreeVector(pointX,x1,y1) << " " << G4ThreeVector(pointX,x2,y2) << " ID " << id1 << " : " << id2 << G4endl;
	    pointPairsIsod.push_back(ppidv(pidv(id1,G4ThreeVector(pointX,x1,y1)),pidv(id2,G4ThreeVector(pointX,x2,y2))));
	  }
	}
      }

      if( pointPairsIsod.size() != 0 ) DrawIsodPoints(pointPairsIsod, polyList,isodoseValues[iisod],DPOrientYZ);

    }
    
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesYZ ENDED ISODOSE FOR PLANE X=" << pointX << " N olys " << polyList->GetLines().size()  << G4endl; 
    theIsodosesYZ->AddLineList(polyList);
    //    exit(1); //GDEB
  } // next Z plane
}

//---------------------------------------------------
void DicomVImage::DrawIsodPoints( std::vector<ppidv> pointPairsIsod, DicomVLineList* polyList, G4double isodoseValue, DPOrientation ori ) {
  //--- Build polygons ordering points 
  std::vector<G4ThreeVector> pointsIsod;
  std::vector<ppidv>::iterator itepp2;
  if( DicomVerb(debugVerb) ) G4cout << " NOW CLASSIFY POINTS " << isodoseValue << " " << pointPairsIsod.size() << G4endl;
  
  while( pointPairsIsod.size() != 0) {
    //--- Loop until id2 is not found repeated: build one polygon
    std::vector<ppidv>::iterator itepp = pointPairsIsod.begin();
    pointsIsod.push_back(itepp->first.second);
    pointsIsod.push_back(itepp->second.second);
    size_t id1 = itepp->first.first;
    size_t id2 = itepp->second.first;
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ BUILDPOLY: first id2 " << id2 << " : " << itepp->first.first << " = " << itepp->second.second << G4endl; 
    pointPairsIsod.erase(itepp);
    G4bool bFirstDirection = true; // check points from the starting pair in one direction or another
    for( ;; ) {
      if( DicomVerb(debugVerb) ) G4cout << " LOOPING POINT " << id2 << G4endl; 
      for( itepp2 = pointPairsIsod.begin(); itepp2 != pointPairsIsod.end(); itepp2++ ) {
	if( itepp2->first.first == id2 ) {
	  id2 = itepp2->second.first;
	  if( bFirstDirection ) {
	    pointsIsod.push_back( itepp2->second.second );
	  } else {
	    pointsIsod.insert( pointsIsod.begin(), itepp2->second.second );
	  }
	  if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ BUILDPOLY: REPEATED id2_1 " << itepp2->first.first << " : " << itepp2->second.first << " = " << itepp2->second.second << G4endl;
	  break;
	}
	if( itepp2->second.first == id2 ) {	
	  id2 = itepp2->first.first;
	  if( bFirstDirection ) {
	    pointsIsod.push_back( itepp2->first.second );
	  } else {
	    pointsIsod.insert( pointsIsod.begin(), itepp2->first.second );
	  }
	  if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ BUILDPOLY: REPEATED id2_2 " << itepp2->second.first << " : " << itepp2->first.first << " = " << itepp2->first.second << G4endl;
	  break;
	}
      }
      if( itepp2 != pointPairsIsod.end() ) {
	pointPairsIsod.erase(itepp2);
	//	    G4cout << " ERASED PAIR " << pointPairsIsod.size() << G4endl; //GDEB
      } else {
	// start building polygon line in the other direction
	if( bFirstDirection ) {
	  id2 = id1;
	  bFirstDirection = true;
	  bFirstDirection = false;
	  if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ CHANGING LOOP DIRECTION " << pointsIsod.size() << G4endl;
	} else {
	  break;
	}
      }
    }
    // create DicomPolyLine
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImage::BuildIsodLinesXZ CREATED ISODOSE POLYGON " << isodoseValue << " WITH N POINTS " << pointsIsod.size() << G4endl;
    G4String lineName = "isod"+DicomVLine::GetOrientationName(ori).substr(8,2)+"_";
    DicomVLine* polygon = new DicomVLine(pointsIsod,lineName+GmGenUtils::ftoa(isodoseValue), ori);
    pointsIsod.clear();
    polyList->AddLine(polygon);
    
  }
}

//----------------------------------------------------------------------
G4bool DicomVImage::IsDose() const
{
  switch(theModality) {
  case DIM_RTDose:
  case DIM_G4dcmDose:
  case DIM_Sqdose:
  case DIM_Sqdose_sq:
  case DIM_Sqdose_err:
  case DIM_Sqdose_errRel:
    return TRUE;
  case DIM_CT:
  case DIM_NM:
  case DIM_RTStruct:
  case DIM_G4dcmCT_MateID:
  case DIM_G4dcmCT_MateDens:
  case DIM_G4dcmCT_StructID:
  case DIM_G4dcmNM:
  case DIM_GammaIndex:
  case DIM_Interfile:
  case DIM_OTHER:
      return FALSE;
  }
  G4Exception("DicomVImage::IsDose",
	      "",
	      FatalErrorInArgument,
      ("Asking for a modality that does not exist: "+GmGenUtils::itoa(theModality)+"\n PLEASE CONTACT GAMOS AUTHORS").c_str());
  return FALSE;
}


//----------------------------------------------------------------------
G4ThreeVector DicomVImage::GetPosition(int iv)
{
  if( iv < 0 ) {
    G4Exception("DicomVImage::GetPosition",
		"",
		FatalException,
		("Asking Position of NEGATIVE index "+GmGenUtils::itoa(iv)+" "+GetName()).c_str());
  }
  G4int ix = iv%theNoVoxelsX;
  G4int iy = (iv/theNoVoxelsX)%theNoVoxelsY;
  G4int iz = iv/theNoVoxelsX/theNoVoxelsY;
  return GetPosition(ix, iy, iz);
}

//----------------------------------------------------------------------
G4ThreeVector DicomVImage::GetPosition(int ix, int iy, int iz)
{
  //return GetPosition(ix, iy, iz);
  if( ix < 0  || iy < 0 || iz < 0 ) {
        G4Exception("DicomVImage::GetPosition",
		"",
		FatalException,
		("Asking Position of NEGATIVE index "+GetName()).c_str());
  }
  G4double posX = theMinX + (ix+0.5)*GetVoxelDimX();
  G4double posY = theMinY + (iy+0.5)*GetVoxelDimY();
  G4double posZ = theMinZ + (iz+0.5)*GetVoxelDimZ();
  return G4ThreeVector(posX,posY,posZ);
  
}

