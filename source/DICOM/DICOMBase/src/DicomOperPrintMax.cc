#include "DicomOperPrintMax.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperPrintMax::DicomOperPrintMax( G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
}

//-----------------------------------------------------------------------------
void DicomOperPrintMax::Operate( DicomVImage* image )
{
  theMax = -DBL_MAX;
  size_t nVox = image->GetNoVoxels();
  std::vector<G4double>* data = image->GetData();
  G4double* dataP = &(data->at(0));
  size_t iMax = 0;
  for( size_t ii = 0; ii < nVox; ii++, dataP++ ) {
    if( *dataP > theMax ) {
      theMax = *dataP;
      iMax = ii;
    }
  }
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imVoxDimX = image->GetVoxelDimX();
  G4double imVoxDimY = image->GetVoxelDimY();
  G4double imVoxDimZ = image->GetVoxelDimZ();
  size_t ix = iMax%imNVoxX;
  size_t iy = (iMax/imNVoxX)%imNVoxY;
  size_t iz = int(iMax/imNVoxXY);
  std::cout << "@@@ DicomOperPrintMax " << image->GetName() << " MAXIMUM= " << theMax
	    << " AT ID " << iMax << " = " << ix << " : " << iy << " : " << iz
	    << " POS " << imMinX+(ix+0.5)*imVoxDimX << " " << imMinY+(iy+0.5)*imVoxDimY << " " << imMinZ+(iz+0.5)*imVoxDimZ 
	    << G4endl;
  
}
