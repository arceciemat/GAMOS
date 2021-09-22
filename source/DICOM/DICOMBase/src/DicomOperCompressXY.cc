#include "DicomOperCompressXY.hh"
#include "DicomMgr.hh"
#include "DicomVImage.hh"
#include "DicomVerbosity.hh"

//-----------------------------------------------------------------------------
DicomOperCompressXY::DicomOperCompressXY( G4int iCompres, G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name ), theCompression( iCompres )
{
  if( theName == "" ) theName = "OperCompressXY";
  
}

//-----------------------------------------------------------------------------
//void DicomOperCompressXY::BuildMaterials( DicomVImage* image )
void DicomOperCompressXY::Operate( DicomVImage* image )
{
  if( DicomVerb(warningVerb) ) G4cout << " DicomOperCompressXY::Operate " << theCompression << G4endl; 
  if( theCompression == 1 ) return;
  
  std::vector<G4double>* imageData = image->GetData();

  //--- Check if compression is possible
  if( image->GetNoVoxelsX()%theCompression != 0 || image->GetNoVoxelsY()%theCompression != 0 ) {
    G4Exception("DPOperCompress::Operate",
		"",
		FatalException,
		("Compression factor = " + std::to_string(theCompression) + " has to be a divisor of Number of voxels X = " + std::to_string(image->GetNoVoxelsX()) + " and Y " + std::to_string(image->GetNoVoxelsY())).c_str());
  }


  size_t noVoxelXnew = image->GetNoVoxelsX()/theCompression;
  size_t noVoxelYnew = image->GetNoVoxelsY()/theCompression;
  size_t nData = noVoxelXnew * noVoxelYnew * image->GetNoVoxelsZ();
  std::vector<G4double>* imageDataNew = new std::vector<G4double>(nData);

  G4int nVoxX = image->GetNoVoxelsX();
  G4int nVoxY = image->GetNoVoxelsY();
  G4int nVoxZ = image->GetNoVoxelsZ();
  G4int nVoxXY = nVoxX*nVoxY;
  size_t copyNo = 0;
  for( G4int iz = 0; iz < nVoxZ; iz ++ ) {
    for( G4int ir = 0; ir < nVoxY; ir += theCompression ) {
      for( G4int ic = 0; ic < nVoxX; ic += theCompression ) {
	double meanHV = 0.;    
	meanHV = 0.;
	int isumrMax = std::min(ir+theCompression,nVoxY); // rows
	int isumcMax = std::min(ic+theCompression,nVoxX); // columns
	for( int isumr = ir; isumr < isumrMax; isumr ++ ) {
	  for( int isumc = ic; isumc < isumcMax; isumc ++ ) {
	    meanHV += imageData->at(isumc+isumr*nVoxX+iz*nVoxXY);
	    //	    G4cout << iz << " " << isumr << " " << isumc << " GET mean " << meanHV << G4endl;
	  }
	}
	meanHV /= (isumrMax-ir)*(isumcMax-ic);
	imageDataNew->at(copyNo++) = meanHV;
      }
    }
  }

  image->SetData(imageDataNew);
  image->SetNoVoxelsX(noVoxelXnew);
  image->SetNoVoxelsY(noVoxelYnew);
  
}
