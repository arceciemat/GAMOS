#include "GmSqdoseHeader.hh"
#include "Gm3ddoseHeader.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
G4bool GmSqdoseHeader::operator==(const GmSqdoseHeader& right) const
{
  G4bool equ = 1;
  if( theNoVoxelsX != right.GetNoVoxelsX() ||
    theNoVoxelsY != right.GetNoVoxelsY() ||
    theNoVoxelsZ != right.GetNoVoxelsZ() ||
    theVoxelLimitsX != right.GetVoxelLimitsX() ||
    theVoxelLimitsY != right.GetVoxelLimitsY() ||
    theVoxelLimitsZ != right.GetVoxelLimitsZ() ) {
    equ = 0;
  }
  return equ;

}

//-----------------------------------------------------------------------
G4bool GmSqdoseHeader::operator!=(const GmSqdoseHeader& right) const
{
  return !( *this == right );
}

//-----------------------------------------------------------------------
GmSqdoseHeader::GmSqdoseHeader(const Gm3ddoseHeader& dh3d)
{
  //  G4cout << " reading header file " << G4endl;
  theNoEvent = dh3d.GetNumberOfEvents();
  G4cout << " N events " << theNoEvent << G4endl;

  theNoVoxelsX = dh3d.GetNoVoxelsX();
  theNoVoxelsY = dh3d.GetNoVoxelsY();
  theNoVoxelsZ = dh3d.GetNoVoxelsZ();

  theVoxelLimitsX = dh3d.GetVoxelLimitsX();
  theVoxelLimitsY = dh3d.GetVoxelLimitsY();
  theVoxelLimitsZ = dh3d.GetVoxelLimitsZ();
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    theVoxelLimitsX[ii] *= CLHEP::cm;
  }
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    theVoxelLimitsY[ii] *= CLHEP::cm;
  }
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ){
    theVoxelLimitsZ[ii] *= CLHEP::cm;
  } 
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(warningVerb) ) G4cout << "GmSqdoseHeader(3ddose)  VoxelDim " <<theVoxelLimitsX[1]-theVoxelLimitsX[0] << " " << theVoxelLimitsY[1]-theVoxelLimitsY[0] << " " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << G4endl;
#endif
  //  G4cout << "GmSqdoseHeader(3ddose)  VoxelLimits " << theVoxelLimitsZ[0] << " " << theVoxelLimitsZ[1] << " " << theVoxelLimitsZ[2] << G4endl;

  // check first Z voxel
  if( theVoxelLimitsZ[1]-theVoxelLimitsZ[0] != theVoxelLimitsZ[2]-theVoxelLimitsZ[1] ) {
    G4cerr << "!! GmSqdoseHeader(const Gm3ddoseHeader) First Z voxel is different " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << " != " << theVoxelLimitsZ[2]-theVoxelLimitsZ[1] << " V0 " << theVoxelLimitsZ[0] << " V1 " << theVoxelLimitsZ[1] << " V2 " << theVoxelLimitsZ[2] << G4endl;
    theVoxelLimitsZ[0] = theVoxelLimitsZ[1] - (theVoxelLimitsZ[2]-theVoxelLimitsZ[1]);
    //    G4cout << "GmSqdoseHeader(3ddose) NEW VoxelLimits " << theVoxelLimitsZ[0] << " " << theVoxelLimitsZ[1] << " " << theVoxelLimitsZ[2] << G4endl;
  }

}
//-----------------------------------------------------------------------
void GmSqdoseHeader::Read( FILE* fin )
{
  theNoEvent = 1E9;
  if( GmGenUtils::freadLittleEndian4(&theNoEvent, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of events");
  }

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(-warningVerb) ) G4cout << " GmSqdoseHeader::Read NEvent " << theNoEvent << G4endl; 
#endif
  
  if( GmGenUtils::freadLittleEndian8(&theNoVoxelsX, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels X");
  }
  if( GmGenUtils::freadLittleEndian8(&theNoVoxelsY, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Y");
  }
  if( GmGenUtils::freadLittleEndian8(&theNoVoxelsZ, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Z");
  }
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(-warningVerb) ) G4cout << " GmSqdoseHeader::Read NVoxels " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
#endif
  
  float vlim;
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    if( GmGenUtils::freadLittleEndian4(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits X");
    }
    theVoxelLimitsX.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsX " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    if( GmGenUtils::freadLittleEndian4(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Y");
    }
    theVoxelLimitsY.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsY " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ) {
    if( GmGenUtils::freadLittleEndian4(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Z");
    }
    theVoxelLimitsZ.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsZ " << vlim << G4endl; //GDEB
  }

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(-warningVerb) ) {
    G4cout << " GmSqdoseHeader::Read VoxelDims " <<theVoxelLimitsX[1]-theVoxelLimitsX[0] << " " << theVoxelLimitsY[1]-theVoxelLimitsY[0] << " " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << G4endl;
    G4cout << " GmSqdoseHeader::Read Limits X: " <<theVoxelLimitsX[0] << " " << theVoxelLimitsX[theVoxelLimitsX.size()-1] << " Y: " << theVoxelLimitsY[0] << " " << theVoxelLimitsY[theVoxelLimitsY.size()-1] << " Z: " << theVoxelLimitsZ[0] << "  " << theVoxelLimitsZ[theVoxelLimitsZ.size()-1] << G4endl;
  }
#endif

  float rotxx, rotxy, rotxz, rotyx, rotyy, rotyz, rotzx, rotzy, rotzz;
  if( GmGenUtils::freadLittleEndian4(&rotxx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Rea",
		"Error",
		FatalException,
		"Problem reading rotation matrix xx");
  }
  if( GmGenUtils::freadLittleEndian4(&rotxy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xy");
  }
  if( GmGenUtils::freadLittleEndian4(&rotxz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xz");
  }
  if( GmGenUtils::freadLittleEndian4(&rotyx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yx");
  }
  if( GmGenUtils::freadLittleEndian4(&rotyy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yy");
  }
  if( GmGenUtils::freadLittleEndian4(&rotyz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yz");
  }
  if( GmGenUtils::freadLittleEndian4(&rotzx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zx");
  }
  if( GmGenUtils::freadLittleEndian4(&rotzy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zy");
  }
  if( GmGenUtils::freadLittleEndian4(&rotzz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zz");
  }

  CLHEP::HepRep3x3 rottemp(rotxx,rotyx,rotzx,  // matrix representation
                           rotxy,rotyy,rotzy,  // (inverted)
                           rotxz,rotyz,rotzz);
  
  theRotationMatrix = G4RotationMatrix( rottemp );
  //  G4cout << " GmSqdoseHeader::Read N VoxelsLimits " << theVoxelLimitsX.size() << " " << theVoxelLimitsY.size() << " " << theVoxelLimitsZ.size() << G4endl;


}



//-----------------------------------------------------------------------
void GmSqdoseHeader::Print( FILE* fout )
{
  /*  if(GmGenUtils::fwriteLittleEndian((int *)theNoEvents,
                sizeof(int),1,fout)!=1)
        printf("\n Error writing temp2. \n");
  */

  if(GmGenUtils::fwriteLittleEndian4(&theNoEvent,sizeof(float),1,fout)!=1)
    printf("\n Error writing nEvent. \n");

  //  G4cout << " GmSqdoseHeader::Print theNoEvent " << theNoEvent << G4endl;
  if(GmGenUtils::fwriteLittleEndian8(&theNoVoxelsX,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelX. \n");
  
  if(GmGenUtils::fwriteLittleEndian8(&theNoVoxelsY,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelY. \n");
  
  if(GmGenUtils::fwriteLittleEndian8(&theNoVoxelsZ,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelZ. \n");

  //  G4cout << " HEADER NEvent " << theNoEvent << " Nvoxels " << theNoVoxelsX << " " << theNoVoxelsY<< " " << theNoVoxelsZ << G4endl; //GDEB
  float* vlimx = new float[theNoVoxelsX+1];
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    vlimx[ii] = theVoxelLimitsX[ii];
    // G4cout << ii << " limx " << vlimx[ii] << " = " << theVoxelLimitsX[ii] << G4endl; //GDEB
  }
  if(GmGenUtils::fwriteLittleEndian4(vlimx, sizeof(float),theNoVoxelsX+1,fout)!=theNoVoxelsX+1)
    printf("\n Error writing VoxelLimitsX. \n");
  
  float* vlimy = new float[theNoVoxelsY+1];
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    vlimy[ii] = theVoxelLimitsY[ii];
    //       G4cout << ii << " limy " << vlimy[ii] << " = " << theVoxelLimitsY[ii] << G4endl; //GDEB
  }
  if(GmGenUtils::fwriteLittleEndian4(vlimy, sizeof(float),theNoVoxelsY+1,fout)!=theNoVoxelsY+1)
    printf("\n Error writing VoxelLimitsY. \n");
  
  float* vlimz = new float[theNoVoxelsZ+1];
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ){
    vlimz[ii] = theVoxelLimitsZ[ii];
    //    G4cout << ii << " limz " << vlimz[ii] << " = " << theVoxelLimitsZ[ii] << G4endl; //GDEB
  }
  if(GmGenUtils::fwriteLittleEndian4(vlimz, sizeof(float),theNoVoxelsZ+1,fout)!=theNoVoxelsZ+1)
    printf("\n Error writing VoxelLimitsZ. \n");

  float rotxx = theRotationMatrix.xx();
  if(GmGenUtils::fwriteLittleEndian4(&rotxx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxx. \n");
  float rotxy = theRotationMatrix.xy();
  if(GmGenUtils::fwriteLittleEndian4(&rotxy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxy. \n");
  float rotxz = theRotationMatrix.xz();
  if(GmGenUtils::fwriteLittleEndian4(&rotxz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxz. \n");
  float rotyx = theRotationMatrix.yx();
  if(GmGenUtils::fwriteLittleEndian4(&rotyx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyx. \n");
  float rotyy = theRotationMatrix.yy();
  if(GmGenUtils::fwriteLittleEndian4(&rotyy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyy. \n");
  float rotyz = theRotationMatrix.yz();
  if(GmGenUtils::fwriteLittleEndian4(&rotyz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyz. \n");
  float rotzx = theRotationMatrix.zx();
  if(GmGenUtils::fwriteLittleEndian4(&rotzx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzx. \n");
  float rotzy = theRotationMatrix.zy();
  if(GmGenUtils::fwriteLittleEndian4(&rotzy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzy. \n");
  float rotzz = theRotationMatrix.zz();
  if(GmGenUtils::fwriteLittleEndian4(&rotzz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzz. \n");

  delete [] vlimx;
  delete [] vlimy;
  delete [] vlimz;
}

//-----------------------------------------------------------------------
void GmSqdoseHeader::Displace( G4ThreeVector theDisp ) 
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
    for( size_t ix = 0; ix < theVoxelLimitsX.size(); ix++ ) {
      theVoxelLimitsX[ix] += dispX;
    }
    
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
    for( size_t iy = 0; iy < theVoxelLimitsY.size(); iy++ ) {
      theVoxelLimitsY[iy] += dispY;
    }
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
    for( size_t iz = 0; iz < theVoxelLimitsZ.size(); iz++ ) {
      theVoxelLimitsZ[iz] += dispZ;
    }
  }
  
  G4cout << " GmSqdoseHeader::Displace Xmin/max " << GetMinX()<<"/"<<GetMaxX()
	 << " Ymin/max " << GetMinY()<<"/"<<GetMaxY()
	 << " Zmin/max " << GetMinZ()<<"/"<<GetMaxZ() << G4endl;
}
  
void GmSqdoseHeader::ChangeVoxelLimits( G4int ixmin, G4int ixmax, G4int iymin, G4int iymax, G4int izmin, G4int izmax )
{
  SetNoVoxelsX(ixmax-ixmin);
  SetNoVoxelsY(iymax-iymin);
  SetNoVoxelsZ(izmax-izmin);
  std::vector<float> newVoxelLimitsX(theVoxelLimitsX);
  std::vector<float> newVoxelLimitsY(theVoxelLimitsY);
  std::vector<float> newVoxelLimitsZ(theVoxelLimitsZ);
  for( size_t ii = 0; ii < newVoxelLimitsX.size(); ii++ ) {
    G4cout << " newVoxelLimitsX " << newVoxelLimitsX[ii] << G4endl; //GDEB
  }  
  
  theVoxelLimitsX.clear();
  theVoxelLimitsY.clear();
  theVoxelLimitsZ.clear();
  for( G4int ix = ixmin; ix < ixmax+1; ix++ ) {
    theVoxelLimitsX.push_back(newVoxelLimitsX[ix]);
    G4cout << ix << " theVoxelLimitsX " << theVoxelLimitsX[theVoxelLimitsX.size()-1] << G4endl; //GDEB
  }
  for( G4int iy = iymin; iy < iymax+1; iy++ ) {
    theVoxelLimitsY.push_back(newVoxelLimitsY[iy]);
  }
  for( G4int iz = izmin; iz < izmax+1; iz++ ) {
    theVoxelLimitsZ.push_back(newVoxelLimitsZ[iz]);
  }
 
  SetNoVoxelsX(theVoxelLimitsX.size()-1);
  SetNoVoxelsY(theVoxelLimitsY.size()-1);
  SetNoVoxelsZ(theVoxelLimitsZ.size()-1);

}
