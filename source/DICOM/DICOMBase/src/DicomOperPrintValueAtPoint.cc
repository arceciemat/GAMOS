#include "DicomOperPrintValueAtPoint.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperPrintValueAtPoint::DicomOperPrintValueAtPoint( G4ThreeVector point, G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )  
{
  thePoint = point;
}

//-----------------------------------------------------------------------------
void DicomOperPrintValueAtPoint::Operate( DicomVImage* image )
{
  std::vector<G4double>* data = image->GetData();
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imVoxDimX = image->GetVoxelDimX();
  G4double imVoxDimY = image->GetVoxelDimY();
  G4double imVoxDimZ = image->GetVoxelDimZ();
  G4int ix = GmGenUtils::GetBelowInt( (thePoint.x()-imMinX)/imVoxDimX );
  if( ix < 0 || ix >= G4int(imNVoxX) ) {
    G4Exception("DicomOperPrintValueAtPoint",
		"",
		FatalException,
		("Point X "+GmGenUtils::ftoa(thePoint.x())+" is outside image limits " + GmGenUtils::ftoa(imMinX) + " , " + GmGenUtils::ftoa(image->GetMaxX())).c_str());
  }
  G4int iy = GmGenUtils::GetBelowInt( (thePoint.y()-imMinY)/imVoxDimY );
  if( iy < 0 || iy >= G4int(imNVoxY) ) {
    G4Exception("DicomOperPrintValueAtPoint",
		"",
		FatalException,
		("Point Y "+GmGenUtils::ftoa(thePoint.y())+" is outside image limits " + GmGenUtils::ftoa(imMinY) + " , " + GmGenUtils::ftoa(image->GetMaxY())).c_str());
  }
  G4int iz = GmGenUtils::GetBelowInt( (thePoint.z()-imMinZ)/imVoxDimZ );
  if( iz < 0 || iz >= G4int(imNVoxZ) ) {
    G4Exception("DicomOperPrintValueAtPoint",
		"",
		FatalException,
		("Point Z "+GmGenUtils::ftoa(thePoint.z())+" is outside image limits " + GmGenUtils::ftoa(imMinZ) + " , " + GmGenUtils::ftoa(image->GetMaxZ())).c_str());
  }
  size_t iPt = ix+iy*imNVoxX+iz*imNVoxXY;

  std::cout << "@@@ DicomOperPrintValueAtPoint " << image->GetName() << " MAXIMUM_AT_POINT " << thePoint << " = " << data->at(iPt) 
	    << " AT ID " << iPt << " = " << ix << " : " << iy << " : " << iz
	    << G4endl;
  
}
