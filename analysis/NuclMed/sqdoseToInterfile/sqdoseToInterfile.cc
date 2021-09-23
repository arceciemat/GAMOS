/*-------------------------------------------------------
    sqdoseToInterfile
-------------------------------------------------------*/
/*------------------------------------------------------
	 Description :   GAMOS sqdose format to Interfile format    
--------------------------------------------------------*/

#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"

#include <fcntl.h>

int main(int argc,char *argv[])
{

  if(argc < 3) { 
    G4Exception("analyseSqdose",
		"Wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO ARGUMENTS: INPUT_SQDOSE_FILE OUTPUT_INTERFILE_FILE");
  }
  
  G4String theSqdoseFN;
  G4String theInterfileFN;

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-help" ) {
      G4cout << "  -fIn    input sqdose file" << G4endl
	     << "  -fOut   output interfile file" << G4endl
	     << "  -help    prints the set of arguments" << G4endl;
      return 0;
    }
  } else if( argc == 3 ) {
      theSqdoseFN = argv[1];
      theInterfileFN = argv[2];

  } else {
    if(argc%2 != 1) { 
    G4Exception("analyseSqdose",
		"Wrong argument",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-fIn" ) {
	theSqdoseFN = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theInterfileFN = argv[ii+1];
      }
    }
  }

  G4cout << " OPENING SQDOSE FILE " << theSqdoseFN << G4endl; //GDEB
  GmSqdose sqDose;
  sqDose = GmSqdose();
  sqDose.Read(theSqdoseFN);
  //  G4int nRead = sqDose.GetDoses().size();

  GmSqdoseHeader* doseHeader = sqDose.GetHeader();
  G4float theNEvents = doseHeader->GetNumberOfEvents();

  G4int nVoxelsX = doseHeader->GetNoVoxelsX();
  G4int nVoxelsY = doseHeader->GetNoVoxelsY();
  G4int nVoxelsZ = doseHeader->GetNoVoxelsZ();
  G4int nVoxels = nVoxelsX*nVoxelsY*nVoxelsZ;
  float voxelWidthX = doseHeader->GetVoxelHalfX()*2.;
  float voxelWidthY = doseHeader->GetVoxelHalfY()*2.;
  float voxelWidthZ = doseHeader->GetVoxelHalfZ()*2.;

  FILE* theOutFileHV=fopen((theInterfileFN+".hv").c_str(), "w");

  char name_hv[512];
  char name_v[512];
  sprintf(name_hv,"%s",(theInterfileFN+".hv").c_str());
  sprintf(name_v,"%s",(theInterfileFN+".v").c_str());

  //strcat(name_hv, ".hv");
    
  fprintf (theOutFileHV, "!INTERFILE  := \n");
  fprintf (theOutFileHV, "name of data file := %s\n",name_v);
  fprintf (theOutFileHV, "!GENERAL DATA := \n");
  fprintf (theOutFileHV, "!GENERAL IMAGE DATA :=\n");
  fprintf (theOutFileHV, "!type of data := sqdose\n");
  //  fprintf (theOutFileHV, "!version of keys := 3.3\n");
  //  fprintf (theOutFileHV, "!data offset in bytes := 0\n");
  //  fprintf (file_stream, "patient id := N_DIR: %d SPAN %d, F_NORMA %f \n", Nx, 0, 0.0);
  fprintf (theOutFileHV, "imagedata byte order := littleendian\n");
  //fprintf (theOutFileHV, "!PET STUDY (General) :=\n");
  // fprintf (theOutFileHV, "!PET data type := 3D-Sinogram\n");
  //fprintf (theOutFileHV, "process status := Reconstructed\n");
  fprintf (theOutFileHV, "!number format := float\n");
  fprintf (theOutFileHV, "!number of bytes per pixel := 4\n");
  fprintf (theOutFileHV, "number of dimensions := 3\n");
  fprintf (theOutFileHV, "matrix axis label [1] := x\n");
  fprintf (theOutFileHV, "!matrix size [1] := %i\n",nVoxelsX);  //-1?
  fprintf (theOutFileHV, "scaling factor (mm/pixel) [1] := %f\n",voxelWidthX); 
  fprintf (theOutFileHV, "matrix axis label [2] := y\n"); 
  fprintf (theOutFileHV, "!matrix size [2] := %i\n",nVoxelsY); 
  fprintf (theOutFileHV, "scaling factor (mm/pixel) [2] := %f\n",voxelWidthY);
  fprintf (theOutFileHV, "matrix axis label [3] := z\n");
  fprintf (theOutFileHV, "!matrix size [3] := %i\n",nVoxelsZ);
  fprintf (theOutFileHV, "scaling factor (mm/pixel) [3] := %f\n",voxelWidthZ);
  fprintf (theOutFileHV, "number of time frames := 1\n");
  fprintf (theOutFileHV, "image scaling factor[1] := 1\n");
  fprintf (theOutFileHV, "data offset in bytes[1] := 0\n");
  fprintf (theOutFileHV, "quantification units := 1\n");
  fprintf (theOutFileHV, "!END OF INTERFILE := \n");
    
  fclose(theOutFileHV);
    
  std::vector<float> theDataV = sqDose.GetDoses();
  float *theData;
  //theData = calloc( nVoxels,sizeof(float));
  theData = (float*) malloc( nVoxels*sizeof(float));
  for( int ii = 0; ii < nVoxels; ii++ ) {
    theData[ii] = theDataV[ii]*theNEvents;
  }
  // int theOutFileV = open ((theInterfileFN+".v").c_str(),O_BINARY|O_RDWR|O_CREAT,S_IREAD|S_IWRITE ,0);
  // int nWritten = write(theOutFileV,theData,nVoxels*sizeof(float));
  // close (theOutFileV);
  //  G4cout << " N_WRITTEN " << nWritten << " N_VOXELS " << nVoxels << G4endl;
  FILE* theOutFileV=fopen(name_v, "wb");
  fwrite(theData,1,nVoxels*sizeof(float), theOutFileV);
  fclose(theOutFileV);
  free(theData);
  G4cout << " WRITTEN N_VOXELS " << nVoxels << G4endl;



}

