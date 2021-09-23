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
  if( fread(&theNoEvent, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of events");
  }

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(warningVerb) ) G4cout << " GmSqdoseHeader::Read NEvent " << theNoEvent << G4endl; 
#endif
  
  if( fread(&theNoVoxelsX, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels X");
  }
  if( fread(&theNoVoxelsY, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Y");
  }
  if( fread(&theNoVoxelsZ, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Z");
  }
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(warningVerb) ) G4cout << " GmSqdoseHeader::Read NVoxels " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
#endif
  
  float vlim;
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits X");
    }
    theVoxelLimitsX.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsX " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Y");
    }
    theVoxelLimitsY.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsY " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ) {
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Z");
    }
    theVoxelLimitsZ.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsZ " << vlim << G4endl; //GDEB
  }

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(warningVerb) ) {
    G4cout << " GmSqdoseHeader::Read VoxelDims " <<theVoxelLimitsX[1]-theVoxelLimitsX[0] << " " << theVoxelLimitsY[1]-theVoxelLimitsY[0] << " " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << G4endl;
    G4cout << " GmSqdoseHeader::Read Limits X: " <<theVoxelLimitsX[0] << " " << theVoxelLimitsX[theVoxelLimitsX.size()-1] << " Y: " << theVoxelLimitsY[0] << " " << theVoxelLimitsY[theVoxelLimitsY.size()-1] << " Z: " << theVoxelLimitsZ[0] << "  " << theVoxelLimitsZ[theVoxelLimitsZ.size()-1] << G4endl;
  }
#endif

  float rotxx, rotxy, rotxz, rotyx, rotyy, rotyz, rotzx, rotzy, rotzz;
  if( fread(&rotxx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Rea",
		"Error",
		FatalException,
		"Problem reading rotation matrix xx");
  }
  if( fread(&rotxy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xy");
  }
  if( fread(&rotxz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xz");
  }
  if( fread(&rotyx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yx");
  }
  if( fread(&rotyy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yy");
  }
  if( fread(&rotyz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yz");
  }
  if( fread(&rotzx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zx");
  }
  if( fread(&rotzy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zy");
  }
  if( fread(&rotzz, sizeof(float),  1, fin) != 1) {
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
  /*  if(fwrite((int *)theNoEvents,
                sizeof(int),1,fout)!=1)
        printf("\n Error writing temp2. \n");
  */

  if(fwrite(&theNoEvent,sizeof(float),1,fout)!=1)
    printf("\n Error writing nEvent. \n");

  //  G4cout << " GmSqdoseHeader::Print theNoEvent " << theNoEvent << G4endl;
  if(fwrite(&theNoVoxelsX,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelX. \n");
  
  if(fwrite(&theNoVoxelsY,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelY. \n");
  
  if(fwrite(&theNoVoxelsZ,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelZ. \n");

  G4cout << " HEADER NEvent " << theNoEvent << " Nvoxels " << theNoVoxelsX << " " << theNoVoxelsY<< " " << theNoVoxelsZ << G4endl;
  float* vlimx = new float[theNoVoxelsX+1];
  for( size_t ii = 0; ii < theNoVoxelsX+1; ii++ ){
    vlimx[ii] = theVoxelLimitsX[ii];
    // G4cout << ii << " limx " << vlimx[ii] << " = " << theVoxelLimitsX[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimx, sizeof(float),theNoVoxelsX+1,fout)!=theNoVoxelsX+1)
    printf("\n Error writing VoxelLimitsX. \n");
  
  float* vlimy = new float[theNoVoxelsY+1];
  for( size_t ii = 0; ii < theNoVoxelsY+1; ii++ ){
    vlimy[ii] = theVoxelLimitsY[ii];
    //       G4cout << ii << " limy " << vlimy[ii] << " = " << theVoxelLimitsY[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimy, sizeof(float),theNoVoxelsY+1,fout)!=theNoVoxelsY+1)
    printf("\n Error writing VoxelLimitsY. \n");
  
  float* vlimz = new float[theNoVoxelsZ+1];
  for( size_t ii = 0; ii < theNoVoxelsZ+1; ii++ ){
    vlimz[ii] = theVoxelLimitsZ[ii];
    //    G4cout << ii << " limz " << vlimz[ii] << " = " << theVoxelLimitsZ[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimz, sizeof(float),theNoVoxelsZ+1,fout)!=theNoVoxelsZ+1)
    printf("\n Error writing VoxelLimitsZ. \n");

  float rotxx = theRotationMatrix.xx();
  if(fwrite(&rotxx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxx. \n");
  float rotxy = theRotationMatrix.xy();
  if(fwrite(&rotxy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxy. \n");
  float rotxz = theRotationMatrix.xz();
  if(fwrite(&rotxz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxz. \n");
  float rotyx = theRotationMatrix.yx();
  if(fwrite(&rotyx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyx. \n");
  float rotyy = theRotationMatrix.yy();
  if(fwrite(&rotyy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyy. \n");
  float rotyz = theRotationMatrix.yz();
  if(fwrite(&rotyz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyz. \n");
  float rotzx = theRotationMatrix.zx();
  if(fwrite(&rotzx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzx. \n");
  float rotzy = theRotationMatrix.zy();
  if(fwrite(&rotzy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzy. \n");
  float rotzz = theRotationMatrix.zz();
  if(fwrite(&rotzz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzz. \n");

  delete [] vlimx;
  delete [] vlimy;
  delete [] vlimz;
}
